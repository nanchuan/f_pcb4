//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢����������������������========
//         ==================================================
//     Բ�㲩ʿ΢������������������������λ��������λ��Bootloader
// ����λ��App��ң�س��򣬼����ǵ�Դ���룬�����ܳơ��������
//     ������ṩ�ο���js200300953����������κε�����������ʹ�ø����
// �����ֵ���ʧ����
//     ���������ѧϰΪĿ���޸ĺ�ʹ�ã�������������ҵ��Ŀ��ʹ�ø������
//     �޸ĸ����ʱ�����뱣��ԭ��Ȩ������
// 
//     �������ϼ���
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// Բ�㲩ʿ΢�����������QQȺ��276721324

// app/attitude/mix.c
// 2013-1-8 15:52:55
// js200300953

#include "mix.h"
#include "../math/interface.h"

const static float MIX_MAG_Y = 0.743144f;/*cos(42)*/
const static float MIX_MAG_Z = -0.669130f;/*sin42*/
const static float MIX_MAG_VECTOR[] = {0,0.743144f/*cos(42)*/,-0.669130f/*sin42*/};
const static float MIX_ACC_VECTOR[] = {0,0,1};
const static float MIX_LONGTREM_FACTOR = 0.005;
//const static float MIX_GRAVITY = 9.7883;

void mix_init(void);
void mix_gyr(quaternion * attitude,const float gyr[3],float interval);
void mix_AccMag(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_AccMag_steepestDescentMethod(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_AccMag_rotateMethod(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_Acc_steepestDescentMethod(quaternion * attitude,const float acc[3]);
void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval);
void mix_gyrAccMag_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],const float mag[3],float interval);

void mix_init(void)
{
}

void mix_gyr(quaternion * attitude,const float gyr[3],float interval)
{
    //
    // ����������ת��
    float delta_x = gyr[0] * interval / 2;
    float delta_y = gyr[1] * interval / 2;
    float delta_z = gyr[2] * interval / 2;
    //
    float q_w = attitude->w;
    float q_x = attitude->x;
    float q_y = attitude->y;
    float q_z = attitude->z;
    //
    // �ںϣ���Ԫ���˷���
    attitude->w = q_w         - q_x*delta_x - q_y*delta_y - q_z*delta_z;
    attitude->x = q_w*delta_x + q_x         + q_y*delta_z - q_z*delta_y;
    attitude->y = q_w*delta_y - q_x*delta_z + q_y         + q_z*delta_x;
    attitude->z = q_w*delta_z + q_x*delta_y - q_y*delta_x + q_z;
    quaternion_normalize(attitude);
}

// �����ںϡ�
void mix_AccMag(quaternion * attitude,const float acc[3],const float mag[3])
{
    mix_AccMag_rotateMethod(attitude,acc,mag);
}

