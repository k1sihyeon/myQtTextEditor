#include "qteditor.h"

#include <QTextEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QFontComboBox>
#include <QDoubleSpinBox>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QColorDialog>
#include <QFontDialog>
#include <QPrinter>
#include <QPrintDialog>

QtEditor::QtEditor(QWidget *parent)
    : QMainWindow(parent)
{
    //MDI Area
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(connectWindow(QMdiSubWindow*)));



    //Dock Widget
    QWidget *w = new QWidget(this);
    QLabel *label = new QLabel("Dock Widget", w);
    QDockWidget *dock = new QDockWidget("Dock Widget", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setWidget(w);

    // Text Edit - Central Widget
    // te = new QTextEdit(this);
    // setCentralWidget(te);

    // MenuBar
    QMenuBar *mb = new QMenuBar(this);
    setMenuBar(mb);

    // QAction *newAct = new QAction(QIcon("new.png"), "&New", this);
    // newAct->setShortcut(tr("Ctrl+N"));
    // newAct->setStatusTip(tr("make new file"));
    // connect(newAct, SIGNAL(triggered()), SLOT(newFile()));

    //tr -> 번역을 위한 예약어

//file menu
    QAction *newAct = makeAction("new.png", tr("&New"), tr("Ctrl+N"), tr("Make New File"), this, SLOT(newFile()));
    QAction *openAct = makeAction("open.png", tr("&Open"), tr("Ctrl+O"), tr("Open File"), this, SLOT(openFile()));
    QAction *saveAct = makeAction("save.png", tr("&Save"), tr("Ctrl+S"), tr("Save File"), this, SLOT(saveFile()));
    QAction *saveasAct = makeAction("saveas.png", tr("Save &As..."), "", tr("Save File As Another File Name"), this, SLOT(saveAsFile()));
    QAction *printAct = makeAction("print.png", tr("&Print"), "Ctrl+P", tr("Print File"), this, SLOT(print()));
    QAction *exitAct = makeAction("quit.png", tr("E&xit"), tr("Ctrl+Q"), tr("Exit Program"), this, SLOT(exit()));

    QMenu *fileMenu = mb->addMenu("&File");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveasAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    //QMenu *alignMenu = fileMenu->addMenu("&Align");
    //QTextEdit::setAlignment(Qt::Alignment a);

//edit menu
    QAction *undoAct = makeAction("undo.png", tr("&Undo"), "Ctrl+Z", tr("Undo work"), NULL, SLOT(undo()));
    // QAction *undoAct = makeAction("undo.png", tr("&Undo"), "Ctrl+Z", tr("Undo work"), [=] {
    //     QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    //     if (subWindow != nullptr) {
    //         QTextEdit *tmpTE = dynamic_cast<QTextEdit*>(subWindow->widget());
    //         tmpTE->undo();
    //     }
    // });
    QAction *redoAct = makeAction("redo.png", tr("&Redo"), "Ctrl+Shift+Z", tr("Redo work"), NULL, SLOT(redo()));
    QAction *copyAct = makeAction("copy.png", tr("&Copy"), "Ctrl+C", tr("Copy text"), NULL, SLOT(copy()));
    QAction *cutAct = makeAction("cut.png", tr("&Cut"), "Ctrl+X", tr("Cut text"), NULL, SLOT(cut()));
    QAction *pasteAct = makeAction("paste.png", tr("&Paste"), "Ctrl+V", tr("Paste work"), NULL, SLOT(paste()));
    QAction *zoominAct = makeAction("zoom_in.png", tr("Zoom &In"), QKeySequence::ZoomIn, tr("Zoom In Screen"), NULL, SLOT(zoomIn()));
    QAction *zoomoutAct = makeAction("zoom_out.png", tr("Zoom &Out"), QKeySequence::ZoomOut, tr("Zoom Out Screen"), NULL, SLOT(zoomOut()));

    QMenu *editMenu = mb->addMenu("&Edit");
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    editMenu->addSeparator();
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(zoominAct);
    editMenu->addAction(zoomoutAct);

    // editActions pushback
    editActions.push_back({undoAct, SLOT(undo())});
    editActions.push_back({redoAct, SLOT(redo())});
    editActions.push_back({copyAct, SLOT(copy())});
    editActions.push_back({pasteAct, SLOT(paste())});
    editActions.push_back({zoominAct, SLOT(zoomIn())});
    editActions.push_back({zoomoutAct, SLOT(zoomOut())});


//format menu
    QMenu *formatMenu = mb->addMenu("For&mat");

    QAction *fontAction = makeAction("font.png", "&Font", "Ctrl+Shift+F", "Choose Font", this, SLOT(selectFont()));
    QAction *colorAction = makeAction("color.png", "&Color", "", "Set Text Color", this, SLOT(selectColor()));

    //action vector ....

    formatMenu->addAction(fontAction);
    formatMenu->addAction(colorAction);

    //add Align Sub Menu
    QMenu *alignMenu = formatMenu->addMenu("&Align");
    alignMenu->setIcon(QIcon(":/icons/align.png"));

    // QAction *alignLeftAct = makeAction("align_left.png", "&Left", "Ctrl+L", "Align Left", [=] {
    //     te->setAlignment(Qt::AlignLeft);
    // });

    //alignText slot은 현재 이 클래스에 있으므로 te가 아닌 this임
    QAction *alignLeftAct = makeAction("align_left.png", "&Left", "Ctrl+L", "Align Left", this, SLOT(alignText()));
    QAction *alignCenterAct = makeAction("align_center.png", "&Center", "Ctrl+E", "Align Center", this, SLOT(alignText()));
    QAction *alignRightAct = makeAction("align_right.png", "&Right", "Ctrl+R", "Align Right", this, SLOT(alignText()));
    QAction *alignJustifyAct = makeAction("align_justify.png", "&Justify", "Ctrl+J", "Align Justify", this, SLOT(alignText()));

    alignMenu->addAction(alignLeftAct);
    alignMenu->addAction(alignCenterAct);
    alignMenu->addAction(alignRightAct);
    alignMenu->addAction(alignJustifyAct);

//window menu
    windowMenu = mb->addMenu("&Window");

    QAction *cascadeAct = makeAction("", "&Cascade", "", "Align Sub Windows by Cascade", mdiArea, SLOT(cascadeSubWindows()));
    QAction *tileAct = makeAction("", "&Tile", "", "Align Sub Windows by Tile", mdiArea, SLOT(tileSubWindows()));

    windowMenu->addAction(cascadeAct);
    windowMenu->addAction(tileAct);
    windowMenu->addSeparator();

//help menu
    QMenu *helpMenu = mb->addMenu("&Help");

    QAction *aboutAct = makeAction("about.png", "&About", "", "Show About Message Box", this, SLOT(about()));
    QAction *aboutQtAct = makeAction("qt.png", "About &Qt", "", "Show About Qt", qApp, SLOT(aboutQt()));

    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    /////////////////////////////////////
    /// \brief ToolBar

//file tool bar
    QToolBar *fileToolBar = addToolBar("&File");
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveasAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitAct);

