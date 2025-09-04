#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

int8_t Date[6]={0};
uint8_t QSerial_Date[128];
uint16_t CRC_Value =0;
uint32_t V_Value =0;
uint16_t I_Value =0;
uint16_t W_Value =0;
uint8_t ZT_Value =0;
uint8_t Date_Lenth =0;
uint8_t RS_485_Flasg =0;
static uint8_t ZDSC_Flag = 0;
int8_t X_Pos_Value=0;
int8_t Y_Pos_Value=0;
int8_t Z_Pos_Value=0;
uint32_t Change_Time=0;
uint32_t Change_Time_number=0;
static uint8_t Change_flag=0;
int8_t TX_T = 0;
int8_t RX_T = 0;
uint8_t project_s = 0;
QString current_String;
static uint8_t Save_Date_Flag = 0;;

#define Timer_Out 1000
#define X_MIN -20
#define X_MAX 20
#define Y_MIN -20
#define Y_MAX 20
#define Z_MIN 15
#define Z_MAX 50

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("测控平台调试上位机   Version: 1.1");
    setWindowIcon(QIcon(":123.ico"));
    serialport = new QSerialPort;
    find_port();                    //查找可用串口
    ui->close_port->setEnabled(false);
    _1s_Timer = new QTimer(this);
    connect(_1s_Timer,SIGNAL(timeout()),this,SLOT(Time_Out()));
    _1s_Timer->start(Timer_Out);
    srand(time(0));
    saveserialdata = new SaveSerialData();  // 创建串口数据保存类对象
    ui->baud->setCurrentIndex(6);
}
Widget::~Widget()
{
    delete ui,serialport;
}
/* 多字节计算 CRC-8 */
int8_t CRC8(int8_t *ptr, uint8_t len)
{
    unsigned char i;
    unsigned char crc=0x00; /* 计算的初始crc值: 0 - 输入值， 1 - 输入翻转 */

    while(len--)
    {
        crc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */
        for (i=8; i>0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x2f;
            else
                crc = (crc << 1);
        }
    }
    //reture (~crc); /* 输出翻转 */
    return (crc);
}

static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

uint16_t usCRC16( uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t         ucCRCHi = 0xFF;
    uint8_t         ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}

void Widget::on_open_port_clicked()
{
   update();
   sleep(100);      //延时100ms
    //初始化串口
        serialport->setPortName(ui->com->currentText());        //设置串口名
        if(serialport->open(QIODevice::ReadWrite))              //打开串口成功
        {
            serialport->setBaudRate(ui->baud->currentText().toInt());       //设置波特率
            switch(ui->bit->currentIndex())                   //设置数据位数
            {
                case 8:serialport->setDataBits(QSerialPort::Data8);break;
                default: break;
            }
            switch(ui->jiaoyan->currentIndex())                   //设置奇偶校验
            {
                case 0: serialport->setParity(QSerialPort::NoParity);break;
                default: break;
            }
            switch(ui->stopbit->currentIndex())                     //设置停止位
            {
                case 1: serialport->setStopBits(QSerialPort::OneStop);break;
                case 2: serialport->setStopBits(QSerialPort::TwoStop);break;
                default: break;
            }
            serialport->setFlowControl(QSerialPort::NoFlowControl);     //设置流控制
            //连接槽函数
            QObject::connect(serialport, &QSerialPort::readyRead, this, &Widget::Read_Date);
            // 设置控件可否使用
            ui->close_port->setEnabled(true);
            ui->open_port->setEnabled(false);
        }
        else    //打开失败提示
        {
            sleep(100);
            QMessageBox::information(this,tr("Erro"),tr("Open the failure"),QMessageBox::Ok);
        }
}
void Widget::on_close_port_clicked()
{
    serialport->clear();        //清空缓存区
    serialport->close();        //关闭串口
    ui->open_port->setEnabled(true);
    ui->close_port->setEnabled(false);
    ui->com->clear();
    find_port();     //重新查找com
}

