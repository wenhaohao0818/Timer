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
    QStringList headers = {QString("计次"),QString("时间"),QString("差值")};
    ui->tableWidget->setColumnCount(headers.length());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(0);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
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
    ranking = 0;
    lastCountTime = 0;
    pause_flag = true;
    timer_status_run = false;
    ui->lcdNumber->display(QString("00:00:00.000"));
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    QStringList headers = {QString("计次"),QString("时间"),QString("差值")};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    ranking++;
    QTime nowtime = QTime(0,0,0).addMSecs(clock_time-pause_time);
    qDebug()<<nowtime.toString("hh:mm:ss.zzz")<<endl;
    ui->tableWidget->insertRow(0);
    QTableWidgetItem *it = new QTableWidgetItem(nowtime.toString("hh:mm:ss.zzz"));
    QTime difftime = QTime(0,0,0).addMSecs(-nowtime.msecsTo(QTime(0,0,0)) - lastCountTime);
    lastCountTime = -nowtime.msecsTo(QTime(0,0,0));
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(ranking)));
    ui->tableWidget->setItem(0, 1, it);
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(difftime.toString("+hh:mm:ss.zzz")));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    for (int i=0;i<ui->tableWidget->rowCount();i++)
       {
           for (int j=0;j<ui->tableWidget->columnCount();j++)
           {
               ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
           }
       }
}

void MainWindow::time_pause()
{

    if(pause_flag)
    {
        qDebug()<<QString::fromLocal8Bit ("timer pause")<<endl;
        timer->stop();
        pause_time_tmp = elapsedTimer.elapsed();
    }else{
        timer->start(10);
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

