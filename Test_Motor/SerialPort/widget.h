#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QObject>
#include "serialdatasave.h"
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void Read_Date();       //读取串口数据
    void find_port();       //查找可用串口
    void sleep(int msec);      //延时函数

    void on_open_port_clicked();
    void on_close_port_clicked();

    void on_JZ_clicked();
    void on_HL_clicked();
    void on_YX_clicked();
    void Time_Out();
    void on_ZDSC_clicked();

    void on_Time_com_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void Start_SaveSerialData();     // 开始保存数据
    void Stop_SaveSerialData();      // 停止保存数据

    void on_KSBC_clicked();

private:
    QString SaveSerialData_FileName_Path = "";

    SaveSerialData *saveserialdata; // 串口数据保存对象
    Ui::Widget *ui;
    QSerialPort *serialport;
    QTimer *_1s_Timer;
    bool textstate_receive;
    bool textstate_send;
};

#endif // WIDGET_H