// �����ںϣ��ü��ٶȺʹų��������Ư�ơ�
// �ݶ��½�����http://blog.sina.com.cn/s/blog_81f1e268010181v3.html
void mix_AccMag_steepestDescentMethod(quaternion * attitude,const float acc[3],const float mag[3])
{
    //
    // ��λ�����ٶȺʹų�����
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_a = acc[0] * a_rsqrt;
    float y_a = acc[1] * a_rsqrt;
    float z_a = acc[2] * a_rsqrt;
    //
    float h_rsqrt = math_rsqrt(mag[0]*mag[0]+mag[1]*mag[1]+mag[2]*mag[2]);
    float x_h = mag[0] * h_rsqrt;
    float y_h = mag[1] * h_rsqrt;
    float z_h = mag[2] * h_rsqrt;
    //
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    //
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    float x_da = x_q*z_q_2 - w_q*y_q_2     - x_a;
    float y_da = y_q*z_q_2 + w_q*x_q_2     - y_a;
    float z_da = 1 - x_q*x_q_2 - y_q*y_q_2 - z_a;
    //
    float x_dh = MIX_MAG_Y*(x_q*y_q_2 + w_q*z_q_2)     + MIX_MAG_Z*(x_q*z_q_2 - w_q*y_q_2)     - x_h;
    float y_dh = MIX_MAG_Y*(1 - x_q*x_q_2 - z_q*z_q_2) + MIX_MAG_Z*(y_q*z_q_2 + w_q*x_q_2)     - y_h;
    float z_dh = MIX_MAG_Y*(y_q*z_q_2 - w_q*x_q_2)     + MIX_MAG_Z*(1 - x_q*x_q_2 - y_q*y_q_2) - z_h;
    //
    float w_pf =  - x_da*y_q + y_da*x_q + x_dh*(MIX_MAG_Y*z_q - MIX_MAG_Z*y_q) \
            + y_dh*MIX_MAG_Z*x_q - z_dh*MIX_MAG_Y*x_q;
    float x_pf = x_da*z_q + y_da*w_q - z_da*x_q + x_dh*(MIX_MAG_Y*y_q + MIX_MAG_Z*z_q) \
            + y_dh*(MIX_MAG_Z*w_q - MIX_MAG_Y*x_q) - z_dh*(MIX_MAG_Y*w_q + MIX_MAG_Z*x_q);
    float y_pf = - x_da*w_q + y_da*z_q - z_da*y_q + x_dh*(MIX_MAG_Y*x_q - MIX_MAG_Z*w_q) \
            + y_dh*MIX_MAG_Z*z_q + z_dh*(MIX_MAG_Y*z_q - MIX_MAG_Z*y_q);
    float z_pf = x_da*x_q + y_da*y_q + x_dh*(MIX_MAG_Y*w_q + MIX_MAG_Z*x_q) \
            + y_dh *(MIX_MAG_Z*y_q - MIX_MAG_Y*z_q) + z_dh*MIX_MAG_Y*y_q;
    //
    attitude->w -= w_pf * MIX_LONGTREM_FACTOR;
    attitude->x -= x_pf * MIX_LONGTREM_FACTOR;
    attitude->y -= y_pf * MIX_LONGTREM_FACTOR;
    attitude->z -= z_pf * MIX_LONGTREM_FACTOR;
    quaternion_normalize(attitude);
}

void mix_AccMag_rotateMethod(quaternion * attitude,const float acc[3],const float mag[3])
{
    quaternion_fromFourVectorRotation(attitude,acc,MIX_ACC_VECTOR,mag,MIX_MAG_VECTOR);
    //
    //quaternion_fromTwoVectorRotation(attitude,acc,MIX_ACC_VECTOR);
    //quaternion_fromTwoVectorRotation(attitude,mag,MIX_MAG_VECTOR);
}

// �����ںϣ��ü��ٶȾ���Ư�ơ�
// �ݶ��½�����http://blog.sina.com.cn/s/blog_81f1e268010181v3.html
void mix_Acc_steepestDescentMethod(quaternion * attitude,const float acc[3])
{
    //
    // ��λ�����ٶȺʹų�����
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_a = acc[0] * a_rsqrt;
    float y_a = acc[1] * a_rsqrt;
    float z_a = acc[2] * a_rsqrt;
    //
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    //
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    float x_da = x_q*z_q_2 - w_q*y_q_2     - x_a;
    float y_da = y_q*z_q_2 + w_q*x_q_2     - y_a;
    float z_da = 1 - x_q*x_q_2 - y_q*y_q_2 - z_a;
    //
    float w_pf =  - x_da*y_q + y_da*x_q;
    float x_pf = x_da*z_q + y_da*w_q - z_da*x_q;
    float y_pf = - x_da*w_q + y_da*z_q - z_da*y_q;
    float z_pf = x_da*x_q + y_da*y_q;
    //
    const float factor = 0.005;
    attitude->w -= w_pf * factor;
    attitude->x -= x_pf * factor;
    attitude->y -= y_pf * factor;
    attitude->z -= z_pf * factor;
    quaternion_normalize(attitude);
}

