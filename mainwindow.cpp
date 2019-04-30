#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "integrator.h"
#include <QFileDialog>

QString filename;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Multi Domain Integral Calculator");
    QPixmap bkgnd("/home/samanoudy/numerical_project/back2.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
   //setWindowOpacity(qreal(40)/100);
    this->setPalette(palette);


   //chage some colors for labels and stuff
   palette = ui->label_3->palette();
   palette.setColor(ui->label_3->foregroundRole(), Qt::blue);
   ui->label_3->setPalette(palette);
   //chage some colors for labels and stuff
   palette = ui->result_browser->palette();
   palette.setColor(ui->result_browser->foregroundRole(), Qt::blue);
   ui->result_browser->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//calculate integral from function string button
void MainWindow::on_pushButton_clicked()
{
    Integrator integ;
    QString functionstr = ui->functiontext->toPlainText();
    std::string function_str = functionstr.toUtf8().constData();
    QString limits = ui->limitsText->toPlainText();
    std::string limits_str= limits.toUtf8().constData();
    std::string result= integ.getResultByFunction(function_str,limits_str); //send to backend and get back the result
    QString resut_ui = QString::fromStdString(result);
    ui->result_browser->setText(resut_ui);
}

void MainWindow::on_pushButton_2_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Import Text File"), "/home", tr("TXT Files (*.txt)"));
}

void MainWindow::on_pushButton_3_clicked()
{
    Integrator integ;
    std::string filename_str=filename.toUtf8().constData();
    std::string result=integ.getResultByFile(filename_str);
    QString resut_ui = QString::fromStdString(result);
    ui->result_browser->setText(resut_ui);
}
