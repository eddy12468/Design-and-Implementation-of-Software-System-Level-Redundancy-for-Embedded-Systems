#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "childwindow.h"
int whichinstall;

/*MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
*/

MainWindow::MainWindow()
{
    //delete ui;
    //createActions();
    createMenu();
    createFormGroupBox();


    QWidget *widget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    btnlayout = new QHBoxLayout();
    QPushButton *start = new QPushButton(tr("start"));
    QPushButton *exit = new QPushButton(tr("exit"));

    connect(start,SIGNAL(clicked()),this,SLOT(startprogram()));
    connect(exit,SIGNAL(clicked()),this,SLOT(close()));

    btnlayout->addStretch();
    btnlayout->addWidget(start);
    btnlayout->addWidget(exit);

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addStretch();
    mainLayout->addLayout(btnlayout);
    widget->setLayout(mainLayout);


    setCentralWidget(widget);

    this->setGeometry(100,100,500,400);
}

void MainWindow::createMenu(){
    menuBar = new QMenuBar;
    installMenu = new QMenu(tr("&install"), this);
    configMenu = new QMenu(tr("&configuration"),this);
    installglusterfs = installMenu->addAction(tr("GlusterFS"));
    installdmtcp = installMenu->addAction(tr("DMTCP"));
    installwatchdog = installMenu->addAction(tr("Watchdog"));
    configIP = configMenu->addAction(tr("Static IP"));
    configlusterfs = configMenu->addAction(tr("GlusterFS"));

    connect(installglusterfs,SIGNAL(triggered()),this,SLOT(showglusterwindow()));
    connect(installdmtcp,SIGNAL(triggered()),this,SLOT(showdmtcpwindow()));
    connect(installwatchdog,SIGNAL(triggered()),this,SLOT(showwatchdogwindow()));
    connect(configIP,SIGNAL(triggered()),this,SLOT(inputip()));
    connect(configlusterfs,SIGNAL(triggered()),this,SLOT(configluster()));
    menuBar->addMenu(installMenu);
    menuBar->addMenu(configMenu);
}

void MainWindow::createFormGroupBox()
{
    formGroupBox = new QGroupBox(tr("Related information"));
    formGroupBox->setStyleSheet("QGroupBox { font: bold; border: 1px solid silver; border-radius: 6px; margin-top: 6px;}"
                                "QGroupBox::title { subcontrol-origin: margin; left: 7px; padding: 0px 10px 0px 10px;}");
    QFormLayout *layout = new QFormLayout;
    //password = new QLineEdit;
    //password->setEchoMode(QLineEdit::Password);
    //password->setClearButtonEnabled(true);

    host =new QComboBox;
    host->setEditable(false);
    host->insertItem(0,tr("Primary system"));
    host->insertItem(1,tr("Redundant system"));

    type = new QComboBox;
    type->setEditable(false);
    type->insertItem(0,tr("Mode 1"));
    type->insertItem(1,tr("Mode 2"));

    //QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *ip1 =new QLabel(tr("Primary System IP:"));
    //QLabel *port1 = new QLabel(tr("Port:"));
    LineEdit3 = new QLineEdit;
    //QLineEdit *LineEdit4 = new QLineEdit;

    QRegExp ipExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipExp,this);
    LineEdit3->setValidator(ipValidator);
    LineEdit3->setClearButtonEnabled(true);
    //LineEdit3->setInputMask("000.000.000.000;");

    //QRegExp portExp("/^[1-9]$|(^[1-9][0-9]$)|(^[1-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9]$)|(^[1-6][0-5][0-5][0-3][0-5]$)/");
    //QRegExpValidator *portValidator = new QRegExpValidator(portExp,this);
    //LineEdit4->setValidator(portValidator);
    //LineEdit4->setClearButtonEnabled(true);

    //LineEdit4 ->setFixedWidth(80);
    //hbox->addWidget(ip1);
    //hbox->addWidget(LineEdit3);
    //hbox->addWidget(port1);
    //hbox->addWidget(LineEdit4);
    //hbox->addStretch();


    //QHBoxLayout *hbox2 = new QHBoxLayout;
    QLabel *ip2 =new QLabel(tr("Redundant System IP:"));
    //QLabel *port2 = new QLabel(tr("Port:"));
    LineEdit5 = new QLineEdit;
    //QLineEdit *LineEdit6 = new QLineEdit;
    LineEdit5->setValidator(ipValidator);
    LineEdit5->setClearButtonEnabled(true);
    //LineEdit6->setValidator(portValidator);
    //LineEdit6->setClearButtonEnabled(true);
    //LineEdit6 ->setFixedWidth(80);

    //hbox2->addWidget(ip2);
    //hbox2->addWidget(LineEdit5);
    //hbox2->addWidget(port2);
    //hbox2->addWidget(LineEdit6);


    QHBoxLayout *hbox3 = new QHBoxLayout;
    QLabel *programpath = new QLabel(tr("Target program path: "));
    path = new QLineEdit;
    QPushButton *open = new QPushButton(this);
    path->setClearButtonEnabled(true);
    open->setIcon(QIcon(":/open.png"));
    connect(open,SIGNAL(clicked()),this,SLOT(showFile()));

    hbox3->addWidget(programpath);
    hbox3->addWidget(path);
    hbox3->addWidget(open);

    time = new QLineEdit;
    time->setFixedWidth(80);
    QRegExp posintegerExp("^[0-9]*[1-9][0-9]*$");
    QRegExpValidator *posintegerValidator = new QRegExpValidator(posintegerExp,this);
    time->setValidator(posintegerValidator);

    layout->setSpacing(10);
    layout->addRow(new QLabel(tr("Host: ")), host);
    //layout->addRow(new QLabel(tr("Root password: ")), password);
    //layout->addRow(hbox);
    layout->addRow(ip1,LineEdit3);
    //layout->addRow(hbox2);
    layout->addRow(ip2,LineEdit5);
    layout->addRow(new QLabel(tr("Program type: ")),type);
    layout->addRow(hbox3);
    layout->addRow(new QLabel(tr("Checkpoint time(s):")),time);
    formGroupBox->setLayout(layout);
}

