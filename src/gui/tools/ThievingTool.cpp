#include "gui/tools/ThievingTool.h"

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/CopperThieving.h"

namespace flatcam::gui {

ThievingTool::ThievingTool(App* app, QWidget* parent) : ThievingToolUIBase(parent), app_(app) {
    setupUi();
    connect(ref_combo_type, &QComboBox::currentIndexChanged, this, [this] { onRefComboTypeChange(); });
    connect(reference_radio, &RadioSet::activatedCustom, this, [this] { onToggleReference(); });
    connect(fill_type_radio, &RadioSet::activatedCustom, this, &ThievingTool::onThievingType);
    connect(fill_button, &QPushButton::clicked, this, [this] { addCopperThieving(); });
    connect(rb_button, &QPushButton::clicked, this, [this] { addRobberBar(); });
    connect(ppm_button, &QPushButton::clicked, this, [this] { addPlatingMask(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo ThievingTool::info() { return {tr_("Copper Thieving Tool"), tr_("Copper Thieving Tool")}; }

void ThievingTool::fillCombo(FCComboBox* combo, ObjectKind kind, bool last) {
    const QString cur = combo->currentText();
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) combo->addItem(o->name());
    if (!last) combo->setCurrentIndex(0);
    else if (!cur.isEmpty() && combo->findText(cur) >= 0) combo->setValue(cur);
    else combo->setCurrentIndex(combo->count() - 1);  // is_last: el último agregado
}

void ThievingTool::setToolUi() {
    clearance_entry->setValue(defNum("tools_copper_thieving_clearance"));
    margin_entry->setValue(defNum("tools_copper_thieving_margin"));
    reference_radio->setValue(defVariant("tools_copper_thieving_reference"));
    bbox_type_radio->setValue(defVariant("tools_copper_thieving_box_type"));
    fill_type_radio->setValue(defVariant("tools_copper_thieving_fill_type"));
    area_entry->setValue(defNum("tools_copper_thieving_area"));
    dot_dia_entry->setValue(defNum("tools_copper_thieving_dots_dia"));
    dot_spacing_entry->setValue(defNum("tools_copper_thieving_dots_spacing"));
    square_size_entry->setValue(defNum("tools_copper_thieving_squares_size"));
    squares_spacing_entry->setValue(defNum("tools_copper_thieving_squares_spacing"));
    line_size_entry->setValue(defNum("tools_copper_thieving_lines_size"));
    lines_spacing_entry->setValue(defNum("tools_copper_thieving_lines_spacing"));
    rb_margin_entry->setValue(defNum("tools_copper_thieving_rb_margin"));
    rb_thickness_entry->setValue(defNum("tools_copper_thieving_rb_thickness"));
    clearance_ppm_entry->setValue(defNum("tools_copper_thieving_mask_clearance"));
    ppm_choice_radio->setValue(defVariant("tools_copper_thieving_geo_choice"));
    // INIT SECTION (la lista aplanada de los rellenos con líneas no se reinicia, como en el original).
    robber_.reset();
    robberLine_.reset();
    thieving_.clear();
    fillCombo(grb_object_combo, ObjectKind::Gerber, true);
    fillCombo(sm_object_combo, ObjectKind::Gerber, true);
    onRefComboTypeChange();
}

void ThievingTool::onRefComboTypeChange() {
    static const ObjectKind kinds[] = {ObjectKind::Gerber, ObjectKind::Excellon, ObjectKind::Geometry};
    const int i = std::max(0, ref_combo_type->currentIndex());
    fillCombo(ref_combo, kinds[std::min(i, 2)], false);
}

void ThievingTool::onToggleReference() {
    const QString ref = reference_radio->value().toString();
    const bool hideCombo = ref == "itself" || ref == "area";
    for (QWidget* w : std::initializer_list<QWidget*>{ref_combo, ref_combo_label, ref_combo_type, ref_combo_type_label})
        w->setVisible(!hideCombo);
    if (ref == "itself") {
        bbox_type_label->show();
        bbox_type_radio->show();
    } else {
        if (fill_type_radio->value().toString() == "line") {
            reference_radio->setValue(QString("itself"));
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Lines Grid works only for 'itself' reference ..."));
            return;
        }
        bbox_type_label->hide();
        bbox_type_radio->hide();
    }
}

void ThievingTool::onThievingType(const QString& choice) {
    MainGUI* ui = app_->ui();
    dots_frame->setVisible(choice == "dot");
    squares_frame->setVisible(choice == "square");
    lines_frame->setVisible(choice == "line");
    if (choice == "solid") ui->inform(tr_("Solid fill selected."));
    else if (choice == "dot") ui->inform(tr_("Dots grid fill selected."));
    else if (choice == "square") ui->inform(tr_("Squares grid fill selected."));
    else if (reference_radio->value().toString() != "itself") {
        reference_radio->setValue(QString("itself"));
        ui->inform("[WARNING_NOTCL] " + tr_("Lines Grid works only for 'itself' reference ..."));
    }
}

void ThievingTool::addCopperThieving() {
    MainGUI* ui = app_->ui();
    clearance_ = clearance_entry->value();
    margin_ = margin_entry->value();
    const QString ref = reference_radio->value().toString();
    grb_ = app_->collection()->byName(grb_object_combo->currentText());
    if (!grb_ || grb_->kind() != ObjectKind::Gerber) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return;
    }
    if (ref == "itself") {
        copperThievingRun({}, nullptr);
    } else if (ref == "area") {
        ui->inform("[WARNING_NOTCL] " + tr_("Click the start point of the area."));
        const QString name = grb_->name();
        app_->picker()->start(
            CanvasPicker::Mode::Rectangles, {},
            [this, name](const ShapeList& zones) {
                grb_ = app_->collection()->byName(name);
                if (grb_) copperThievingRun(zones, nullptr);
            },
            tr_("Click the end point of the filling area."), false);
    } else {
        FlatCAMObj* refObj = app_->collection()->byName(ref_combo->currentText());
        if (!refObj) {
            ui->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Could not retrieve object"), ref_combo->currentText()));
            return;
        }
        copperThievingRun({}, refObj);
    }
}

FlatCAMObj* ThievingTool::createGerber(FlatCAMObj* src, const QString& name, ShapeList solid, ShapeList follow,
                                       std::map<std::string, Aperture> apertures,
                                       const std::vector<std::string>& added) {
    // Una copia del Gerber de origen (opciones, formato) con la geometría nueva; las aperturas nuevas van al final.
    auto out = std::make_unique<Gerber>(static_cast<GerberObject*>(src)->gerber());
    out->replaceGeometry(std::move(solid), std::move(apertures), added);
    out->followGeometry() = std::move(follow);
    auto o = std::make_unique<GerberObject>(name, std::move(out));
    FlatCAMObj* obj = app_->addObject(std::move(o));
    app_->refreshSourceFile(obj);  // source_file = export_gerber(local_use=...)
    return obj;
}

void ThievingTool::copperThievingRun(const ShapeList& area, FlatCAMObj* ref) {
    MainGUI* ui = app_->ui();
    ui->inform(tr_("Copper Thieving Tool started. Reading parameters."));
    const Gerber& g = static_cast<GerberObject*>(grb_)->gerber();
    ThievingParams p;
    p.clearance = clearance_;
    p.margin = margin_;
    p.minArea = area_entry->value();
    const QString refSel = reference_radio->value().toString();
    p.reference = refSel == "itself" ? ThievingReference::Itself
                  : refSel == "area" ? ThievingReference::Area
                                     : ThievingReference::Box;
    p.minimalBox = bbox_type_radio->value().toString() == "min";
    const QString fill = fill_type_radio->value().toString();
    p.fill = fill == "dot" ? ThievingFill::Dot
             : fill == "square" ? ThievingFill::Square
             : fill == "line" ? ThievingFill::Line
                              : ThievingFill::Solid;
    p.dotDia = dot_dia_entry->value();
    p.dotSpacing = dot_spacing_entry->value();
    p.squareSize = square_size_entry->value();
    p.squareSpacing = squares_spacing_entry->value();
    p.lineSize = line_size_entry->value();
    p.lineSpacing = lines_spacing_entry->value();
    p.circleSteps = defInt("tools_copper_thieving_circle_steps", 64);
    ThievingBoxReference box;
    if (ref) {
        box.kind = ref->kind() == ObjectKind::Gerber     ? "gerber"
                   : ref->kind() == ObjectKind::Geometry ? "geometry"
                                                         : "excellon";
        if (auto* gg = dynamic_cast<GeometryObject*>(ref)) box.solid = gg->solidGeometry();
        else box.solid = ref->solidShapes();
    }
    ui->inform(tr_("Copper Thieving Tool. Preparing isolation polygons."));
    ui->inform(tr_("Copper Thieving Tool. Preparing areas to fill with copper."));
    ThievingResult r = copperThieving(g, p, area, box, flat_);
    if (r.thieving) thieving_ = *r.thieving;
    if (!r.ok) {
        // El original falla en su hilo (sin aviso) o muestra el error.
        if (!r.error.empty()) ui->inform("[ERROR_NOTCL] " + tr_(r.error.c_str()));
        return;
    }
    ui->inform(tr_("Copper Thieving Tool. Appending new geometry and buffering."));
    std::vector<std::string> added;
    if (!g.apertures().count("0")) added.push_back("0");
    lastCreated_ = createGerber(grb_, grb_->name() + "_thief", std::move(r.solid), g.followGeometry(),
                                std::move(r.apertures), added);
    onExit();
    ui->inform("[success] " + tr_("Copper Thieving Tool done."));
}

FlatCAMObj* ThievingTool::addRobberBar() {
    MainGUI* ui = app_->ui();
    const double margin = rb_margin_entry->value();
    rbThickness_ = rb_thickness_entry->value();
    grb_ = app_->collection()->byName(grb_object_combo->currentText());
    if (!grb_ || grb_->kind() != ObjectKind::Gerber) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    const Gerber& g = static_cast<GerberObject*>(grb_)->gerber();
    RobberBarResult r = robberBar(g, margin, rbThickness_);
    robber_ = r.bar;
    robberLine_ = r.line;
    std::vector<std::string> added;
    if (!r.newAperture.empty()) added.push_back(r.newAperture);
    lastCreated_ = createGerber(grb_, grb_->name() + "_robber", std::move(r.solid), std::move(r.follow),
                                std::move(r.apertures), added);
    onExit();
    ui->inform("[success] " + tr_("Copper Thieving Tool done."));
    return lastCreated_;
}

FlatCAMObj* ThievingTool::addPlatingMask() {
    MainGUI* ui = app_->ui();
    FlatCAMObj* sm = app_->collection()->byName(sm_object_combo->currentText());
    if (!sm || sm->kind() != ObjectKind::Gerber) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    const Gerber& g = static_cast<GerberObject*>(sm)->gerber();
    PlatingMaskResult r = platingMask(g, clearance_ppm_entry->value(), ppm_choice_radio->value().toString().at(0).toLatin1(),
                                      thieving_, robber_, robberLine_, rbThickness_);
    if (!r.ok) return nullptr;  // el original falla en su hilo (solo queda en el registro)
    plated_area_entry->setValue(QString::fromStdString(pyFloatRepr(r.platedArea)));
    // name.rpartition('.'): sin punto, el nombre queda como extensión.
    const QString name = sm->name();
    const int dot = name.lastIndexOf('.');
    const QString outname = dot < 0 ? QString("_plating_mask.%1").arg(name)
                                    : QString("%1_plating_mask.%2").arg(name.left(dot), name.mid(dot + 1));
    lastCreated_ = createGerber(sm, outname, std::move(r.solid), std::move(r.follow), std::move(r.apertures), r.added);
    // file_opened.emit("gerber", outname): registra como archivo reciente el nombre del objeto (no es un archivo).
    app_->registerRecent("gerber", outname);
    onExit();
    ui->inform("[success] " + tr_("Generating Pattern Plating Mask done."));
    return lastCreated_;
}

void ThievingTool::onExit() {
    app_->picker()->cancel();
    app_->ui()->inform("[success] " + tr_("Copper Thieving Tool exit."));
}

}  // namespace flatcam::gui
