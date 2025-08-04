#ifndef SAVESERIALDATA_H
#define SAVESERIALDATA_H


#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>

//任务执行类
class SaveSerialData : public QObject
{
    Q_OBJECT
public:
    explicit SaveSerialData(QObject *parent = nullptr);

public:
    typedef struct TimeStamp
    {
        uint32_t YearMonthDay;
        uint32_t HourMinuteSecond;
        uint16_t MSecond;
    } TimeStamp_t;

    typedef union TimeStamp_Buf
    {
        TimeStamp_t TimeStamp;
        uint8_t bytes[10] = {0};
    }TimeStamp_Buf_t;
public:

private:
    QFile *file;
    bool IsNeedTime;
    TimeStamp_Buf_t TimeStamp;

public:

    void Open_File(QString SaveSerialData_FileName_Path, bool state);   // 打开文件，以及是否需要增加时间戳
    void close_File();      // 关闭文件
    void SaveData_File(const QByteArray &info); // 保存数据

private:

public slots:

signals:

};
#endif // SAVESERIALDATA_H