void MainWindow::showFile(){

    QString s = QFileDialog::getOpenFileName(this,tr("開啟檔案"),"/","files(*)");
    path->setText(s);
}

void MainWindow::showglusterwindow(){
    whichinstall = 1;
    ChildWindow *cdlg = new ChildWindow(this);
    cdlg->setModal(false);
    cdlg->show();

}

void MainWindow::showdmtcpwindow(){
    whichinstall = 2;
    ChildWindow *cdlg = new ChildWindow(this);
    cdlg->setModal(false);
    cdlg->show();
}

void MainWindow::showwatchdogwindow(){
    whichinstall = 3;
    ChildWindow *cdlg = new ChildWindow(this);
    cdlg->setModal(false);
    cdlg->show();
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this,tr("Last Hint"), QString(tr("Are you sure you want to quit?")),
                                    QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::No){
        event->ignore();
    }
    else if(button == QMessageBox::Yes){
        event->accept();;
    }
}


void MainWindow::configluster(){

    if(LineEdit3->text()==""||LineEdit5->text()==""){
         QMessageBox::warning(this,tr("Warning"), QString(tr("Please input primary and redundant system ip.")),QMessageBox::Ok);
    }
    else {

    system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/GlusterFS_configuration.sh ");
    int hostindex = host->currentIndex()+1;
    QMessageBox::StandardButton button;
    if(hostindex == 1){
        button = QMessageBox::question(this,tr("Last Hint"), QString(tr("Did you install gluster on redundant system ?")),
                                    QMessageBox::Yes|QMessageBox::No);
    }
    else {
        button = QMessageBox::question(this,tr("Last Hint"), QString(tr("Did you install gluster on primary system ?")),
                                    QMessageBox::Yes|QMessageBox::No);
    }
    if(button == QMessageBox::Yes){

        QString k = "lxterminal -e /bin/bash -c '~/build-Redundant_system_UI_v2-Desktop-Release/GlusterFS_configuration.sh ";
        QString s = QString::number(hostindex);
        k=k+s+" "+LineEdit3->text()+" "+LineEdit5->text()+";read'";
        system(qPrintable(k));
        }

    }
}

