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

#include "textfileparser.h"
#include "protocol.h"
#include <QTextStream>

TextFileParser::TextFileParser()
{
    m_file = NULL;
    m_timer = NULL;
}

TextFileParser::~TextFileParser()
{
}

// 成功就返回true，否则false。
// name:文件名，period:时标周期，ms。
bool TextFileParser::start(const QString &file_name,int period_ms)
{
    if(m_timer != NULL || m_file != NULL)
        stop();
    //
    m_file = new QFile(file_name);
    if(! m_file->exists() || ! m_file->open(QFile::ReadOnly | QIODevice::Text))
        return false;
    //
    m_timer = new QTimer(this);
    m_timer->setInterval(period_ms);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(periodPass()));
    m_timer->start();
    //
    return true;
}

void TextFileParser::pause()
{
    if(m_timer != NULL)
        m_timer->stop();
}

void TextFileParser::continue_()
{
    if(m_timer != NULL)
        m_timer->start();
}

void TextFileParser::stop()
{
    if(m_timer != NULL)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = NULL;
    }
    if(m_file != NULL)
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
    //
    emit si_stop();
}

// 不断解析，直到遇到时标。
void TextFileParser::periodPass()
{
    int type = 0;
    do
    {
        QByteArray buf = m_file->readLine();
        if(buf.length() <= 0)
        {
            stop();
            return;
        }
        //
        QTextStream ts(buf);
        ts >> type;
        switch(type)
        {
        case Protocol::RETURN_ATTITUDE_QUATERNION :
            {
            float w,x,y,z;
            ts >> w >> x >> y >> z;
                Quaternion atti(w,x,y,z);
                emit displayAttitudeQuaternion(atti);
            }
            break;
        case Protocol::RETURN_TRANSLATION :
            {
                float x,y,z;
                ts >> x >> y >> z;
                emit displayTranslation(x,y,z);
            }
            break;
        }
    }while(type != Protocol::PERIOD_MARK);
}
