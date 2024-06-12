#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>
#include <QDebug>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void time_reset();
    void time_start();
    void time_counts();
    void time_pause();
    void onTimeout();
    void check_button_status();



private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QElapsedTimer elapsedTimer;
    bool timer_status_run = false;
    int timer_id = 0;
    int clock_time = 0;
    int pause_time = 0;
    bool pause_flag = true;
    unsigned int pause_time_tmp = 0;

};
#endif // MAINWINDOW_H