void Widget::Read_Date()
{
    QByteArray buf;
    QByteArray buf_Save_Date;
    QString buf_Save_String;
    buf = serialport->readAll();
    if(!buf.isEmpty())
    {
        Date_Lenth = buf.size();
        for(int i = 0;i < Date_Lenth;i++){
            QSerial_Date[i] = static_cast<uint8_t>(buf.at(i));
        }
        CRC_Value = usCRC16(QSerial_Date,Date_Lenth - 2);
        if(CRC_Value == (QSerial_Date[Date_Lenth-2] | ((uint16_t)QSerial_Date[Date_Lenth-1])<<8)){
           I_Value = QSerial_Date[4] | ((uint16_t)QSerial_Date[3])<<8;
           V_Value = QSerial_Date[8] | ((uint32_t)QSerial_Date[7])<<8 | ((uint32_t)QSerial_Date[6])<<16 | ((uint32_t)QSerial_Date[5])<<24;
           W_Value = QSerial_Date[10] | ((uint16_t)QSerial_Date[9])<<8;
           ZT_Value = QSerial_Date[12] | ((uint16_t)QSerial_Date[11])<<8;
           TX_T = QSerial_Date[17];
           RX_T = QSerial_Date[18];
           ui->V->setText(QString::number(V_Value));
           ui->I->setText(QString::number(I_Value));
           ui->W->setText(QString::number(W_Value));
           ui->ZT->setText(QString::number(ZT_Value));
           project_s = ui->CPXZ->currentIndex();
           if(project_s == 0 || project_s == 1){
               ui->TX_Temp->setText(QString::number(TX_T));
               ui->RX_Temp->setText(QString::number(RX_T));
           }else{
               ui->TX_Temp->clear();
               ui->RX_Temp->clear();
           }
           switch (ui->CPXZ->currentIndex()) {
               case 0://300W自然散热TX端
                   switch (ZT_Value) {
                       case 0xff:
                               ui->State_Anylise->setText("待机模式！！");
                          break;
                       case 0x00:
                               ui->State_Anylise->setText("恒流充电中！！");
                          break;
                       case 0x01:
                               ui->State_Anylise->setText("恒压充电中！！");
                          break;
                       case 0x02:
                               ui->State_Anylise->setText("接受端过温！！");
                          break;
                       case 0x03:
                               ui->State_Anylise->setText("接受端过流！！");
                          break;
                       case 0x04:
                               ui->State_Anylise->setText("接受端过压！！");
                          break;
                       case 0x05:
                               ui->State_Anylise->setText("电池已充满！！");
                          break;
                       case 10:
                               ui->State_Anylise->setText("PFC电压错误！！");
                          break;
                       case 11:
                               ui->State_Anylise->setText("逆变驱动错误(ready,fault引脚报错)！！");
                          break;
                       case 15:
                               ui->State_Anylise->setText("风扇故障！！");
                          break;
                       case 17:
                               ui->State_Anylise->setText("发送端过温！！");
                          break;
                       case 22:
                               ui->State_Anylise->setText("设备温度故障！！");
                               ui->TX_Temp->setText(QString::number(999));
                          break;
                       case 0x15:
                               ui->State_Anylise->setText("设备通讯故障！！");
                          break;
                       case 24:
                               ui->State_Anylise->setText("设备通讯中断异常！！");
                          break;
                       default:
                           break;
                   }
                   break;
               case 1:
                   switch (ZT_Value) {
                       case 0xff:
                               ui->State_Anylise->setText("待机模式！！");
                          break;
                       case 0x00:
                               ui->State_Anylise->setText("恒流充电中！！");
                          break;
                       case 0x01:
                               ui->State_Anylise->setText("恒压充电中！！");
                          break;
                       case 0x02:
                               ui->State_Anylise->setText("设备过温！！");
                          break;
                       case 0x03:
                               ui->State_Anylise->setText("设备过流！！");
                          break;
                       case 0x04:
                               ui->State_Anylise->setText("设备过压！！");
                          break;
                       case 0x05:
                               ui->State_Anylise->setText("电池已充满！！");
                          break;
                       case 0x06:
                               ui->State_Anylise->setText("设备温度故障！！");
                               ui->RX_Temp->setText(QString::number(999));
                          break;
                       case 0x07:
                               ui->State_Anylise->setText("设备自检温度异常(过高or过低)！！");
                          break;
                       case 0x08:
                               ui->State_Anylise->setText("设备自检电压过低！！");
                          break;
                       case 0x09:
                               ui->State_Anylise->setText("设备自检电压过高！！");
                          break;
                       case 0x0a:
                               ui->State_Anylise->setText("设备自检电流过高！！");
                          break;
                       case 0x0b:
                               ui->State_Anylise->setText("设备通讯故障！！");
                          break;
                       default:
                           break;
                   }
                   break;
               case 2:
                   switch (ZT_Value) {
                       case 0xff:
                               ui->State_Anylise->setText("待机模式！！");
                          break;
                       case 0x00:
                               ui->State_Anylise->setText("恒流充电中！！");
                          break;
                       case 0x01:
                               ui->State_Anylise->setText("恒压充电中！！");
                          break;
                       case 0x02:
                               ui->State_Anylise->setText("充电过程中，接收端输出电压故障！！");
                          break;
                       case 0x03:
                               ui->State_Anylise->setText("恒压基准值！！");
                          break;
                       case 0x04:
                               ui->State_Anylise->setText("恒流基准值！！");
                          break;
                       case 0x05:
                               ui->State_Anylise->setText("软启动电流故障！！");
                          break;
                       case 0x06:
                               ui->State_Anylise->setText("充电过程中，接收端输出电流故障！！");
                          break;
                       case 0x07:
                               ui->State_Anylise->setText("接收端过温保护！！");
                          break;
                       case 0x08:
                               ui->State_Anylise->setText("接收端风扇运行不正常！！");
                          break;
                       case 0x09:
                               ui->State_Anylise->setText("充电过程中，接收端输入电压过低！！");
                          break;
                       case 0x0a:
                               ui->State_Anylise->setText("运行过程中，PFC电压错误！！");
                          break;
                       case 0x0b:
                               ui->State_Anylise->setText("运行过程中，逆变驱动错误！！");
                          break;
                       case 0x0c:
                               ui->State_Anylise->setText("发射端电流值突变异常！！");
                          break;
                       case 0x0d:
                               ui->State_Anylise->setText("线圈效率值报警！！");
                          break;
                       case 0x0f:
                               ui->State_Anylise->setText("发射端风扇运行不正常！！");
                          break;
                       case 0x10:
                               ui->State_Anylise->setText("充满状态！！");
                          break;
                       case 0x11:
                               ui->State_Anylise->setText("发射端过温保护！！");
                          break;
                       case 0x12:
                               ui->State_Anylise->setText("蓝牙未连接上！！");
                          break;
                       case 0x13:
                               ui->State_Anylise->setText("蓝牙连接断开！！");
                          break;
                       case 0x14:
                               ui->State_Anylise->setText("运行过程中，接收端位置未对准！！");
                          break;
                       case 0x15:
                               ui->State_Anylise->setText("初始化过程中，接收端位置未对准！！");
                          break;
                       default:
                           break;
                   }
                   break;
               case 3:
                    ui->ZT->clear();
                    ui->State_Anylise->clear();
                    ui->TX_Temp->clear();
                    ui->RX_Temp->clear();
                   break;
               case 4:
                   switch (ZT_Value) {
                       case 0xff:
                               ui->State_Anylise->setText("待机模式！！");
                          break;
                       case 0x00:
                               ui->State_Anylise->setText("恒流充电中！！");
                          break;
                       case 0x01:
                               ui->State_Anylise->setText("恒压充电中！！");
                          break;
                       case 0x02:
                               ui->State_Anylise->setText("充电过程中，接收端输出电压故障！！");
                          break;
                       case 0x03:
                               ui->State_Anylise->setText("恒压基准值！！");
                          break;
                       case 0x04:
                               ui->State_Anylise->setText("恒流基准值！！");
                          break;
                       case 0x05:
                               ui->State_Anylise->setText("软启动电流故障！！");
                          break;
                       case 0x06:
                               ui->State_Anylise->setText("充电过程中，接收端输出电流故障！！");
                          break;
                       case 0x07:
                               ui->State_Anylise->setText("接收端过温保护！！");
                          break;
                       case 0x08:
                               ui->State_Anylise->setText("接收端风扇运行不正常！！");
                          break;
                       case 0x09:
                               ui->State_Anylise->setText("充电过程中，接收端输入电压过低！！");
                          break;
                       case 0x0a:
                               ui->State_Anylise->setText("运行过程中，PFC电压错误！！");
                          break;
                       case 0x0b:
                               ui->State_Anylise->setText("运行过程中，逆变驱动错误！！");
                          break;
                       case 0x0c:
                               ui->State_Anylise->setText("发射端电流值突变异常！！");
                          break;
                       case 0x0d:
                               ui->State_Anylise->setText("线圈效率值报警！！");
                          break;
                       case 0x0f:
                               ui->State_Anylise->setText("发射端风扇运行不正常！！");
                          break;
                       case 0x10:
                               ui->State_Anylise->setText("充满状态！！");
                          break;
                       case 0x11:
                               ui->State_Anylise->setText("发射端过温保护！！");
                          break;
                       case 0x12:
                               ui->State_Anylise->setText("蓝牙未连接上！！");
                          break;
                       case 0x13:
                               ui->State_Anylise->setText("蓝牙连接断开！！");
                          break;
                       case 0x14:
                               ui->State_Anylise->setText("运行过程中，接收端位置未对准！！");
                          break;
                       case 0x15:
                               ui->State_Anylise->setText("初始化过程中，接收端位置未对准！！");
                          break;
                       default:
                           break;
                   }
                   break;
               case 5:
                   ui->ZT->clear();
                   ui->State_Anylise->clear();
                   ui->TX_Temp->clear();
                   ui->RX_Temp->clear();
                   break;
               default:
                   break;
           }
           if(Save_Date_Flag == 1){
              //buf_Save_Date.append("/*****/\r\n").append(buf).append("\r\n");
              //saveserialdata->SaveData_File(buf_Save_Date);
               buf_Save_String.append(current_String);
               buf_Save_String.append("TX温度：").append(ui->TX_Temp->text()).append("  ℃ ");
               buf_Save_String.append("RX温度：").append(ui->RX_Temp->text()).append("  ℃ ");
               buf_Save_String.append("输出电压：").append(ui->V->text()).append("  mV ");
               buf_Save_String.append("输出电流：").append(ui->I->text()).append("  mA ");
               buf_Save_String.append("输出功率：").append(ui->W->text()).append("  W ");
               buf_Save_String.append("状态：").append(ui->ZT->text()).append("  ：").append(ui->State_Anylise->text());
               saveserialdata->Save_String_File(buf_Save_String);
           }
           RS_485_Flasg = 0;
        }
    }
    buf.clear();
}


