#include "gui/editors/GCodeEditor.h"

#include <QHeaderView>
#include <QScrollArea>
#include <QTextBlock>
#include <set>

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"

namespace flatcam::gui {

namespace {

QTableWidgetItem* cell(const QString& text) {
    auto* it = new QTableWidgetItem(text);
    it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return it;
}

/** `[x for x in text.split("\n") if x != '']`. */
QStringList nonEmptyLines(const QString& text) {
    QStringList out;
    for (const QString& l : text.split('\n'))
        if (!l.isEmpty()) out << l;
    return out;
}

}  // namespace

GCodeEditor::GCodeEditor(App* app) : QObject(app), app_(app), ui_(new GCodeEditorUI()) {
    // name_entry.returnPressed -> on_name_activate (el nombre no se usa); update_gcode_button -> insert_gcode (pass);
    // el segundo "Insertar código" no está conectado.
    connect(ui_->name_entry, &QLineEdit::returnPressed, this, [this] { editedObjName_ = ui_->name_entry->text(); });
    connect(ui_->exit_editor_button, &QPushButton::clicked, this, [this] { app_->editorToObject(); });
    // ui_connect: las tablas siempre conectadas (el original solo conecta la que tiene herramientas; la otra no tiene
    // filas que elegir).
    connect(ui_->cnc_tools_table, &QTableWidget::clicked, this, [this] { onRowSelectionChange(); });
    connect(ui_->exc_cnc_tools_table, &QTableWidget::clicked, this, [this] { onRowSelectionChange(); });
    connect(ui_->cnc_tools_table->horizontalHeader(), &QHeaderView::sectionClicked, this, [this] { onToggleAllRows(); });
    connect(ui_->exc_cnc_tools_table->horizontalHeader(), &QHeaderView::sectionClicked, this,
            [this] { onToggleAllRows(); });
}

GCodeEditor::~GCodeEditor() { delete ui_; }

AppTextEditor* GCodeEditor::tab() const { return tab_; }

void GCodeEditor::setUi() {
    MainGUI* ui = app_->ui();
    // Una pestaña nueva cada vez, protegida (sin el botón de cerrar).
    auto* ed = new AppTextEditor(app_, true);
    tab_ = ed;
    FCDetachableTab2* area = ui->plot_tab_area;
    area->addTab(ed, tr_("Code Editor"));
    ed->setObjectName("gcode_editor_tab");
    for (int i = 0; i < area->count(); ++i)
        if (area->widget(i)->objectName() == ed->objectName()) area->protectTab(i);
    ui->position_label->setText("");
    ui->rel_position_label->setText("");
    ed->setCompleterEnabled(false);
    ed->buttonRun->hide();
    area->setCurrentWidget(ed);
    ui_->append_text->setPlainText(defStr("cncjob_append"));
    ui_->prepend_text->setPlainText(defStr("cncjob_prepend"));
    ui->properties_scroll_area->takeWidget();
    ui->properties_scroll_area->setWidget(ui_);
    ui->notebook->setCurrentWidget(ui->properties_tab);
    editedObjName_ = obj_->name();
    ui_->name_entry->setText(editedObjName_);
}

void GCodeEditor::buildUi() {
    // Un trabajo de taladrado muestra la tabla de Excellon; uno de geometría, la otra.
    const bool drilling = obj_->isDrilling();
    const bool any = !obj_->tools().empty();
    ui_->cnc_tools_table->setVisible(any && !drilling);
    ui_->exc_cnc_tools_table->setVisible(any && drilling);
    if (any && !drilling) buildCncToolsTable();
    if (any && drilling) buildExcellonCncTools();
}

void GCodeEditor::buildCncToolsTable() {
    FCTable* t = ui_->cnc_tools_table;
    const auto& tools = obj_->tools();
    const int dec = appDecimals();
    t->setRowCount(static_cast<int>(tools.size()) + 3);
    t->setItem(0, 1, cell(tr_("All GCode")));
    t->setItem(1, 1, cell(tr_("Header GCode")));
    t->setItem(2, 1, cell(tr_("Start GCode")));
    int row = 2;
    int idx = 0;
    for (const CncToolRow& r : tools) {
        ++idx;
        ++row;
        QString off = r.offset;
        if (!off.isEmpty()) off[0] = off[0].toUpper();
        t->setItem(row, 0, cell(QString::number(idx)));
        t->setItem(row, 1, cell(QString::number(r.dia, 'f', dec)));
        t->setItem(row, 2, cell(off));
        t->setItem(row, 3, cell(r.type));
        t->setItem(row, 4, cell(r.toolType));
        t->setItem(row, 5, new QTableWidgetItem(QString::number(r.tool)));
    }
    t->resizeColumnsToContents();
    t->resizeRowsToContents();
    t->verticalHeader()->hide();
    t->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = t->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Fixed);
    h->resizeSection(4, 40);
    t->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t->setColumnWidth(0, 20);
    t->setColumnWidth(4, 40);
    t->setMinimumHeight(t->contentHeight());
    t->setMaximumHeight(t->contentHeight());
}

