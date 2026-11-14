#include "gui/Preferences.h"
#include "gui/objects/CNCJobObject.h"

#include "gui/objects/CNCAutolevel.h"

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
    outlineColor = colorFromFlatCam(defStr("cncjob_plot_line"));
    fillColor = colorFromFlatCam(defStr("cncjob_plot_fill"));
}

QColor CNCJobObject::travelColor() { return colorFromFlatCam("#B5AB3A4c"); }  // cncjob_travel_line

std::string CNCJobObject::headerText() const {
    if (storedHeader_) return *storedHeader_;
    GcodeHeaderInfo h = header_;
    h.name = name().toStdString();
    // gcode_header: "Created on Friday, 30 October 2026 at 16:59" (strftime en inglés).
    h.createdOn = QLocale(QLocale::English).toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm")
                      .toStdString();
    return CncJob::gcodeHeader(h);
}

std::string CNCJobObject::preprocessorName() const {
    if (tools_.empty()) return {};
    const JsonValue& v = tools_.front().data[isDrilling() ? "tools_drill_ppname_e" : "ppname_g"];
    return v.isString() ? v.text() : std::string();
}

std::string CNCJobObject::exportText(std::string preamble, std::string postamble) const {
    return compose(headerText(), std::move(preamble), std::move(postamble));
}

std::string CNCJobObject::compose(const std::string& header, std::string preamble, std::string postamble) const {
    // export_gcode.
    if (preamble.empty()) preamble = defStr("cncjob_prepend").toStdString();
    if (postamble.empty()) postamble = defStr("cncjob_append").toStdString();
    const std::string pp = preprocessorName();
    const bool drilling = isDrilling();
    // include_header = False solo en Roland_MDX_20 e ISEL_ICP_CNC.
    const bool includeHeader = pp != "Roland_MDX_20" && pp != "ISEL_ICP_CNC";
    std::string gcode = job_->gcode();
    if (!includeHeader) {
        // Un taladrado es multitool y esta rama solo suma los G-code de cnc_tools (vacío en un taladrado): sale sin
        // cuerpo, como el original.
        if (drilling) gcode.clear();
        return preamble + "\n" + gcode + "\n" + postamble;
    }
    const std::string end = defBool("cncjob_footer") ? "M02" : "";
    // hpgl: solo se mira en los trabajos de geometría (en uno de taladrado el original recorre cnc_tools, vacío).
    if (!drilling && pp.find("hpgl") != std::string::npos) {
        static const QRegularExpression pa(R"(^PA\s*(-?\d+\.\d*),?\s*(-?\d+\.\d*)*;?$)");
        std::string body;
        // str.splitlines: también corta en \r.
        const QStringList lines = QString::fromStdString(gcode).split(QRegularExpression("\r\n|\r|\n"));
        for (int i = 0; i < lines.size(); ++i) {
            if (i == lines.size() - 1 && lines[i].isEmpty()) break;  // splitlines no da la línea vacía final
            const QRegularExpressionMatch m = pa.match(lines[i]);
            if (m.hasMatch())
                body += QString::asprintf("PA%d,%d;\n", int(m.captured(1).toDouble()), int(m.captured(2).toDouble()))
                            .toStdString();
            else
                body += lines[i].toStdString() + "\n";
        }
        return header + "\n" + job_->startGcode() + "\n" + preamble + "\n" + body + "\n" + postamble + end;
    }
    std::string g;
    if (!preamble.empty() && !postamble.empty())
        g = header + job_->startGcode() + "\n" + preamble + "\n" + gcode + "\n" + postamble + "\n" + end;
    if (preamble.empty()) g = header + job_->startGcode() + "\n" + gcode + "\n" + postamble + "\n" + end;
    if (postamble.empty()) g = header + job_->startGcode() + "\n" + preamble + "\n" + gcode + "\n" + end;
    if (preamble.empty() && postamble.empty()) g = header + job_->startGcode() + "\n" + gcode + "\n" + end;
    return g;
}

void CNCJobObject::initSourceFile() {
    prependSnippet_ = defStr("cncjob_prepend");
    appendSnippet_ = defStr("cncjob_append");
    sourceHeader_ = headerText();
    sourceFile_ = QString::fromStdString(compose(sourceHeader_, prependSnippet_.toStdString(),
                                                 appendSnippet_.toStdString()));
}

