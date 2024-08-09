#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>

class QTextEdit;
class QMdiArea;
class QMdiSubWindow;

class QtEditor : public QMainWindow
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();

    //QTextEdit *te;
    QMdiArea *mdiArea;
    QVector<std::pair<QAction*, const char *>> editActions;
    QTextEdit *prevTE = nullptr;

public slots:
    void openFile();
    void alignText();
    QTextEdit *newFile();
    void connectWindow(QMdiSubWindow* window);

private:
    template <typename T>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot);

    template <typename T, typename Functor>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor slot);

};
#endif // QTEDITOR_H