// ������ں����ݺͼ��ٶȡ�
void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval)
{
    const static float FACTOR = 0.001;
    //
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    // ���ٶȼƵĶ�������λ����
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_aa = acc[0] * a_rsqrt;
    float y_aa = acc[1] * a_rsqrt;
    float z_aa = acc[2] * a_rsqrt;
    //
    // ���������µ��������ٶȳ�������λ����
    float x_ac = x_q*z_q_2 - w_q*y_q_2;
    float y_ac = y_q*z_q_2 + w_q*x_q_2;
    float z_ac = 1 - x_q*x_q_2 - y_q*y_q_2;
    //
    // ����ֵ�볣���Ĳ����
    float x_ca = y_aa * z_ac - z_aa * y_ac;
    float y_ca = z_aa * x_ac - x_aa * z_ac;
    float z_ca = x_aa * y_ac - y_aa * x_ac;
    //
    // ����������ת��
    float delta_x = gyr[0] * interval / 2 + x_ca * FACTOR;
    float delta_y = gyr[1] * interval / 2 + y_ca * FACTOR;
    float delta_z = gyr[2] * interval / 2 + z_ca * FACTOR;
    //
    // �ںϣ���Ԫ���˷���
    attitude->w = w_q         - x_q*delta_x - y_q*delta_y - z_q*delta_z;
    attitude->x = w_q*delta_x + x_q         + y_q*delta_z - z_q*delta_y;
    attitude->y = w_q*delta_y - x_q*delta_z + y_q         + z_q*delta_x;
    attitude->z = w_q*delta_z + x_q*delta_y - y_q*delta_x + z_q;
    quaternion_normalize(attitude);
}

void mix_gyrAccMag_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],const float mag[3],float interval)
{
    const static float FACTOR = 0.001;
    //
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    // ��λ�����ٶȼƵĶ�����
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_aa = acc[0] * a_rsqrt;
    float y_aa = acc[1] * a_rsqrt;
    float z_aa = acc[2] * a_rsqrt;
    //
    // ��λ�����̵Ķ�����
    float h_rsqrt = math_rsqrt(mag[0]*mag[0]+mag[1]*mag[1]+mag[2]*mag[2]);
    float x_hh = mag[0] * h_rsqrt;
    float y_hh = mag[1] * h_rsqrt;
    float z_hh = mag[2] * h_rsqrt;
    //
    // ���������µ��������ٶȳ������ѵ�λ����
    float x_ac = x_q*z_q_2 - w_q*y_q_2;
    float y_ac = y_q*z_q_2 + w_q*x_q_2;
    float z_ac = 1 - x_q*x_q_2 - y_q*y_q_2;
    //
    // ���������µĵشų��������ѵ�λ����
    float x_hc = MIX_MAG_Y*(x_q*y_q_2 + w_q*z_q_2)     + MIX_MAG_Z*(x_q*z_q_2 - w_q*y_q_2)    ;
    float y_hc = MIX_MAG_Y*(1 - x_q*x_q_2 - z_q*z_q_2) + MIX_MAG_Z*(y_q*z_q_2 + w_q*x_q_2)    ;
    float z_hc = MIX_MAG_Y*(y_q*z_q_2 - w_q*x_q_2)     + MIX_MAG_Z*(1 - x_q*x_q_2 - y_q*y_q_2);
    //
    // ����ֵ�볣���Ĳ����
    float x_ca = y_aa * z_ac - z_aa * y_ac;
    float y_ca = z_aa * x_ac - x_aa * z_ac;
    float z_ca = x_aa * y_ac - y_aa * x_ac;
    float x_ch = y_hh * z_hc - z_hh * y_hc;
    float y_ch = z_hh * x_hc - x_hh * z_hc;
    float z_ch = x_hh * y_hc - y_hh * x_hc;
    //
    // ����������ת��
    float delta_x = gyr[0] * interval / 2 + (x_ca + x_ch) * FACTOR;
    float delta_y = gyr[1] * interval / 2 + (y_ca + y_ch) * FACTOR;
    float delta_z = gyr[2] * interval / 2 + (z_ca + z_ch) * FACTOR;
    //
    // �ںϣ���Ԫ���˷���
    attitude->w = w_q         - x_q*delta_x - y_q*delta_y - z_q*delta_z;
    attitude->x = w_q*delta_x + x_q         + y_q*delta_z - z_q*delta_y;
    attitude->y = w_q*delta_y - x_q*delta_z + y_q         + z_q*delta_x;
    attitude->z = w_q*delta_z + x_q*delta_y - y_q*delta_x + z_q;
    quaternion_normalize(attitude);
}
