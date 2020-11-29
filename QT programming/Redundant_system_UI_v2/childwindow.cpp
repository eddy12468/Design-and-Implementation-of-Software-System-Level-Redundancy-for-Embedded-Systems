    #include "childwindow.h"
    #include "mainwindow.h"
    #include <QtWidgets>
    extern int whichinstall;

    ChildWindow::ChildWindow(QWidget *parent) : QDialog(parent){
        //ui(new Ui::MainWindow)

        system("sudo chmod 777 ~/build-Redundant_System_UI-Desktop-Release/install_software.sh");

        setGeometry(200,200,300,100);
        createButtonGroup();

        QLabel *label1;

        switch(whichinstall){

            case 1:
                label1 = new QLabel(tr("Do you want to install GlusterFS?"));
                setWindowTitle(tr("install GlusterFS"));
                //connect(install,SIGNAL(triggered()),this,SLOT(installsoftware()));
                break;
            case 2:
                label1 = new QLabel(tr("Do you want to install DMTCP?"));
                setWindowTitle(tr("install DMTCP"));
                //connect(install,SIGNAL(triggered()),this,SLOT(installsoftware()));
                break;
            case 3:
                label1 = new QLabel(tr("Do you want to install Watchdog?"));
                setWindowTitle(tr("install Watchdog"));
                //connect(install,SIGNAL(triggered()),this,SLOT(installsoftware()));
                break;
            default:
                break;
        }

       /* if(whichinstall==1)
            label1 = new QLabel(tr("Do you want to install GlusterFS?"));
        else
            label1 = new QLabel(tr("Do you want to install DMTCP?"));
    */
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(label1);
        mainLayout->addWidget(ButtonGroup);
        this->setLayout(mainLayout);

       /* install = new QPushButton(tr("install"),this);
        cancel = new QPushButton(tr("cancel"),this);
        */

        /*QHBoxLayout *mainlayout = new QHBoxLayout;
        mainlayout->addWidget(install);
        mainlayout->addWidget(cancel);*/

       /* QWidget *widget1 = new QWidget ();
        widget1->setLayout(mainlayout);
        setCentralWidget(widget1);
    */
    }

    void ChildWindow::createButtonGroup(){

        ButtonGroup = new QGroupBox();

        QHBoxLayout *hbox = new QHBoxLayout;
        install = new QPushButton(tr("install"),this);
        cancel = new QPushButton(tr("cancel"),this);
        connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
        connect(install,SIGNAL(clicked()),this,SLOT(installsoftware()));

        hbox->addStretch(); //add a placeholder, make it aligns to the right
        hbox->addWidget(install);
        hbox->addWidget(cancel);

        ButtonGroup->setLayout(hbox);
    }

    void ChildWindow::installsoftware(){

        QString k = "lxterminal -e /bin/bash -c '~/build-Redundant_system_UI_v2-Desktop-Release/install_software.sh ";
        QString s = QString::number(whichinstall);
        k=k+s+";read'";
        system(qPrintable(k));
        reject();
    }
