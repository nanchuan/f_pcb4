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

#ifndef DIALOGCONTROL_H
#define DIALOGCONTROL_H

#include <QDialog>
#include <QKeyEvent>
#include <QVector>
#include <QLineEdit>
#include <QTimer>
#include <stdint.h>
#include "quaternion.h"
#include "dialogcameracontrol.h"

namespace Ui {
class DialogControl;
}

class DialogControl : public QDialog
{
    Q_OBJECT

public:
    explicit DialogControl(QWidget *parent = 0);
    ~DialogControl();

signals:
    void setMode(uint8_t mode);
    void sendHeartbeat();
    void lockThrottle_setThrottle(const QVector<float> &throttle);
    void lockAttitude(const QByteArray & param);
    void makeHorizontalAttitude(Quaternion q);

public slots:
    void onLockAttitude(const QByteArray & param);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private slots:
    void on_lockAttitude_throttleBase_valueChanged(int value);
    void on_setMode_halt_clicked();
    void on_setMode_lockThrottle_clicked();
    void on_setMode_lockAttitude_clicked();
    void on_lockThrottle_slider0_valueChanged(int value);
    void on_lockThrottle_slider1_valueChanged(int value);
    void on_lockThrottle_slider2_valueChanged(int value);
    void on_lockThrottle_slider3_valueChanged(int value);
    void on_heartbeat_enable_clicked(bool checked);
    void onHeartbeatTimer();
    void on_lockAttitude_hsSensitivity_valueChanged(int value);
    void on_lockAttitude_btReset_clicked();
    void on_btGetReady_clicked();
    void on_lockAttitude_cbCameraControl_stateChanged(int arg1);
    void on_lockAttitude_btCameraSetting_clicked();
    void onLockAttitudeTimer(void);
    void lockAttitude_onControlChanged(void);
    void on_btMakeHorizontalAttitude_clicked();

private:
    void lockAttitude_changeThrottle(int delta);
    void lockThrottle_changed(int val);
    unsigned int getUint(QLineEdit *from, unsigned int default_);
    float getFloat(QLineEdit *from, float default_);
    void lockAttitude_sendControl(void);
    void loadSettings(void);
    Quaternion lockAttitude_getControlAttitude(void);

private:
    Ui::DialogControl *ui;
    enum
    {
        HALT,
        LOCK_THROTTLE,
        LOCK_ATTITUDE
    }m_currentMode;
    QTimer m_heartbeatTimer;
    float m_baseThrottle;
    DialogCameraControl m_dialogCameraControl;
    QTimer m_lockAttitudeTimer;
    bool m_lockAttitudeCanSend;
    bool m_lockAttitudeNeedSend;
};

#endif // DIALOGCONTROL_H
