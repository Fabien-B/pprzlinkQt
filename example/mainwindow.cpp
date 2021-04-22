#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pprzlinkqt.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->label_button, &QPushButton::clicked, this, [=]() {
        //vbox->removeWidget(label);
        ui->label->deleteLater();
    });


    auto pp = new PprzlinkQt("/home/fabien/paparazzi/var/messages.xml", "testPprzLinkQt", "testPprzLinkQt ready");
    pp->start("127.255.255.255", 2010);
    
    pp->bind("FLIGHT_PARAM", ui->label, [=](QString sender, Message msg) {
        QString ac_id;
        float alt;
        msg.getField("ac_id", ac_id);
        msg.getField("alt", alt);
        //qDebug() << ac_id << alt;
        ui->label->setText(QString("%1 alt: %2 m").arg(ac_id, alt));

    });

    //connect(ui->line_edit, &QLineEdit::returnPressed, this, [=]() {
    //    ivyqt->send(ui->line_edit->text());
    //});
}

MainWindow::~MainWindow()
{
    delete ui;
}
