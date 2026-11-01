#include "gui/objects/CNCJobObject.h"

#include <QDateTime>
#include <QFile>
#include <QLocale>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>
#include <cmath>

#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

CNCJobObject::CNCJobObject(QString name, std::unique_ptr<CncJob> job, GcodeHeaderInfo header,
                           std::vector<CncToolRow> tools)
    : FlatCAMObj(ObjectKind::CNCJob, std::move(name)), job_(std::move(job)), header_(std::move(header)),
      tools_(std::move(tools)) {
    outlineColor = colorFromFlatCam("#4650BDFF");  // cncjob_plot_line
    fillColor = colorFromFlatCam("#5E6CFFFF");     // cncjob_plot_fill
}

QColor CNCJobObject::travelColor() { return colorFromFlatCam("#B5AB3A4c"); }  // cncjob_travel_line

std::string CNCJobObject::exportText() const {
    GcodeHeaderInfo h = header_;
    h.name = name().toStdString();
    // gcode_header: "Created on Friday, 30 October 2026 at 16:59" (strftime en inglés).
    h.createdOn = QLocale(QLocale::English).toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm")
                      .toStdString();
    return job_->exportGcode(h);
}

bool CNCJobObject::saveTo(const QString& filename) const {
    QFile f(filename);
    // Modo texto: fines de línea del sistema, como el `open(filename, 'w')` de Python (en Windows, \r\n).
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    out << QString::fromStdString(exportText());
    return out.status() == QTextStream::Ok;
}

PlotLayerData CNCJobObject::plotData() const {
    // Se recorre el G-code: rápidos con la herramienta arriba = viajes; avances con Z de corte = cortes; cada bajada en
    // un taladrado = un agujero del diámetro de la herramienta en uso.
    PlotLayerData d;
    d.lineColor = outlineColor;
    d.fillColor = fillColor;
    d.travelColor = travelColor();
    static const QRegularExpression reX(R"(X(-?[\d.]+))"), reY(R"(Y(-?[\d.]+))"), reZ(R"(Z(-?[\d.]+))"),
        reDia(R"(Tool Dia = ([\d.]+))");
    double x = 0, y = 0, z = 1, dia = tools_.empty() ? 0.1 : tools_.front().dia;
    bool laserOn = false;
    // Taladrado: cada bajada sin X,Y es un agujero. Geometría: lo que corta se dibuja como la trayectoria engrosada
    // con el radio de la herramienta (plot2: buffer(tooldia / 1.99999999)), rellena.
    const bool drilling = !tools_.empty() && (tools_.front().drills > 0 || tools_.front().slots > 0);
    std::vector<std::vector<Point2D>> cuts;
    std::vector<double> cutDia;
    double xmin = 1e300, ymin = 1e300, xmax = -1e300, ymax = -1e300;
    auto grow = [&](double px, double py) {
        xmin = std::min(xmin, px);
        xmax = std::max(xmax, px);
        ymin = std::min(ymin, py);
        ymax = std::max(ymax, py);
    };
    auto addCircle = [&](double cx, double cy, double r) {
        constexpr int n = 32;
        for (int i = 0; i < n; ++i) {
            const double a0 = 2 * M_PI * i / n, a1 = 2 * M_PI * (i + 1) / n;
            d.fill.insert(d.fill.end(), {float(cx), float(cy), float(cx + r * std::cos(a0)), float(cy + r * std::sin(a0)),
                                         float(cx + r * std::cos(a1)), float(cy + r * std::sin(a1))});
            d.lines.insert(d.lines.end(), {float(cx + r * std::cos(a0)), float(cy + r * std::sin(a0)),
                                           float(cx + r * std::cos(a1)), float(cy + r * std::sin(a1))});
        }
        grow(cx - r, cy - r);
        grow(cx + r, cy + r);
    };
    const QStringList lines = QString::fromStdString(job_->gcode()).split('\n');
    for (const QString& l : lines) {
        if (const auto m = reDia.match(l); m.hasMatch()) dia = m.captured(1).toDouble();
        if (l.startsWith("M03")) laserOn = true;
        if (l.startsWith("M05") || l.startsWith("M5")) laserOn = false;
        const bool rapid = l.startsWith("G00"), feed = l.startsWith("G01");
        if (!rapid && !feed) continue;
        const auto mx = reX.match(l), my = reY.match(l), mz = reZ.match(l);
        const double nx = mx.hasMatch() ? mx.captured(1).toDouble() : x;
        const double ny = my.hasMatch() ? my.captured(1).toDouble() : y;
        if (mz.hasMatch()) {
            const double nz = mz.captured(1).toDouble();
            if (drilling && feed && nz < 0 && z >= 0 && !mx.hasMatch() && !my.hasMatch()) addCircle(x, y, dia / 2);
            z = nz;
        }
        if (mx.hasMatch() || my.hasMatch()) {
            // Con el láser (GRBL_laser) no hay Z: corta lo que se recorre con M03 encendido.
            const bool cutting = feed && (z < 0 || laserOn);
            if (cutting && !drilling) {
                // Tramos seguidos forman una sola trayectoria.
                if (cuts.empty() || cuts.back().back().x != x || cuts.back().back().y != y || cutDia.back() != dia) {
                    cuts.push_back({{x, y}});
                    cutDia.push_back(dia);
                }
                cuts.back().push_back({nx, ny});
            } else {
                auto& dst = cutting ? d.lines : d.travel;
                dst.insert(dst.end(), {float(x), float(y), float(nx), float(ny)});
            }
            grow(x, y);
            grow(nx, ny);
            x = nx;
            y = ny;
        }
    }
    for (std::size_t i = 0; i < cuts.size(); ++i) {
        // Desvío consciente (solo visual): 16 segmentos por cuarto de círculo en vez de cncjob_steps_per_circle (64).
        const Shape path = Shape::lineString(cuts[i]).buffer(cutDia[i] / 1.99999999, 16).simplify(0.0005);
        addShape(path, d, true);
        if (const auto b = path.bounds()) {
            grow(b->xmin, b->ymin);
            grow(b->xmax, b->ymax);
        }
    }
    if (xmax >= xmin) d.bounds = QRectF(xmin, ymin, xmax - xmin, ymax - ymin);
    return d;
}

QRectF CNCJobObject::bounds() const { return plotData().bounds; }

QWidget* CNCJobObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<CNCObjectUI>();
    static_cast<CNCObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

}  // namespace flatcam::gui
