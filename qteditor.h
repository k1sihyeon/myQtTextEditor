#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>

class QtEditor : public QMainWindow
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();

public slots:
    void newFile();
    void openFile();

private:
    template <typename T>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot);

    template <typename T, typename Functor>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor slot);

};
#endif // QTEDITOR_H
