#include "qteditor.h"

#include <QTextEdit>
#include <QMenuBar>
#include <QToolBar>

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
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    //QMenu *alignMenu = fileMenu->addMenu("&Align");
    //QTextEdit::setAlignment(Qt::Alignment a);

    QMenu *editMenu = mb->addMenu("&Edit");

    QMenu *formatMenu = mb->addMenu("For&mat");

    QMenu *windowMenu = mb->addMenu("&Window");

    QMenu *helpMenu = mb->addMenu("&Help");


    /////////////////////////////////////
    /// \brief ToolBar

    QToolBar *fileToolBar = addToolBar("&File");
    fileToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    fileToolBar->addAction(newAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(openAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(saveasAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitAct);

    QToolBar *editTB = addToolBar("&Edit");
    editTB->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QToolBar *formatTB = addToolBar("&Format");
    formatTB->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QToolBar *windowTB = addToolBar("&Window");
    windowTB->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //toolbar toggle을 위한 메뉴
    QMenu *toolbarMenu = windowMenu->addMenu("&Toolbar");
    toolbarMenu->addAction(fileToolBar->toggleViewAction());
    toolbarMenu->addAction(editTB->toggleViewAction());
    toolbarMenu->addAction(formatTB->toggleViewAction());
    toolbarMenu->addAction(windowTB->toggleViewAction());


    // editTB->addAction(/**/);

}

void QtEditor::newFile() {
    qDebug("Make New File");
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
