#include "SerialDataSave.h"



SaveSerialData::SaveSerialData(QObject *parent) : QObject(parent)
{

}

void SaveSerialData::Open_File(QString SaveSerialData_FileName_Path, bool state)    // 打开文件，以及是否需要增加时间戳
{
    file = new QFile();
    file->setFileName(SaveSerialData_FileName_Path);
    if (!file->open(QIODevice::Append))
    {
        qDebug() << "open file error";
        return;
    }
    IsNeedTime = state;
}

void SaveSerialData::close_File()   // 关闭文件
{
    file->close();
}

void SaveSerialData::SaveData_File(const QByteArray &data)  // 保存数据
{
    QByteArray data_temp = data;
    if(IsNeedTime)
    {
        QString TimeStamp_temp = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        TimeStamp.TimeStamp.YearMonthDay = TimeStamp_temp.mid(0,8).toUInt();
        TimeStamp.TimeStamp.HourMinuteSecond = TimeStamp_temp.mid(8,6).toUInt();
        TimeStamp.TimeStamp.MSecond = TimeStamp_temp.mid(14,3).toUInt();

        QByteArray data_temp((const char*)TimeStamp.bytes, 10);
        data_temp.append(data_temp);
    }
    QDataStream out(file);   // we will serialize the data into the file

    if(file->isOpen())
    {
      if(!file->write(data_temp.data(),data_temp.size()) ||  !file->flush())
      {
          file->close(); // 可以考虑不要
      }
    }
}

void SaveSerialData::Save_String_File(QString data)  // 保存数据
{
    QString flie_String;
    if(IsNeedTime)
    {
        QString TimeStamp_temp = QDateTime::currentDateTime().toString("yyyy:MM:dd:hh:mm:ss:zzz");
        flie_String.append(TimeStamp_temp);
    }
    flie_String.append(data);
    flie_String.append('\n');
    QTextStream stream(file);
    stream << flie_String;
}