void MainWindow::inputip(){

    system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/static_ip_configuration.sh ");
    QDialog *dlg = new QDialog();
    QFormLayout *flayout = new QFormLayout();
    edit1 = new QLineEdit();
    edit2 = new QLineEdit();
    edit3 = new QLineEdit();
    edit4 = new QLineEdit();
    edit5 = new QLineEdit();
    edit6 = new QLineEdit();
    QHBoxLayout *hbox = new QHBoxLayout();
    QPushButton *confirm = new QPushButton(tr("confirm"));
    QPushButton *cancel = new QPushButton(tr("cancel"));
    //button->addButton(tr("confirm"),QDialogButtonBox::YesRole);
    //button->addButton(tr("cancel"),QDialogButtonBox::NoRole);

    hbox->addStretch();
    hbox->addWidget(confirm);
    hbox->addWidget(cancel);

    QRegExp ipExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipExp,this);
    edit1->setValidator(ipValidator);
    edit1->setClearButtonEnabled(true);
    edit2->setValidator(ipValidator);
    edit2->setClearButtonEnabled(true);
    edit3->setValidator(ipValidator);
    edit3->setClearButtonEnabled(true);
    edit4->setValidator(ipValidator);
    edit4->setClearButtonEnabled(true);
    edit5->setClearButtonEnabled(true);
    edit6->setClearButtonEnabled(true);


    flayout->addRow(new QLabel(tr("IP: ")), edit1);
    flayout->addRow(new QLabel(tr("Gateway: ")),edit2);
    flayout->addRow(new QLabel(tr("Netmask: ")),edit3);
    flayout->addRow(new QLabel(tr("Broadcast: ")),edit4);
    flayout->addRow(new QLabel(tr("wpa-ssid: ")),edit5);
    flayout->addRow(new QLabel(tr("wpa-psk: ")),edit6);
    flayout->addRow(hbox);

    dlg->resize(300,100);
    dlg->setWindowTitle(tr("Setting static ip..."));
    dlg->setLayout(flayout);

    connect(confirm,SIGNAL(clicked()),this,SLOT(configip()));
    connect(cancel,SIGNAL(clicked()),dlg,SLOT(reject()));

    //dlg->show();

    dlg->setModal(false);
    dlg->show();

}
void MainWindow::configip(){

    QString s = "Your setting are: \n IP: "+edit1->text()+"\n Gateway: "+edit2->text()+"\n Natmask: "+edit3->text()+
                "\n Broadcast: "+edit4->text()+"\n wpa-ssid: "+edit5->text()+"\n wpa-psk: "+edit6->text();
    QMessageBox::StandardButton button = QMessageBox::question(this,tr("Last Hint"), QString(s),
                                    QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){
        QString k = "lxterminal -e /bin/bash -c '~/build-Redundant_system_UI_v2-Desktop-Release/static_ip_configuration.sh ";
        QString s2 = edit1->text()+" "+edit2->text()+" "+edit3->text()+" "+edit4->text()+" "+edit5->text()+" "+edit6->text();
        k=k+s2+";read'";
        qDebug() << k;
        system(qPrintable(k));

    }

}

void MainWindow::startprogram(){

    int hostindex = host->currentIndex()+1;
    int typeindex = type->currentIndex()+1;
    QString k;
    QString s;
    QString w;
    QString dirpath = QCoreApplication::applicationDirPath();

    system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/libdmtcp_mv_ckpt_to_gluster.so ");
    system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/mv_ckpt_from_gluster.sh ");

    //qDebug() << hostindex;
    if(hostindex == 1){
        system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/primary_system ");

        if(LineEdit5->text()==""||path->text()==""){

            QMessageBox::warning(this,tr("Warning"), QString(tr("Please input redundant system ip and target programpath.")),
                                            QMessageBox::Ok);
        }
        else{

            s = QString::number(typeindex)+" "+LineEdit5->text()+" "+path->text()+" "+time->text();
            w="sudo sed -i '/@lxterminal -e/c @lxterminal -e "+dirpath+"/primary_system ";
            w=w+s+"' ~/.config/lxsession/LXDE-pi/autostart";
            qDebug() << w;
            system(qPrintable(w));

            k = "lxterminal -e /bin/bash -c '~/build-Redundant_system_UI_v2-Desktop-Release/primary_system ";
            k=k+s+";read'";
            qDebug() << k;
            system(qPrintable(k));
            system("~/build-Redundant_system_UI_v2-Desktop-Release/mv");


        }
    }
    else{
        system("sudo chmod 777 ~/build-Redundant_system_UI_v2-Desktop-Release/redundant_system ");
        if(typeindex==1 && path->text()==""){
            QMessageBox::warning(this,tr("Warning"), QString(tr("Please input target programpath.")),
                                            QMessageBox::Ok);
        }
        else{

            s = QString::number(typeindex)+" "+path->text();
            w="sudo sed -i '/@lxterminal -e/c @lxterminal -e "+dirpath+"/redundant_system ";
            w=w+s+"' ~/.config/lxsession/LXDE-pi/autostart";
            qDebug() << w;
            system(qPrintable(w));

            k = "lxterminal -e /bin/bash -c '~/build-Redundant_system_UI_v2-Desktop-Release/redundant_system ";
            k=k+s+";read'";
            qDebug() << k;
            system(qPrintable(k));
        }

    }

}