bool CNCJobObject::saveTo(const QString& filename, const std::string& preamble, const std::string& postamble) const {
    QFile f(filename);
    // cncjob_line_ending fuerza \r\n fuera de Windows (open(newline='\r\n')); si no, modo texto: fines de línea del
    // sistema, como el `open(filename, 'w')` de Python.
#ifdef Q_OS_WIN
    const bool crlf = false;
#else
    const bool crlf = defBool("cncjob_line_ending");
#endif
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Truncate;
    if (!crlf) mode |= QIODevice::Text;
    if (!f.open(mode)) return false;
    QString text = QString::fromStdString(exportText(preamble, postamble));
    if (crlf) text.replace("\n", "\r\n");
    QTextStream out(&f);
    out << text;
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
    const bool drilling = isDrilling();
    std::vector<std::vector<Point2D>> cuts, travels;
    std::vector<double> cutDia, travelDia;
    // Tramos seguidos del mismo tipo forman una sola trayectoria (gcode_parse).
    auto extend = [](std::vector<std::vector<Point2D>>& paths, std::vector<double>& dias, double x0, double y0,
                     double x1, double y1, double di) {
        if (paths.empty() || paths.back().back().x != x0 || paths.back().back().y != y0 || dias.back() != di) {
            paths.push_back({{x0, y0}});
            dias.push_back(di);
        }
        paths.back().push_back({x1, y1});
    };
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
    // En un trabajo de geometría cada herramienta tiene su propio G-code y su diámetro (el láser no lo escribe).
    std::vector<std::pair<std::string, double>> chunks;
    if (!drilling && job_->toolGcodes().size() == tools_.size()) {
        for (std::size_t i = 0; i < tools_.size(); ++i) chunks.push_back({job_->toolGcodes()[i], tools_[i].dia});
    } else {
        chunks.push_back({job_->gcode(), dia});
    }
    for (const auto& [text, chunkDia] : chunks) {
    dia = chunkDia;
    const QStringList lines = QString::fromStdString(text).split('\n');
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
            if (cutting && !drilling) extend(cuts, cutDia, x, y, nx, ny, dia);
            else if (cutting) d.lines.insert(d.lines.end(), {float(x), float(y), float(nx), float(ny)});
            else extend(travels, travelDia, x, y, nx, ny, dia);
            grow(x, y);
            grow(nx, ny);
            x = nx;
            y = ny;
        }
    }
    }
    // plot2: cada trayectoria engrosada con el radio de la herramienta y simplificada (tool_tolerance 0,0005); lo que
    // corta en azul y los viajes encima, en amarillo translúcido. Desvío consciente (solo visual): 16 segmentos por
    // cuarto de círculo en vez de cncjob_steps_per_circle (64).
    auto thick = [&](const std::vector<Point2D>& pts, double di) {
        Shape path = Shape::lineString(pts).buffer(di / 1.99999999, 16).simplify(0.0005);
        if (const auto b = path.bounds()) {
            grow(b->xmin, b->ymin);
            grow(b->xmax, b->ymax);
        }
        return path;
    };
    for (std::size_t i = 0; i < cuts.size(); ++i) addShape(thick(cuts[i], cutDia[i]), d, true);
    PlotGroup tr;
    tr.fillColor = colorFromFlatCam("#F0E24D4C");  // cncjob_travel_fill
    tr.lineColor = travelColor();                  // cncjob_travel_line
    QRectF ignored;
    for (std::size_t i = 0; i < travels.size(); ++i) addShape(thick(travels[i], travelDia[i]), tr, ignored, true);
    if (!tr.lines.empty()) d.groups.push_back(std::move(tr));
    if (xmax >= xmin) d.bounds = QRectF(xmin, ymin, xmax - xmin, ymax - ymin);
    return d;
}

QRectF CNCJobObject::bounds() const { return plotData().bounds; }

CNCAutolevel* CNCJobObject::autolevel(App* app) {
    if (!autolevel_) autolevel_ = new CNCAutolevel(app, this);
    return autolevel_;
}

QWidget* CNCJobObject::propertiesUi() {
    if (!ui_) {
        auto* panel = new CNCObjectUI();
        ui_.reset(panel);
        // set_ui: con algún fragmento en las Preferencias se marca "Usar fragmentos de código CNC".
        if (!appendSnippet_.isEmpty() || !prependSnippet_.isEmpty()) panel->snippets_cb->setChecked(true);
    }
    static_cast<CNCObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

}  // namespace flatcam::gui
