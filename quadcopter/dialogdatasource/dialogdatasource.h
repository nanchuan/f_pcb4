//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 、下位机App和遥控程序，及它们的源代码，以下总称“软件”。
//     软件仅提供参考，js200300953不对软件作任何担保，不对因使用该软件
// 而出现的损失负责。
//     软件可以以学习为目的修改和使用，但不允许以商业的目的使用该软件。
//     修改该软件时，必须保留原版权声明。
// 
//     更多资料见：
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// 圆点博士微型四轴飞行器QQ群：276721324

#ifndef DIALOGDATASOURCE_H
#define DIALOGDATASOURCE_H

#include <QDialog>
#include "textfileparser.h"
#include <QLineEdit>
#include <QTimer>
#include "qextserialport/qextserialport.h"
#include "binaryparser.h"

namespace Ui {
class DialogDataSource;
}

class DialogDataSource : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDataSource(QWidget *parent = 0);
    ~DialogDataSource();
    BinaryParser & getBinaryParser();
    TextFileParser & getTextParser();

private slots:
    void every30ms();
    void on_serial_openClose_clicked();
    void on_serial_checkCrc_stateChanged(int arg1);
    void on_file_selectFile_clicked();
    void on_file_startStop_clicked();
    void on_file_pauseContinue_clicked();
    void si_sl_stop();
    void serial_send(const QByteArray &bytes);
    void on_cbPrintTransmitPacket_stateChanged(int arg1);
    void on_cbPrintReceivePacket_stateChanged(int arg1);

private:
    void initFileSetting();
    void initSerialSetting();
    void connectSignalSlot();
    void dataSorce_getSerialSetting(PortSettings &setting);
    unsigned int getUintFromLineEdit(QLineEdit *from, unsigned int default_);
    void openSerial(void);
    void closeSerial(void);

private:
    const static char * SETTING_DATASOURCE_SERIAL_PORT;
    const static char * SETTING_DATASOURCE_SERIAL_BOUNDRATE;
    const static char * SETTING_DATASOURCE_SERIAL_DATABITS;
    const static char * SETTING_DATASOURCE_SERIAL_PARITY;
    const static char * SETTING_DATASOURCE_SERIAL_STOPBITS;
    const static char * SETTING_DATASOURCE_FILE_PATH;
    const static char * SETTING_DATASOURCE_FILE_PERIOD;

private:
    Ui::DialogDataSource *ui;
    bool m_file_running; // 运行状态，是否正在读文件。
    bool m_file_pause;   // 运行状态，读文件是否被暂停。
    TextFileParser m_textFileParser; // 文本文件解析器。
    QextSerialPort * m_serial;
    bool m_serial_opening;
    BinaryParser m_serialBinaryParser;
    QTimer m_serial_timer;
};

#endif // DIALOGDATASOURCE_H
