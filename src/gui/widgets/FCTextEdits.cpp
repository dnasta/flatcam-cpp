#include "gui/widgets/FCTextEdits.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <type_traits>

#include "gui/Translation.h"

namespace flatcam::gui {

MyCompleter::MyCompleter(QObject* parent) : QCompleter(parent) {
    setCompletionMode(QCompleter::PopupCompletion);
    connect(this, qOverload<const QString&>(&QCompleter::highlighted), this,
            [this](const QString& t) { lastSelected_ = t; });
}

template <typename Base>
FCTextAreaCommon<Base>::FCTextAreaCommon(QWidget* parent) : Base(parent) {
    completer_.setModel(&model_);
    setModelData({});
    QObject::connect(&completer_, &MyCompleter::insertText, this,
                     [this](const QString& c) { insertCompletion(c); });
    // insert_completion_click
    QObject::connect(completer_.popup(), &QAbstractItemView::clicked, this, [this] {
        Q_EMIT completer_.insertText(completer_.selected());
        completer_.setCompletionMode(QCompleter::PopupCompletion);
    });
    QObject::connect(this, &Base::undoAvailable, this, [this](bool v) { undoFlag_ = v; });
    QObject::connect(this, &Base::redoAvailable, this, [this](bool v) { redoFlag_ = v; });
}

template <typename Base>
void FCTextAreaCommon<Base>::insertCompletion(const QString& completion) {
    QTextCursor tc = this->textCursor();
    const int extra = static_cast<int>(completion.size() - completer_.completionPrefix().size());
    // la palabra ya está completa: solo un espacio
    if (extra == 0) {
        tc.insertText(" ");
        completer_.popup()->hide();
        return;
    }
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    tc.insertText(" ");
    this->setTextCursor(tc);
    completer_.popup()->hide();
}

template <typename Base>
void FCTextAreaCommon<Base>::focusInEvent(QFocusEvent* e) {
    completer_.setWidget(this);
    Base::focusInEvent(e);
}

template <typename Base>
void FCTextAreaCommon<Base>::insertFromMimeData(const QMimeData* data) {
    QString text = data->text();
    if (QApplication::keyboardModifiers() == Qt::ShiftModifier) text.replace('\\', '/');
    this->insertPlainText(text);
}

template <typename Base>
void FCTextAreaCommon<Base>::contextMenuEvent(QContextMenuEvent* e) {
    QMenu menu;
    QTextCursor tc = this->textCursor();
    const bool hasText = !tc.selectedText().isEmpty();
    auto add = [&](const QString& name, const QString& key, bool enabled, auto fn) {
        QAction* a = menu.addAction(QString("%1\t%2").arg(tr_(name), tr_(key)));
        QObject::connect(a, &QAction::triggered, this, fn);
        a->setEnabled(enabled);
    };
    add("Undo", "Ctrl+Z", undoFlag_, [this] { this->undo(); });
    add("Redo", "Ctrl+Y", redoFlag_, [this] { this->redo(); });
    menu.addSeparator();
    add("Cut", "Ctrl+X", hasText, [this] {
        QTextCursor c = this->textCursor();
        QApplication::clipboard()->clear();
        QApplication::clipboard()->setText(c.selectedText());
        c.deleteChar();
    });
    add("Copy", "Ctrl+C", hasText, [this] {
        QApplication::clipboard()->clear();
        QApplication::clipboard()->setText(this->textCursor().selectedText());
    });
    add("Paste", "Ctrl+V", true, [this] { this->textCursor().insertText(QApplication::clipboard()->text()); });
    add("Delete", "Del", true, [this] { this->textCursor().deleteChar(); });
    menu.addSeparator();
    add("Select All", "Ctrl+A", true, [this] { this->selectAll(); });
    menu.exec(e->globalPos());
}

template <typename Base>
void FCTextAreaCommon<Base>::comment() {
    const int pos = this->textCursor().position();
    this->moveCursor(QTextCursor::StartOfLine);
    if (this->textCursor().block().text().startsWith(" ")) this->moveCursor(QTextCursor::NextWord);  // salta la sangría
    this->moveCursor(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    if (this->textCursor().selectedText() == "#") {
        this->textCursor().deletePreviousChar();  // borra el '#'
        this->moveCursor(QTextCursor::NextWord, QTextCursor::KeepAnchor);  // y el espacio
        this->textCursor().removeSelectedText();
    } else {
        this->moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        this->textCursor().insertText("# ");
    }
    QTextCursor c(this->textCursor());
    c.setPosition(pos);
    this->setTextCursor(c);
}

template <typename Base>
void FCTextAreaCommon<Base>::keyPressEvent(QKeyEvent* e) {
    const int key = e->key();
    const Qt::KeyboardModifiers mod = e->modifiers();
    const bool ctrlShift = (mod & Qt::ControlModifier) && (mod & Qt::ShiftModifier);
    if (ctrlShift && key == Qt::Key_V) this->insertPlainText(QApplication::clipboard()->text().replace('\\', '/'));
    // Ctrl+/ comenta; en FCTextAreaExtended va en un `elif`, así que con Shift no (en el de texto plano, sí).
    if ((mod & Qt::ControlModifier) && key == Qt::Key_Slash &&
        (!ctrlShift || std::is_same_v<Base, QPlainTextEdit>))
        comment();
    QTextCursor tc = this->textCursor();
    if ((key == Qt::Key_Tab || key == Qt::Key_Enter || key == Qt::Key_Return) && completer_.popup()->isVisible()) {
        Q_EMIT completer_.insertText(completer_.selected());
        completer_.setCompletionMode(QCompleter::PopupCompletion);
        return;
    }
    // Los paréntesis se cierran solos; al escribir el de cierre delante de otro igual, se lo saltea.
    auto closing = [&](const QString& ch) {
        tc.select(QTextCursor::WordUnderCursor);
        if (tc.selectedText() == ch) {
            tc.movePosition(QTextCursor::Right);
            this->setTextCursor(tc);
        } else {
            tc.clearSelection();
            this->textCursor().insertText(ch);
        }
    };
    if (key == Qt::Key_BraceLeft) {
        tc.insertText("{}");
        this->moveCursor(QTextCursor::Left);
    } else if (key == Qt::Key_BracketLeft) {
        tc.insertText("[]");
        this->moveCursor(QTextCursor::Left);
    } else if (key == Qt::Key_ParenLeft) {
        tc.insertText("()");
        this->moveCursor(QTextCursor::Left);
    } else if (key == Qt::Key_BraceRight) {
        closing("}");
    } else if (key == Qt::Key_BracketRight) {
        closing("]");
    } else if (key == Qt::Key_ParenRight) {
        closing(")");
    } else {
        Base::keyPressEvent(e);
    }
    if (completerEnable) {
        tc.select(QTextCursor::WordUnderCursor);
        QRect cr = this->cursorRect();
        if (!tc.selectedText().isEmpty()) {
            completer_.setCompletionPrefix(tc.selectedText());
            completer_.popup()->setCurrentIndex(completer_.completionModel()->index(0, 0));
            cr.setWidth(completer_.popup()->sizeHintForColumn(0) +
                        completer_.popup()->verticalScrollBar()->sizeHint().width());
            completer_.complete(cr);
        } else {
            completer_.popup()->hide();
        }
    }
}

template class FCTextAreaCommon<QTextEdit>;
template class FCTextAreaCommon<QPlainTextEdit>;

// ------------------------------------------------------------------ FCTextAreaLineNumber

namespace {

/** La columna de los números (`FCTextAreaLineNumber.NumberBar`). */
class NumberBar : public QWidget {
public:
    explicit NumberBar(FCTextAreaLineNumber::PlainTextEdit* edit) : QWidget(edit), edit_(edit) { adjustWidth(1); }
    void adjustWidth(int count) {
        // tres espacios de más por el espacio que se agrega al número
        const int w = fontMetrics().horizontalAdvance(QString::number(count) + "   ");
        if (width() != w) setFixedWidth(w);
    }
    void updateContents(const QRect&, int scroll) {
        if (scroll) this->scroll(0, scroll);
        else update();
    }

protected:
    void paintEvent(QPaintEvent* e) override {
        edit_->numberbarPaint(this, e);
        QWidget::paintEvent(e);
    }

private:
    FCTextAreaLineNumber::PlainTextEdit* edit_;
};

}  // namespace

FCTextAreaLineNumber::PlainTextEdit::PlainTextEdit(QWidget* parent) : FCPlainTextAreaExtended(parent) {
    setFrameStyle(QFrame::NoFrame);
    highlight();
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [this] { highlight(); });
}

void FCTextAreaLineNumber::PlainTextEdit::highlight() {
    QTextEdit::ExtraSelection sel;
    sel.format.setBackground(palette().alternateBase());
    sel.format.setProperty(QTextFormat::FullWidthSelection, true);
    sel.cursor = textCursor();
    sel.cursor.clearSelection();
    setExtraSelections({sel});
}

void FCTextAreaLineNumber::PlainTextEdit::numberbarPaint(QWidget* bar, QPaintEvent* e) {
    const QFontMetrics fm = fontMetrics();
    const int current = document()->findBlock(textCursor().position()).blockNumber() + 1;
    QPainter painter(bar);
    painter.fillRect(e->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int line = block.blockNumber();
    double top = blockBoundingGeometry(block).translated(contentOffset()).top();
    double bottom = top + blockBoundingRect(block).height();
    while (block.isValid() && top <= e->rect().bottom()) {
        ++line;
        if (block.isVisible() && bottom >= e->rect().top()) {
            QFont f = painter.font();
            // la línea del cursor, en negrita y azul; el resto, del color del fondo del editor
            f.setBold(line == current);
            painter.setPen(line == current ? QColor(Qt::blue) : palette().base().color());
            painter.setFont(f);
            painter.drawText(QRect(0, static_cast<int>(top), bar->width(), fm.height()), Qt::AlignRight,
                             " " + QString::number(line) + "  ");
        }
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    }
}

FCTextAreaLineNumber::FCTextAreaLineNumber(QWidget* parent) : QFrame(parent) {
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    edit_ = new PlainTextEdit();
    auto* bar = new NumberBar(edit_);
    auto* hbox = new QHBoxLayout(this);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->addWidget(bar);
    hbox->addWidget(edit_);
    connect(edit_, &QPlainTextEdit::blockCountChanged, bar, [bar](int n) { bar->adjustWidth(n); });
    connect(edit_, &QPlainTextEdit::updateRequest, bar, [bar](const QRect& r, int dy) { bar->updateContents(r, dy); });
}

}  // namespace flatcam::gui