//edit tool bar
    QToolBar *editTB = addToolBar("&Edit");
    editTB->setToolButtonStyle(Qt::ToolButtonIconOnly);
    editTB->addAction(undoAct);
    editTB->addAction(redoAct);
    editTB->addSeparator();
    editTB->addAction(copyAct);
    editTB->addAction(cutAct);
    editTB->addAction(pasteAct);
    editTB->addSeparator();
    editTB->addAction(zoominAct);
    editTB->addAction(zoomoutAct);

//window tool bar
    QToolBar *windowTB = addToolBar("&Window");
    windowTB->setToolButtonStyle(Qt::ToolButtonIconOnly);
    windowTB->addAction(cascadeAct);
    windowTB->addAction(tileAct);

//format tool bar

    // Font combo box, fontsize spin box in format toolbar
    fontComboBox = new QFontComboBox(this);
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), NULL, SLOT(setCurrentFont(QFont)));

    sizeSpinBox = new QDoubleSpinBox(this);
    connect(sizeSpinBox, SIGNAL(valueChanged(double)), NULL, SLOT(setFontPointSize(qreal)));
    sizeSpinBox->setMinimumWidth(100);

    addToolBarBreak();  //다음 툴바는 아래 줄로

    //format tool bar
    QToolBar *formatTB = addToolBar("&Format");
    formatTB->setToolButtonStyle(Qt::ToolButtonIconOnly);
    formatTB->addAction(fontAction);
    formatTB->addAction(colorAction);
    formatTB->addSeparator();
    formatTB->addWidget(fontComboBox);
    formatTB->addWidget(sizeSpinBox);
    formatTB->addSeparator();
    formatTB->addAction(alignLeftAct);
    formatTB->addAction(alignCenterAct);
    formatTB->addAction(alignRightAct);
    formatTB->addAction(alignJustifyAct);

//toolbar toggle을 위한 메뉴
    QMenu *toolbarMenu = windowMenu->addMenu("&Toolbar");
    toolbarMenu->setIcon(QIcon(":/icons/toolbar.png"));
    toolbarMenu->addAction(fileToolBar->toggleViewAction());
    toolbarMenu->addAction(editTB->toggleViewAction());
    toolbarMenu->addAction(formatTB->toggleViewAction());
    toolbarMenu->addAction(windowTB->toggleViewAction());
    toolbarMenu->addAction(dock->toggleViewAction());
    windowMenu->addSeparator();


// QStatusBar
    QStatusBar *statusbar = statusBar();
    QLabel *statusLabel = new QLabel("Qt Editor", statusbar);
    statusLabel->setObjectName("StatusLabel");
    statusbar->addPermanentWidget(statusLabel);
    statusbar->showMessage("started", 1500);

    // Text Edit - on MDI Area
    QTextEdit *te = newFile();

}

