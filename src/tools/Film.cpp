#include "tools/Film.h"

#include "cam/PyNumber.h"
#include "io/SvgExport.h"

namespace flatcam {

namespace {

Point2D centerOf(const Shape& g) {
    const auto b = g.bounds();
    if (!b) return {0, 0};
    return {(b->xmax + b->xmin) / 2.0, (b->ymax + b->ymin) / 2.0};
}

/// La referencia de la inclinación: una esquina de la caja de `g`, o vacío para el centro de lo que se inclina.
std::optional<Point2D> skewOrigin(const std::string& ref, const Shape& g) {
    if (ref == "center") return std::nullopt;
    const auto b = g.bounds();
    const BoundingBox bb = b.value_or(BoundingBox{});
    if (ref == "topleft") return Point2D{bb.xmin, bb.ymax};
    if (ref == "bottomleft") return Point2D{bb.xmin, bb.ymin};
    if (ref == "topright") return Point2D{bb.xmax, bb.ymax};
    if (ref == "bottomright") return Point2D{bb.xmax, bb.ymin};
    return std::nullopt;  // otro texto: skew_ref queda en 'center'
}

Shape scaleShape(const Shape& g, double fx, double fy, std::optional<Point2D> origin) {
    return g.transform(AffineTransform::scaling(fx, fy, origin.value_or(centerOf(g))));
}

Shape skewShape(const Shape& g, double ax, double ay, std::optional<Point2D> origin) {
    return g.transform(AffineTransform::skew(ax, ay, origin.value_or(centerOf(g))));
}

/// MultiPolygon(lista) de Shapely 1.8: de una figura múltiple solo queda la primera parte.
Shape multiPolygon(const ShapeList& l) {
    ShapeList polys;
    for (const Shape& s : l) {
        if (s.isEmpty()) continue;
        if (s.type() == ShapeType::Polygon) polys.push_back(s);
        else if (s.type() == ShapeType::MultiPolygon) polys.push_back(s.parts().front());
    }
    return Shape::collection(polys);
}

bool truthy(const std::optional<double>& v) { return v && *v != 0.0; }

}  // namespace

FilmResult filmSvg(const ShapeList& obj, const ShapeList& box, const BoundingBox& boxBounds, const std::string& uom,
                   const FilmOptions& o) {
    double sx = o.scaleX, sy = o.scaleY;
    std::optional<Point2D> scaleReference;  // 'center'
    const double dpiRate = o.pngDpi / o.screenDpi;
    const Point2D trScaleReference{boxBounds.xmin, boxBounds.ymin};
    if (dpiRate != 1 && o.fileType == "png") {
        // Como el original: suma la relación de los dpi al factor.
        sx += dpiRate;
        sy += dpiRate;
        scaleReference = trScaleReference;
    }
    const Shape boxGeo = Shape::unaryUnion(box);
    const std::optional<Point2D> skewRef = skewOrigin(o.skewReference, boxGeo);

    // La caja: las transformaciones se acumulan.
    Shape tbox = boxGeo;
    if (sx != 0.0 && sy == 0.0) tbox = scaleShape(tbox, sx, 1.0, trScaleReference);
    else if (sx == 0.0 && sy != 0.0) tbox = scaleShape(tbox, 1.0, sy, trScaleReference);
    else if (sx != 0.0 && sy != 0.0) tbox = scaleShape(tbox, sx, sy, trScaleReference);
    const bool kx = truthy(o.skewX), ky = truthy(o.skewY);
    if (kx && !ky) tbox = skewShape(tbox, *o.skewX, 0.0, skewRef);
    else if (!kx && ky) tbox = skewShape(tbox, 0.0, *o.skewY, skewRef);
    else if (kx && ky) tbox = skewShape(tbox, *o.skewX, *o.skewY, skewRef);
    if (o.mirror == "x") tbox = scaleShape(tbox, 1.0, -1.0, std::nullopt);
    if (o.mirror == "y") tbox = scaleShape(tbox, -1.0, 1.0, std::nullopt);
    if (o.mirror == "both") tbox = scaleShape(tbox, -1.0, -1.0, std::nullopt);
    const BoundingBox bounds = tbox.bounds().value_or(BoundingBox{});
    const double w = bounds.xmax - bounds.xmin, h = bounds.ymax - bounds.ymin;

    // Geometry.export_svg: cada transformación parte de la geometría sin transformar (la última que aplica gana).
    const Shape geomSvg = Shape::unaryUnion(obj);
    const std::optional<Point2D> objSkewRef = skewOrigin(o.skewReference, geomSvg);
    Shape geom = geomSvg;
    if (sx != 0.0 && sy == 0.0) geom = scaleShape(geomSvg, sx, 1.0, scaleReference);
    else if (sx == 0.0 && sy != 0.0) geom = scaleShape(geomSvg, 1.0, sy, scaleReference);
    else if (sx != 0.0 && sy != 0.0) geom = scaleShape(geomSvg, sx, sy, scaleReference);
    if (kx && !ky) geom = skewShape(geomSvg, *o.skewX, 0.0, objSkewRef);
    else if (!kx && ky) geom = skewShape(geomSvg, 0.0, *o.skewY, objSkewRef);
    else if (kx && ky) geom = skewShape(geomSvg, *o.skewX, *o.skewY, objSkewRef);
    if (o.mirror == "x") geom = scaleShape(geomSvg, 1.0, -1.0, std::nullopt);
    if (o.mirror == "y") geom = scaleShape(geomSvg, -1.0, 1.0, std::nullopt);
    if (o.mirror == "both") geom = scaleShape(geomSvg, -1.0, -1.0, std::nullopt);
    const double stroke = o.strokeFactor <= 0 ? 0.01 : o.strokeFactor;

    // ET.fromstring: se cambian los atributos de los hijos directos de la raíz (un polígono suelto no tiene hijos).
    XmlNode root = parseMiniXml(shapelySvg(geom, stroke));
    for (XmlNode& c : root.children) {
        if (c.tag.empty()) continue;
        c.set("fill", o.negative ? "#FFFFFF" : o.color);
        c.set("opacity", o.negative ? "1.0" : o.opacity);
        c.set("stroke", o.negative ? "#FFFFFF" : o.color);
    }
    const double boundary = o.negative ? o.boundary : (std::string("mm").find(uom) != std::string::npos ? 1.0 : 0.0393701);
    const std::string svgwidth = pyFloatRepr(w + 2 * boundary), svgheight = pyFloatRepr(h + 2 * boundary);
    const std::string minx = pyFloatRepr(bounds.xmin - boundary);
    const std::string miny = pyFloatRepr(bounds.ymin + boundary + h);
    if (o.negative) {
        XmlNode rect;
        rect.tag = "rect";
        rect.attrs = {{"x", minx},
                      {"y", pyFloatRepr(bounds.ymin - boundary)},
                      {"width", svgwidth},
                      {"height", svgheight},
                      {"id", "neg_rect"},
                      {"style", "fill:#000000;opacity:1.0;stroke-width:0.0"}};
        root.children.insert(root.children.begin(), rect);
    }
    std::string header = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
    header += "width=\"" + svgwidth + uom + "\" ";
    header += "height=\"" + svgheight + uom + "\" ";
    header += "viewBox=\"" + minx + " -" + miny + " " + svgwidth + " " + svgheight + "\" ";
    header += ">";
    header += "<g transform=\"scale(1,-1)\">";
    // str(ET.tostring(root)): el texto de los bytes, con "b'" y "'" (quedan como texto dentro del grupo).
    const std::string doc = header + "b'" + lxmlToString(root) + "'" + "</g> </svg>";
    FilmResult r;
    r.svg = prettyMiniXml(parseMiniXml(doc));
    r.bounds = bounds;
    r.width = w + 2 * boundary;
    r.height = h + 2 * boundary;
    return r;
}

Shape filmPunchExcellon(const ShapeList& film, const ShapeList& drills) {
    return multiPolygon(film).difference(multiPolygon(drills));
}

std::optional<Shape> filmPunchPads(const Gerber& g, double punchSize, std::string& error) {
    ShapeList punching;
    for (const std::string& code : g.apertureOrder()) {
        const Aperture& ap = g.apertures().at(code);
        if (ap.type == "C") {
            if (punchSize >= ap.size.value_or(0.0)) {
                error = "size";
                return std::nullopt;
            }
        } else if (ap.type == "R" || ap.type == "O") {
            if (punchSize >= ap.width || punchSize >= ap.height) {
                error = "size";
                return std::nullopt;
            }
        } else {
            error = "keyerror";  // P, AM y REG no tienen 'width': KeyError
            return std::nullopt;
        }
        for (const ApertureGeometry& e : ap.geometry)
            if (e.follow && e.follow->type() == ShapeType::Point) punching.push_back(e.follow->buffer(punchSize / 2));
    }
    const Shape punch = multiPolygon(punching);
    const ShapeList& solid = g.solidGeometry();
    const Shape temp = solid.size() == 1 && solid[0].type() == ShapeType::Polygon ? solid[0] : multiPolygon(solid);
    const Shape punched = temp.difference(punch);
    if (punched.equalsExact(temp)) {
        error = "same";
        return std::nullopt;
    }
    return punched;
}

}  // namespace flatcam