void GCodeEditor::buildExcellonCncTools() {
    FCTable* t = ui_->exc_cnc_tools_table;
    const auto& tools = obj_->tools();
    const int dec = appDecimals();
    t->setRowCount(static_cast<int>(tools.size()) + 3);
    t->setItem(0, 1, cell(tr_("All GCode")));
    t->setItem(1, 1, cell(tr_("Header GCode")));
    t->setItem(2, 1, cell(tr_("Start GCode")));
    int row = 2;
    int idx = 0;
    for (const CncToolRow& r : tools) {
        ++idx;
        ++row;
        t->setItem(row, 0, cell(QString::number(idx)));
        t->setItem(row, 1, cell(QString::number(r.dia, 'f', dec)));
        t->setItem(row, 2, cell(QString::number(r.drills)));
        t->setItem(row, 3, cell(QString::number(r.slots)));
        t->setItem(row, 4, new QTableWidgetItem(QString::number(r.tool)));
        // offset + tools_drill_cutz: la Z de corte con el desplazamiento.
        t->setItem(row, 5, cell(QString::number(r.cutZ, 'f', dec)));
    }
    t->resizeColumnsToContents();
    t->resizeRowsToContents();
    t->verticalHeader()->hide();
    t->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = t->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    t->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t->setColumnWidth(0, 20);
    t->setMinimumHeight(t->contentHeight());
    t->setMaximumHeight(t->contentHeight());
}

void GCodeEditor::edit(CNCJobObject* obj) {
    obj_ = obj;
    setUi();
    buildUi();
    tab_->loadText(obj->sourceFile(), true, false, true);
    app_->ui()->inform(QString("[success] %1...").arg(tr_("Loaded Machine Code into Code Editor")));
}

void GCodeEditor::save(CNCJobObject* obj) {
    if (tab_) {
        obj->setSourceFile(tab_->toPlainText());
        tab_->buttonSave->setStyleSheet("");
        tab_->buttonSave->setIcon(QIcon(":/images/save_as.png"));
    }
    deactivate();
}

void GCodeEditor::deactivate() { obj_ = nullptr; }

void GCodeEditor::closeTab() {
    FCDetachableTab2* area = app_->ui()->plot_tab_area;
    for (int i = area->count() - 1; i >= 0; --i) {
        QWidget* w = area->widget(i);
        if (w->objectName() != "gcode_editor_tab") continue;
        area->closeTab(i);
        // En el original la pestaña se libera cuando deja de usarse; acá, después del evento en curso.
        w->deleteLater();
    }
}

