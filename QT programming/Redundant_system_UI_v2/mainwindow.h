#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "childwindow.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QDialog>

class QAction;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QLineEdit;
class QComboBox;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    explicit MainWindow(QWidget *parent = 0);
    MainWindow();



/*private:
    Ui::MainWindow *ui;*/
private slots:
    void showFile();
    void showglusterwindow();
    void showdmtcpwindow();
    void showwatchdogwindow();
    void closeEvent(QCloseEvent *event);
    void configluster();
    void configip();
    void inputip();
    void startprogram();



private:
    void createMenu();
    void createFormGroupBox();




     QMenuBar *menuBar;
     QVBoxLayout *vbox;
     QGroupBox *formGroupBox;
     QHBoxLayout *btnlayout;
     QMenu *installMenu;
     QMenu *configMenu;
     QAction *installglusterfs;
     QAction *installdmtcp;
     QAction *installwatchdog;
     QAction *configlusterfs;
     QAction *configIP;
     QLineEdit *path;
     QComboBox *host;
     QComboBox *type;
     QLineEdit *LineEdit3;
     QLineEdit *LineEdit5;
     QLineEdit *edit1;
     QLineEdit *edit2;
     QLineEdit *edit3;
     QLineEdit *edit4;
     QLineEdit *edit5;
     QLineEdit *edit6;
     QLineEdit *time;
     //QLineEdit *password;





};

#endif // MAINWINDOW_H
