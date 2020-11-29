#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H
#include <QWidget>
#include<QDialog>
class QGroupBox;

namespace Ui {
class ChildWindow;
}

class ChildWindow : public QDialog
{
    Q_OBJECT

public:
    ChildWindow(QWidget *parent = 0);
    ChildWindow();

/*private:
    Ui::MainWindow *ui;*/
private slots:
    void installsoftware();

private:
    void createButtonGroup();

    QGroupBox *ButtonGroup;
    QPushButton *install;
    QPushButton *cancel;


};

#endif // CHILDWINDOW_H
