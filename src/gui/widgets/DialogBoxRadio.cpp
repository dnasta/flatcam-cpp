#include "gui/widgets/DialogBoxRadio.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

#include "gui/Translation.h"

namespace flatcam::gui {

DialogBoxRadio::DialogBoxRadio(const QString& title, const QString& label, const QIcon& icon, const QString& initialText,
                               const QString& reference, QWidget* parent)
    : QDialog(parent) {
    const QString location = initialText.isEmpty() ? QString("(0, 0)") : initialText;
    setWindowIcon(icon);
    setWindowTitle(title);
    auto* form = new QFormLayout(this);
    auto* refLabel = new QLabel(tr_("Reference") + ":");
    refLabel->setToolTip(tr_("The reference can be:\n- Absolute -> the reference point is point (0,0)\n"
                             "- Relative -> the reference point is the mouse position before Jump"));
    ref_radio = new RadioSet({{tr_("Abs"), "abs"}, {tr_("Relative"), "rel"}}, Qt::Horizontal, false);
    ref_radio->setValue(reference);
    form->addRow(refLabel, ref_radio);
    form->addRow(new QLabel(""));
    form->addRow(new QLabel("<b>" + label + "</b>"));
    auto* locLabel = new QLabel(tr_("Location") + ":");
    locLabel->setToolTip(tr_("The Location value is a tuple (x,y).\n"
                             "If the reference is Absolute then the Jump will be at the position (x,y).\n"
                             "If the reference is Relative then the Jump will be at the (x,y) distance\n"
                             "from the current mouse location point."));
    lineEdit = new QLineEdit(this);
    lineEdit->setText(QString(location).remove('(').remove(')'));
    lineEdit->selectAll();
    lineEdit->setFocus();
    form->addRow(locLabel, lineEdit);
    auto* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    form->addRow(box);
    box->button(QDialogButtonBox::Ok)->setText(tr_("Ok"));
    box->button(QDialogButtonBox::Cancel)->setText(tr_("Cancel"));
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

DialogBoxChoice::DialogBoxChoice(const QString& title, const QIcon& icon, const QString& choice, QWidget* parent)
    : QDialog(parent) {
    setWindowIcon(icon);
    setWindowTitle(title);
    auto* form = new QFormLayout(this);
    ref_radio = new RadioSet({{tr_("Bottom Left"), "bl"},
                              {tr_("Top Left"), "tl"},
                              {tr_("Bottom Right"), "br"},
                              {tr_("Top Right"), "tr"},
                              {tr_("Center"), "c"}},
                             Qt::Vertical, false);
    ref_radio->setValue(choice);
    form->addRow(ref_radio);
    auto* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    form->addRow(box);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

}  // namespace flatcam::gui
