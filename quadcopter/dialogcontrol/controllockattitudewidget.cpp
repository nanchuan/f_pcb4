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

#include "controllockattitudewidget.h"

ControlLockAttitudeWidget::ControlLockAttitudeWidget(QWidget *parent) :
    QWidget(parent)
{
    m_posX = m_posY = 0;
    m_horizontalSensitivity = 0.005;
}

void ControlLockAttitudeWidget::setHorizontalSensitivity(float sen)
{
    m_posX = m_posX * m_horizontalSensitivity / sen;
    m_posY = m_posY * m_horizontalSensitivity / sen;
    //
    m_horizontalSensitivity = sen;
    //
    refreshControl();
    update();
}

void ControlLockAttitudeWidget::reset()
{
    m_posX = 0;
    m_posY = 0;
    //
    refreshControl();
    update();
}

void ControlLockAttitudeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //
    // 画边框。
    QRect frame = this->rect();
    int center_x = (frame.left() + frame.right()) / 2;
    int center_y = (frame.top() + frame.bottom()) / 2;
    frame.setWidth(frame.width()-1);
    frame.setHeight(frame.height()-1);
    painter.drawRect(frame);
    //
    painter.drawLine(center_x-frame.width()/2,center_y,center_x+frame.width()/2,center_y);
    painter.drawLine(center_x,center_y-frame.height()/2,center_x,center_y+frame.height()/2);
    //
    static const int HALF_SIZE = 20;
    painter.fillRect(QRectF(QPoint(center_x-HALF_SIZE + m_posX,center_y - HALF_SIZE - m_posY),
                            QSize(2*HALF_SIZE,2*HALF_SIZE)),Qt::SolidPattern);
    //
    float y = 8;
    painter.drawText(8,y+=16,QString("w=%1").arg(m_control.w()));
    painter.drawText(8,y+=16,QString("x=%1").arg(m_control.x()));
    painter.drawText(8,y+=16,QString("y=%1").arg(m_control.y()));
    painter.drawText(8,y+=16,QString("z=%1").arg(m_control.z()));
}

void ControlLockAttitudeWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void ControlLockAttitudeWidget::refreshControl()
{
    m_control.assign(1,-m_posY * m_horizontalSensitivity,m_posX * m_horizontalSensitivity,0);
    m_control.nomalize();
    //
    emit controlChanged();
}

void ControlLockAttitudeWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    //
    if (event->buttons() & Qt::LeftButton)
    {
        m_posX += dx;
        m_posY += -dy;
        //
        refreshControl();
        update();
    }
    m_lastPos = event->pos();
}
