#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>

class QTextEdit;

class QtEditor : public QMainWindow
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();

    QTextEdit *te;

public slots:
    void newFile();
    void openFile();
    void alignText();

private:
    template <typename T>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot);

    template <typename T, typename Functor>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor slot);

};
#endif // QTEDITOR_H
