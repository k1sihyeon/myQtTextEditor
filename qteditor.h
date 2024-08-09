#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>
#include <QHash>

class QTextEdit;
class QMdiArea;
class QMdiSubWindow;
class QFontComboBox;
class QDoubleSpinBox;

class QtEditor : public QMainWindow
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();

    //QTextEdit *te;
    QMdiArea *mdiArea;
    QVector<QPair<QAction*, const char *>> editActions;
    QTextEdit *prevTE = nullptr;
    QFontComboBox *fontComboBox;
    QDoubleSpinBox *sizeSpinBox;
    QMenu *windowMenu;
    QHash<QAction*, QWidget*> windowHash;

public slots:
    void openFile();
    void saveFile();
    void saveAsFile();
    void print();
    void alignText();
    void selectColor();
    void selectFont();
    void selectWindow();
    QTextEdit *newFile();
    void connectWindow(QMdiSubWindow* window);
    void about();

private:
    template <typename T>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject* recv, const char* slot);

    template <typename T, typename Functor>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, Functor slot);

};
#endif // QTEDITOR_H
