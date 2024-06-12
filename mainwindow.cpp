#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->setDigitCount(15);
    ui->lcdNumber->display(QString("00:00:00.000"));
    timer = new QTimer();
    timer->setInterval(1000);



    connect(ui->bt_reset,&QPushButton::clicked,this, &MainWindow::time_reset);
    connect(ui->bt_start,&QPushButton::clicked,this, &MainWindow::time_start);
    connect(ui->bt_pause,&QPushButton::clicked,this, &MainWindow::time_pause);
    connect(ui->bt_counts,&QPushButton::clicked,this, &MainWindow::time_counts);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimeout);

    check_button_status();
    ui->tableWidget->setColumnCount(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::time_reset()
{

    qDebug()<<QString::fromLocal8Bit ("timer reset")<<endl;
    timer->stop();
    clock_time = 0;
    pause_time = 0;
    pause_time_tmp = 0;
    pause_flag = true;
    timer_status_run = false;
    ui->lcdNumber->display(QString("00:00:00.000"));
    check_button_status();
}

void MainWindow::time_start()
{

    qDebug()<<QString::fromLocal8Bit ("timer start")<<endl;
    clock_time = 0;
    pause_time = 0;
    pause_time_tmp = 0;
    pause_flag = true;
    timer_status_run = true;
    elapsedTimer.restart();
    timer->start(1);
    check_button_status();
}

void MainWindow::time_counts()
{
    qDebug()<<QString::fromLocal8Bit ("timer counts")<<endl;
    QTime difftime = QTime(0,0,0).addMSecs(clock_time-pause_time);
    qDebug()<<difftime.toString("hh:mm:ss.zzz")<<endl;
    ui->tableWidget->insertRow(0);
    QTableWidgetItem *it = new QTableWidgetItem(difftime.toString("hh:mm:ss.zzz"));
    ui->tableWidget->setItem(0, 1, it);
}

void MainWindow::time_pause()
{

    if(pause_flag)
    {
        qDebug()<<QString::fromLocal8Bit ("timer pause")<<endl;
        timer->stop();
        pause_time_tmp = elapsedTimer.elapsed();
    }else{
        timer->start(100);
        pause_time_tmp = elapsedTimer.elapsed() - pause_time_tmp;
        pause_time += pause_time_tmp;
        qDebug()<<QString::fromLocal8Bit ("timer pause")<<pause_time<<endl;
    }
    pause_flag = !pause_flag;
    check_button_status();
}

void MainWindow::onTimeout()
{
    clock_time = elapsedTimer.elapsed();
    QTime difftime = QTime(0,0,0).addMSecs(clock_time-pause_time);
    ui->lcdNumber->display(difftime.toString("hh:mm:ss.zzz"));
}

void MainWindow::check_button_status()
{
    ui->bt_start->setDisabled(timer_status_run);
    ui->bt_pause->setDisabled(!timer_status_run);
    ui->bt_reset->setDisabled(!timer_status_run);
    ui->bt_counts->setDisabled(!timer_status_run);
}