void Widget::find_port()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->com->addItem(serial.portName());
            serial.close();
        }
    }
}

void Widget::sleep( int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Widget::on_JZ_clicked()
{
    Date[0] = 0xaa;
    Date[1] = 0x00;
    Date[2] = 0x00;
    Date[3] = 0x00;
    Date[4] = 0xAA;
    Date[5] = CRC8(Date,5);
    QByteArray data(reinterpret_cast<const char*>(Date),6);
    serialport->write(data);
}

void Widget::on_HL_clicked()
{
    Date[0] = 0xaa;
    Date[1] = 0x00;
    Date[2] = 0x00;
    Date[3] = 0x00;
    Date[4] = ui->SD->currentIndex();
    Date[5] = CRC8(Date,5);
    QByteArray data(reinterpret_cast<const char*>(Date),6);
    serialport->write(data);
}

void Widget::on_YX_clicked()
{
    current_String.clear();
    Date[0] = 0xaa;
    Date[1] = ui->X->text().toInt();
    Date[2] = ui->Y->text().toInt();
    Date[3] = ui->Z->text().toInt();
    Date[4] = ui->SD->currentIndex();
    Date[5] = CRC8(Date,5);
    if(Date[1] >=X_MIN && Date[1] <= X_MAX){//X
        if(Date[2] >=Y_MIN && Date[2] <= Y_MAX){//Y
            if(Change_flag == 1){
                Change_flag = 0;
                Date[3] = 80;
                Date[5] = CRC8(Date,5);
                QByteArray data(reinterpret_cast<const char*>(Date),6);
                serialport->write(data);
                if(ui->SD->currentIndex() == 0)
                    sleep(16000);
                else if(ui->SD->currentIndex() == 1)
                    sleep(8000);
                else if(ui->SD->currentIndex() == 2)
                    sleep(4000);
                on_YX_clicked();
            }else{
                if(Date[3] >=Z_MIN && Date[3] <= Z_MAX){//Z
                    current_String.append(" 位置 X: ").append(ui->X->text()).append("  mm ");
                    current_String.append("Y: ").append(ui->Y->text()).append("  mm ");
                    current_String.append("Z: ").append(ui->Z->text()).append("  mm ");
                    QByteArray data(reinterpret_cast<const char*>(Date),6);
                    serialport->write(data);
                }else
                    QMessageBox::information(this,"Error!","Z轴超出界限范围,15~50");
            }
       }else
            QMessageBox::information(this,"Error!","Y轴超出界限范围,-20~20");
    }else
        QMessageBox::information(this,"Error!","X轴超出界限范围,-20~20");
}
static uint8_t RS485_Date[8]= {0xFF,0x03,0x00,0x00,0x00,0x08,0x11,0xD6};
void Widget::Time_Out(){
    RS485_Date[0] = ui->lineEdit->text().toInt();
    RS485_Date[7] =(uint8_t)(usCRC16(RS485_Date,6)>>8);
    RS485_Date[6] =(uint8_t)usCRC16(RS485_Date,6);
    QByteArray data(reinterpret_cast<const char*>(RS485_Date),8);
    serialport->write(data);
    RS_485_Flasg++;
    if(RS_485_Flasg > 5){
        ui->V->clear();
        ui->I->clear();
        ui->W->clear();
        ui->ZT->clear();
        ui->TX_Temp->clear();
        ui->RX_Temp->clear();
        ui->State_Anylise->clear();
        RS_485_Flasg = 5;
    }
    Change_Time_number++;
    if(ZDSC_Flag == 1){
        switch (ui->Time_com->currentIndex()) {
            case 0x00:
                Change_Time = 60;
                break;
            case 0x01:
                Change_Time = 60*2;
                break;
            case 0x02:
                Change_Time = 60*4;
                break;
            case 0x03:
                Change_Time = 60*6;
                break;
            case 0x04:
                Change_Time = 60*8;
                break;
            case 0x05:
                Change_Time = 60*10;
                break;
            case 0x06:
                Change_Time = 60*20;
                break;
            case 0x07:
                Change_Time = 60*30;
                break;
            case 0x08:
                Change_Time = 60*60;
                break;
            case 0x09:
                Change_Time = 60*120;
                break;
            default:
                break;
        }
        if(Change_Time_number % Change_Time == 0){
            Change_flag = 1;
            Change_Time_number = 0;
            X_Pos_Value = X_MIN + 1.0 * rand() / RAND_MAX * (X_MAX - X_MIN);
            Y_Pos_Value = Y_MIN + 1.0 * rand() / RAND_MAX * (Y_MAX - Y_MIN);
            Z_Pos_Value = Z_MIN + 1.0 * rand() / RAND_MAX * (Z_MAX - Z_MIN);
            ui->X->setText(QString::number(X_Pos_Value));
            ui->Y->setText(QString::number(Y_Pos_Value));
            ui->Z->setText(QString::number(Z_Pos_Value));
            on_YX_clicked();
        }
    }else
        Change_Time_number = 0;
}
void Widget::on_ZDSC_clicked()
{
    ZDSC_Flag++;
    if(ZDSC_Flag % 2 == 1){
        ui->ZDSC->setText("关闭自动运行");
        ui->YX->setDisabled(true);
        ui->JZ->setDisabled(true);
        ui->HL->setDisabled(true);
    }
    else{
        ui->ZDSC->setText("开启自动运行");
        ui->YX->setEnabled(true);
        ui->JZ->setEnabled(true);
        ui->HL->setEnabled(true);
        ZDSC_Flag = 0;
    }
}

void Widget::on_Time_com_currentIndexChanged(int index)
{
    if(ZDSC_Flag == 1){
        qDebug()<<index;
    }
}

void Widget::on_pushButton_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
    QString lastPath = setting.value("LastFilePath").toString();

    SaveSerialData_FileName_Path = QFileDialog::getSaveFileName(this,
                                                tr("选择文件"),
                                                lastPath,
                                                tr("ALL Files(*)"));
    if(SaveSerialData_FileName_Path.isEmpty())
    {
        ui->Label_SaveSerialData_StateMsg->setText("请选择数据存储文件！");
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->Label_SaveSerialData_StateMsg->setText("文件将存在  " + SaveSerialData_FileName_Path + "！");
        ui->pushButton->setEnabled(true);
    }
}

void Widget::Start_SaveSerialData()     // 开始保存数据
{
    ui->KSBC->setText("停止保存");
    ui->pushButton->setEnabled(false);
    ui->CheckBox_SaveSerialData_WithTime->setEnabled(false);
    saveserialdata->Open_File(SaveSerialData_FileName_Path,  ui->CheckBox_SaveSerialData_WithTime->isChecked());
}

void Widget::Stop_SaveSerialData()      // 停止保存数据
{
    ui->KSBC->setText("开始保存");
    ui->pushButton->setEnabled(true);
    ui->CheckBox_SaveSerialData_WithTime->setEnabled(true);
    saveserialdata->close_File();
}


void Widget::on_KSBC_clicked()
{
    if(ui->KSBC->text() == "开始保存")
    {
        Start_SaveSerialData();
        Save_Date_Flag = 1;
    }
    else
    {
        Stop_SaveSerialData();
        Save_Date_Flag = 0;
    }
}