void GCodeEditor::onRowSelectionChange() {
    if (!obj_ || !tab_) return;
    FCTextAreaLineNumber::PlainTextEdit* ed = tab_->plainEditor();
    const QTextDocument::FindFlags flags = QTextDocument::FindCaseSensitively;
    ed->moveCursor(QTextCursor::Start);
    const bool drilling = obj_->isDrilling();
    if (obj_->tools().empty()) return;
    FCTable* table = drilling ? ui_->exc_cnc_tools_table : ui_->cnc_tools_table;
    // Las filas elegidas, en el orden de un set de Python de enteros chicos (creciente).
    std::set<int> rows;
    for (const QModelIndex& i : table->selectionModel()->selectedIndexes()) rows.insert(i.row());
    if (rows.count(0)) {
        // En Qt 5 selectAll no emite cursorPositionChanged (en Qt 6 sí): el resaltado de la línea no cambia.
        const auto extra = ed->extraSelections();
        ed->selectAll();
        ed->setExtraSelections(extra);
        return;
    }
    // La cabecera y el código de inicio: desde la primera línea hasta la última aparición de su última línea.
    auto selectBlock = [&](const QString& text) {
        const QStringList lines = nonEmptyLines(text);
        // text_list[0] de una lista vacía: IndexError, el manejador se corta.
        if (lines.isEmpty()) return false;
        ed->find(lines.front(), flags);
        QTextCursor c = ed->textCursor();
        const int start = c.selectionStart();
        int end = 0;
        while (ed->find(lines.back(), flags)) {
            c = ed->textCursor();
            end = c.selectionEnd();
        }
        c.setPosition(start);
        c.setPosition(end, QTextCursor::KeepAnchor);
        ed->setTextCursor(c);
        return true;
    };
    if (rows.count(1) && !selectBlock(QString::fromStdString(obj_->sourceHeader()))) return;
    if (rows.count(2) && !selectBlock(QString::fromStdString(obj_->job().startGcode()))) return;

    const auto& tools = obj_->tools();
    const auto& gcodes = obj_->job().toolGcodes();
    const int dec = appDecimals();
    QList<QTextEdit::ExtraSelection> sel;
    for (int row : rows) {
        if (row <= 2) continue;
        const QTableWidgetItem* numItem = table->item(row, 0);
        if (!numItem) continue;
        const int toolNo = numItem->text().toInt();
        std::optional<QString> found;
        if (!drilling) {
            // cnc_tools[tool_no]: la clave es el número de la herramienta (si no está: KeyError, se corta todo). En un
            // trabajo leído de un proyecto las claves son textos ("1"): falla siempre.
            if (obj_->projectData.isObject()) return;
            for (size_t i = 0; i < tools.size(); ++i)
                if (tools[i].tool == toolNo) found = i < gcodes.size() ? QString::fromStdString(gcodes[i]) : QString();
            if (!found) return;
        } else {
            // En un trabajo leído de un proyecto las claves de exc_cnc_tools son textos y dec_format falla (TypeError).
            if (obj_->projectData.isObject()) return;
            // La herramienta con el mismo diámetro redondeado (la última, si se repite).
            const QTableWidgetItem* diaItem = table->item(row, 1);
            const QString dia = diaItem ? QString::number(diaItem->text().toDouble(), 'f', dec) : QString();
            for (size_t i = 0; i < tools.size(); ++i)
                if (QString::number(tools[i].dia, 'f', dec) == dia)
                    found = i < gcodes.size() ? QString::fromStdString(gcodes[i]) : QString();
            if (!found) continue;
        }
        const QStringList lines = nonEmptyLines(*found);
        if (lines.isEmpty()) return;  // IndexError en el original
        // Primero la herramienta (T<n>), después la primera línea de su G-code.
        if (!ed->find(QString("T%1").arg(toolNo), flags)) continue;
        QTextCursor c = ed->textCursor();
        c.setPosition(c.selectionStart());
        if (!ed->find(lines.front(), flags)) continue;
        c = ed->textCursor();
        const int start = c.selectionStart();
        int end = 0;
        if (!ed->find("M6", flags)) {
            // La última herramienta: hasta el final.
            ed->moveCursor(QTextCursor::End);
            c = ed->textCursor();
            end = c.selectionEnd();
        } else {
            c = ed->textCursor();
            const int m6 = c.selectionEnd();
            QTextCursor back = ed->textCursor();
            back.setPosition(start);
            ed->setTextCursor(back);
            std::vector<int> ends;
            while (ed->find(lines.back(), flags)) {
                c = ed->textCursor();
                ends.push_back(c.selectionEnd());
            }
            int best = -1;
            for (int p : ends)
                if (p < m6) best = std::max(best, p);
            if (best >= 0) {
                end = best;
            } else {
                ed->moveCursor(QTextCursor::End);
                c = ed->textCursor();
                end = c.selectionEnd();
            }
        }
        c.setPosition(start);
        c.setPosition(end, QTextCursor::KeepAnchor);
        ed->setTextCursor(c);
        QTextEdit::ExtraSelection s;
        s.cursor = ed->textCursor();
        s.format.setFontUnderline(true);
        sel << s;
    }
    ed->setExtraSelections(sel);
}

void GCodeEditor::onToggleAllRows() {
    if (!obj_ || obj_->tools().empty()) return;
    FCTable* table = obj_->isDrilling() ? ui_->exc_cnc_tools_table : ui_->cnc_tools_table;
    std::set<int> rows;
    for (const QModelIndex& i : table->selectionModel()->selectedIndexes()) rows.insert(i.row());
    // Con todas elegidas se vacía la tabla (la selección del texto queda: se limpia una copia del cursor); si no, se
    // eligen todas (sin tocar el texto: selectAll no emite clicked).
    if (static_cast<int>(rows.size()) == table->rowCount()) table->clearSelection();
    else table->selectAll();
}

}  // namespace flatcam::gui