QTextEdit* QtEditor::newFile() {
    qDebug("Make New File");

    QAction* newfileAct = makeAction("document.png", tr("&New File"), "", tr("New File"), this, SLOT(selectWindow()));
    windowMenu->addAction(newfileAct);

    QTextEdit *textedit = new QTextEdit;

    connect(textedit, SIGNAL(cursorPositionChanged()), SLOT(setFontWidget()));
    connect(newfileAct, SIGNAL(triggered()), textedit, SLOT(setFocus()));

    connect(textedit, SIGNAL(destroyed(QObject*)), textedit, SLOT(deleteLater()));
    connect(textedit, SIGNAL(destroyed(QObject*)), newfileAct, SLOT(deleteLater()));

    mdiArea->addSubWindow(textedit);
    textedit->show();

    windowHash[newfileAct] = textedit;

    return textedit;
}

void QtEditor::openFile() {
    qDebug("Open File");
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file to open"), QDir::home().dirName(), "Text File (*.txt *.html *.c *.cpp *.h)");
    qDebug() << filename;
}

void QtEditor::saveFile() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Select file to save"), ".", "Text File (*.txt *.html *.c *.cpp *.h)");
    qDebug() << filename;

    QTextEdit* te = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    if (te != nullptr)
        windowHash.key(te)->setText(filename);
}

void QtEditor::saveAsFile() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Select file to save as"), ".", "Text File (*.txt *.html *.c *.cpp *.h)");
    qDebug() << filename;
}

void QtEditor::print() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        QTextEdit* te = (QTextEdit*)mdiArea->currentSubWindow()->widget();
        te->print(&printer);
    }
}

void QtEditor::selectColor() {
    QTextEdit* te = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QColor color = QColorDialog::getColor(te->textColor(), this);
    if (color.isValid())
        te->setTextColor(color);
}

void QtEditor::selectFont() {
    bool ok;
    QTextEdit* te = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QFont font = QFontDialog::getFont(&ok, te->currentFont(), this);
    if (ok)
        te->setCurrentFont(font);
}

void QtEditor::selectWindow() {
    QTextEdit *textedit = (QTextEdit*)windowHash[(QAction*)sender()];
    textedit->setFocus();
}

void QtEditor::alignText() {
    // 변경할 text edit 받아오기
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    QTextEdit *te = dynamic_cast<QTextEdit*>(subWindow->widget());

    // 호출한 action 받아오기
    QAction *action = qobject_cast<QAction*>(sender());

    if (action->text().contains("Left", Qt::CaseInsensitive))
        te->setAlignment(Qt::AlignLeft);
    else if (action->text().contains("Center", Qt::CaseInsensitive))
        te->setAlignment(Qt::AlignCenter);
    else if (action->text().contains("Right", Qt::CaseInsensitive))
        te->setAlignment(Qt::AlignRight);
    else if (action->text().contains("Justify", Qt::CaseInsensitive))
        te->setAlignment(Qt::AlignJustify);
    else
        ;//error;
}

void QtEditor::connectWindow(QMdiSubWindow* window) {
    if (window == nullptr)
        prevTE = nullptr;
    else {
        QTextEdit* te = qobject_cast<QTextEdit*>(window->widget());

        if (prevTE != nullptr) {
            for (QPair<QAction*, const char *> a : editActions)
                a.first->disconnect(prevTE);
        }

        prevTE = te;

        for (QPair<QAction*, const char *> a : editActions)
            connect(a.first, SIGNAL(triggered()), te, a.second);

        connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), te, SLOT(setCurrentFont(QFont)));
        connect(sizeSpinBox, SIGNAL(valueChanged(double)), te, SLOT(setFontPointSize(qreal)));
    }

    // current window가 변경 될 때마다 editaction 들을 모조리 disconnect하고 새로운 window(textedit)에 연결
}

void QtEditor::about() {
    //QMessageBox::about(this, "myQtTextEditor", "this is Qt Text Editor");
    QMessageBox::information(this, "Qt Text Editor Title", "Information Message", QMessageBox::Yes);
}

template <typename T>
QAction *QtEditor::makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot) {
    QAction *act = new QAction(text, this);
    if (icon.length())
        act->setIcon(QIcon(":/icons/" + icon));
    QKeySequence keySeq(shortCut);
    act->setShortcut(keySeq);
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered()), recv, slot);
    return act;
}

template <typename T, typename Functor>
QAction *QtEditor::makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor lambda) {
    QAction *act = new QAction(text, this);
    if (icon.length())
        act->setIcon(QIcon(":/icons/" + icon));
    QKeySequence keySeq(shortCut);
    act->setShortcut(keySeq);
    act->setStatusTip(toolTip);
    connect(act, &QAction::triggered, this, lambda);
    return act;
}

QtEditor::~QtEditor() {}
