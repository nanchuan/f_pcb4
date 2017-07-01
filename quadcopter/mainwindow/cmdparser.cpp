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

#include "cmdparser.h"
#include <QStringList>
#include "mainwindow.h"
#include "../protocol.h"

CmdParser::CmdParser()
{
}

void CmdParser::onCmd(QString cmd)
{
    QStringList sl = cmd.split(QRegExp("\\s+"),QString::SkipEmptyParts);
    if(sl.length() < 1)
        return;
    QString msg = QString("<U>%1</U>").arg(cmd);
    //
    if(sl[0] == QString("help"))
    {
        msg += QString::fromUtf8("<br>目前支持的命令有：<br>help<br>reset<br>");
    }
    else if(sl[0] == QString("reset"))
    {
        QByteArray p;
        p.append(Protocol::BOOTLOADER_CMD);
        p.append(Protocol::BootloaderCmd::ENTER_BOOTLOADER);
        emit sendPacket(p);
    }
    else
    {
        msg += QString::fromUtf8(":&nbsp;&nbsp;没有这个命令。");
    }
    MainWindow::printMessage(msg);
}
