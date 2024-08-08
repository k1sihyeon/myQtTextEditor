#include "qteditor.h"

#include <QTextEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QFontComboBox>
#include <QDoubleSpinBox>
#include <QStatusBar>
#include <QLabel>

QtEditor::QtEditor(QWidget *parent)
    : QMainWindow(parent)
{
    QTextEdit *te = new QTextEdit(this);
    setCentralWidget(te);

    QMenuBar *mb = new QMenuBar(this);
    setMenuBar(mb);

    // QAction *newAct = new QAction(QIcon("new.png"), "&New", this);
    // newAct->setShortcut(tr("Ctrl+N"));
    // newAct->setStatusTip(tr("make new file"));
    // connect(newAct, SIGNAL(triggered()), SLOT(newFile()));

    //tr -> 번역을 위한 예약어

//file menu
    QAction *newAct = makeAction("new.png", tr("&New"), tr("Ctrl+N"), tr("make new file"), this, SLOT(newFile()));
    QAction *openAct = makeAction("open.png", "&Open", tr("Ctrl+O"), "open file", this, SLOT(openFile()));
    QAction *saveAct = makeAction("save.png", "&Save", tr("Ctrl+S"), "save file", this, SLOT(saveFile()));
    QAction *saveasAct = makeAction("saveas.png", "Save &As...", "", "save file as another file name", this, SLOT(saveasFile()));
    QAction *printAct = makeAction("print.png", "&Print", "Ctrl+P", "print file", this, SLOT(printFile()));
    QAction *exitAct = makeAction("quit.png", "E&xit", tr("Ctrl+Q"), "exit program", this, SLOT(exit()));

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
    QAction *undoAct = makeAction("undo.png", tr("&Undo"), "Ctrl+Z", tr("Undo work"), te, SLOT(undo()));
    QAction *redoAct = makeAction("redo.png", "&Redo", "Ctrl+Shift+Z", "Redo work", te, SLOT(redo()));
    QAction *copyAct = makeAction("copy.png", "&Copy", "Ctrl+C", "Copy text", te, SLOT(copy()));
    QAction *cutAct = makeAction("cut.png", "&Cut", "Ctrl+X", "Cut text", te, SLOT(cut()));
    QAction *pasteAct = makeAction("paste.png", "&Paste", "Ctrl+V", "Paste work", te, SLOT(paste()));
    QAction *zoominAct = makeAction("zoom_in.png", "Zoom &In", "Ctrl++", "Zoom In Screen", te, SLOT(zoomIn()));
    QAction *zoomoutAct = makeAction("zoom_out.png", "Zoom &Out", "Ctrl+-", "Zoom Out Screen", te, SLOT(zoomOut()));

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


//format menu
    QMenu *formatMenu = mb->addMenu("For&mat");

//window menu
    QMenu *windowMenu = mb->addMenu("&Window");

//help menu
    QMenu *helpMenu = mb->addMenu("&Help");


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

//format tool bar

    // Font combo box, fontsize spin box in format toolbar
    QFontComboBox *fontComboBox = new QFontComboBox(this);
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), te, SLOT(setCurrentFont(QFont)));

    QDoubleSpinBox *sizeSpinBox = new QDoubleSpinBox(this);
    connect(sizeSpinBox, SIGNAL(valueChanged(double)), te, SLOT(setFontPointSize(qreal)));

    addToolBarBreak();  //다음 툴바는 아래 줄로

    //format tool bar
    QToolBar *formatTB = addToolBar("&Format");
    formatTB->setToolButtonStyle(Qt::ToolButtonIconOnly);
    formatTB->addSeparator();
    formatTB->addWidget(fontComboBox);
    formatTB->addWidget(sizeSpinBox);

//toolbar toggle을 위한 메뉴
    QMenu *toolbarMenu = windowMenu->addMenu("&Toolbar");
    toolbarMenu->addAction(fileToolBar->toggleViewAction());
    toolbarMenu->addAction(editTB->toggleViewAction());
    toolbarMenu->addAction(formatTB->toggleViewAction());
    toolbarMenu->addAction(windowTB->toggleViewAction());


// QStatusBar
    QStatusBar *statusbar = statusBar();
    QLabel *statusLabel = new QLabel("Qt Editor", statusbar);
    statusLabel->setObjectName("StatusLabel");
    statusbar->addPermanentWidget(statusLabel);
    statusbar->showMessage("started", 1500);

}

void QtEditor::newFile() {
    qDebug("Make New File");
}

void QtEditor::openFile() {
    qDebug("Open File");
}

template <typename T>
QAction *QtEditor::makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot) {
    QAction *act = new QAction(text, this);
    if (icon.length())
        act->setIcon(QIcon("icons/" + icon));
    act->setShortcut(QString(shortCut));
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered()), recv, slot);
    return act;
}

template <typename T, typename Functor>
QAction *QtEditor::makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor lambda) {
    QAction *act = new QAction(text, this);
    if (icon.length())
        act->setIcon(QIcon("icons/" + icon));
    QKeySequence keySeq(shortCut);
    act->setShortcut(keySeq);
    act->setStatusTip(toolTip);
    connect(act, &QAction::triggered, this, lambda);
    return act;
}


QtEditor::~QtEditor() {}
