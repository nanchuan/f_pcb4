﻿//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
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

#ifndef BINARYPARSER_H
#define BINARYPARSER_H

#include <QObject>
#include <QByteArray>
#include "quaternion.h"
#include <stdint.h>
#include <QVector>

class BinaryParser : public QObject
{
    Q_OBJECT
public:
    BinaryParser();
    void needCheckCrc(bool need = true);
    void needPrintTransmitPacket(bool need = true);
    void needPrintReceivePacket(bool need = true);

signals:
    void displayAttitudeQuaternion(const Quaternion & atti);
    void displayTranslation(float x,float y,float z);
    void send(const QByteArray &bytes);
    void onReturnVector(uint8_t type,float x,float y,float z);
    void onReturnStatus(uint8_t which,const QVector<uint8_t> &value);
    void onReturnMessage(const QString &msg);
    void onBootloaderStatus(uint8_t status,QByteArray param);
    void onParameter(const QByteArray & param);
    void onControlLockAttitude(const QByteArray & param);

public slots:
    void onReceivedData(const QByteArray &bytes);
    void cmd_getAttitudeQuaternion();
    void cmd_getTranslation();
    void cmd_getAttitudeAngle();
    void cmd_getVector(uint8_t type);
    void cmd_getStatus(const QVector<uint8_t> &which);
    void cmd_bootloadCmd(uint8_t cmd,QByteArray param);
    void cmd_parameter(const QByteArray &param);
    void sendPacket(const QByteArray & packet); // packet=|type|content|

    void control_setMode(uint8_t mode);
    void control_sendHeartbeat();
    void control_lockThrottle_setThrottle(const QVector<float> &throttle);
    void control_lockAttitude(const QByteArray & param);
private:
    void handleOndByte(int byte);
    void frameCompleted(const char frame[],int len);
    bool checkFrameCrc(const char frame[],int len);
    void transmitFrame(uint8_t protocol,uint8_t type,const void *data,uint8_t len);

private:
    static const int FINDING_55   = 100;
    static const int NEED_AA      = 101;
    static const int NEED_LENGTH  = 102;
    static const int DATA_LEN_MAX = 32;
    static const int DATA_START   = 0;
    int m_status;
    int m_frameLength;
    char m_buff[DATA_LEN_MAX];
    bool m_checkCrc;
    bool m_needPrintTransmitPacket;
    bool m_needPrintReceivePacket;
};

#endif // BINARYPARSER_H
