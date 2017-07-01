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

#ifndef GLCUBE_H
#define GLCUBE_H

#include "globject.h"
#include "material.h"

class GlCube : public GlObject
{
public:
    GlCube();
    GlCube(float x,float y,float z);
    void load(float x,float y,float z);
    virtual void draw(void);
    bool setLineWidth(float w);
    Material * getMaterialLine(void)    {return &m_materialLine;}
    Material * getMaterialSurface(void) {return &m_materialsurface;}
    void needDrawLine(bool yes=true)    {m_drawLine = yes;}
    void needDrawSurface(bool yes=true) {m_drawSurface = yes;}
private:
    void genericInit();
    float m_vertex[8][3];
    bool m_drawLine;
    bool m_drawSurface;
    float m_lineWidth;
    Material m_materialLine;
    Material m_materialsurface;
};

#endif // GLCUBE_H
