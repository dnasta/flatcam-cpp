#include "io/SvgExport.h"

#include <vector>

#include "cam/PyNumber.h"

namespace flatcam {

namespace {

/// "{},{}".format(*c)
std::string pt(const Point2D& c) { return pyFloatRepr(c.x) + "," + pyFloatRepr(c.y); }

std::string polygonSvg(const Shape& p, double sf, std::string fill) {
    if (p.isEmpty()) return "<g />";
    if (fill.empty()) fill = p.isValid() ? "#66cc99" : "#ff3333";
    std::vector<std::vector<Point2D>> rings{p.exterior().coordinates()};
    for (const Shape& r : p.interiors()) rings.push_back(r.coordinates());
    std::string path;
    for (const auto& c : rings) {
        if (!path.empty()) path += " ";
        path += "M " + pt(c[0]) + " L ";
        for (std::size_t i = 1; i < c.size(); ++i) path += (i > 1 ? " L " : "") + pt(c[i]);
        path += " z";
    }
    return "<path fill-rule=\"evenodd\" fill=\"" + fill + "\" stroke=\"#555555\" stroke-width=\"" + pyFloatRepr(2. * sf) +
           "\" opacity=\"0.6\" d=\"" + path + "\" />";
}

std::string lineSvg(const Shape& l, double sf, std::string stroke) {
    if (l.isEmpty()) return "<g />";
    if (stroke.empty()) stroke = l.isValid() ? "#66cc99" : "#ff3333";
    std::string pts;
    for (const Point2D& c : l.coordinates()) pts += (pts.empty() ? "" : " ") + pt(c);
    return "<polyline fill=\"none\" stroke=\"" + stroke + "\" stroke-width=\"" + pyFloatRepr(2. * sf) + "\" points=\"" +
           pts + "\" opacity=\"0.8\" />";
}

std::string pointSvg(const Shape& p, double sf, std::string fill) {
    if (p.isEmpty()) return "<g />";
    if (fill.empty()) fill = p.isValid() ? "#66cc99" : "#ff3333";
    const Point2D c = p.coordinates().front();
    return "<circle cx=\"" + pyFloatRepr(c.x) + "\" cy=\"" + pyFloatRepr(c.y) + "\" r=\"" + pyFloatRepr(3. * sf) +
           "\" stroke=\"#555555\" stroke-width=\"" + pyFloatRepr(1. * sf) + "\" fill=\"" + fill + "\" opacity=\"0.6\" />";
}

// ---------------------------------------------------------------- un XML mínimo para imitar minidom.toprettyxml

struct Node {
    std::string tag;                                      // vacío: texto
    std::vector<std::pair<std::string, std::string>> attrs;
    std::vector<Node> children;
    std::string text;
};

/// Lee el XML que arma este archivo (elementos con atributos entre comillas dobles, sin entidades ni comentarios).
struct MiniParser {
    const std::string& s;
    std::size_t i{0};
    std::vector<Node> parseChildren(const std::string& closing) {
        std::vector<Node> out;
        while (i < s.size()) {
            if (s.compare(i, 2, "</") == 0) {
                i = s.find('>', i) + 1;
                (void)closing;
                return out;
            }
            if (s[i] == '<') {
                out.push_back(parseElement());
            } else {
                const std::size_t j = s.find('<', i);
                Node t;
                t.text = s.substr(i, j - i);
                out.push_back(t);
                i = j;
            }
        }
        return out;
    }
    Node parseElement() {
        Node n;
        ++i;  // '<'
        std::size_t j = i;
        while (s[j] != ' ' && s[j] != '>' && s[j] != '/') ++j;
        n.tag = s.substr(i, j - i);
        i = j;
        while (true) {
            while (s[i] == ' ') ++i;
            if (s[i] == '/') {
                i += 2;  // "/>"
                return n;
            }
            if (s[i] == '>') {
                ++i;
                n.children = parseChildren(n.tag);
                return n;
            }
            const std::size_t eq = s.find('=', i);
            const std::string name = s.substr(i, eq - i);
            const std::size_t q2 = s.find('"', eq + 2);
            n.attrs.push_back({name, s.substr(eq + 2, q2 - eq - 2)});
            i = q2 + 1;
        }
    }
};

std::string escape(const std::string& v) {
    std::string o;
    for (char c : v) {
        if (c == '&') o += "&amp;";
        else if (c == '<') o += "&lt;";
        else if (c == '"') o += "&quot;";
        else if (c == '>') o += "&gt;";
        else o += c;
    }
    return o;
}

/// Element.writexml / Text.writexml de minidom con addindent = "\t" y newl = "\n".
void write(const Node& n, const std::string& indent, std::string& out) {
    if (n.tag.empty()) {
        out += indent + escape(n.text) + "\n";
        return;
    }
    out += indent + "<" + n.tag;
    // minidom (expat con espacios de nombres) deja primero las declaraciones xmlns y después el resto, en su orden.
    for (int pass = 0; pass < 2; ++pass)
        for (const auto& [k, v] : n.attrs)
            if ((k.rfind("xmlns", 0) == 0) == (pass == 0)) out += " " + k + "=\"" + escape(v) + "\"";
    if (n.children.empty()) {
        out += "/>\n";
        return;
    }
    out += ">";
    if (n.children.size() == 1 && n.children[0].tag.empty()) {
        out += escape(n.children[0].text) + "</" + n.tag + ">\n";
        return;
    }
    out += "\n";
    for (const Node& c : n.children) write(c, indent + "\t", out);
    out += indent + "</" + n.tag + ">\n";
}

}  // namespace

std::string shapelySvg(const Shape& geom, double sf, const std::string& color) {
    switch (geom.type()) {
        case ShapeType::Polygon: return polygonSvg(geom, sf, color);
        case ShapeType::LineString:
        case ShapeType::LinearRing: return lineSvg(geom, sf, color);
        case ShapeType::Point: return pointSvg(geom, sf, color);
        default: {
            // Figuras múltiples: el color de la validez del conjunto, para todas las partes.
            if (geom.isEmpty()) return "<g />";
            const std::string c = color.empty() ? (geom.isValid() ? "#66cc99" : "#ff3333") : color;
            std::string out = "<g>";
            for (const Shape& p : geom.parts()) out += shapelySvg(p, sf, c);
            return out + "</g>";
        }
    }
}

std::string exportSvg(const ShapeList& shapes, const std::string& units, double scaleStrokeFactor) {
    if (shapes.empty()) return {};
    const Shape geom = Shape::unaryUnion(shapes);
    const auto b = geom.bounds();
    if (!b) return {};
    if (scaleStrokeFactor <= 0) scaleStrokeFactor = 0.01;
    const double w = b->xmax - b->xmin, h = b->ymax - b->ymin;
    const std::string svgw = pyFloatRepr(w), svgh = pyFloatRepr(h);
    std::string doc = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
    doc += "width=\"" + svgw + units + "\" height=\"" + svgh + units + "\" viewBox=\"" + pyFloatRepr(b->xmin) + " " +
           pyFloatRepr(b->ymin - h) + " " + svgw + " " + svgh + "\">";
    doc += "<g transform=\"scale(1,-1)\">" + shapelySvg(geom, scaleStrokeFactor) + "</g> </svg>";
    MiniParser p{doc};
    const Node root = p.parseElement();
    std::string out = "<?xml version=\"1.0\" ?>\n";
    write(root, "", out);
    return out;
}

}  // namespace flatcam
