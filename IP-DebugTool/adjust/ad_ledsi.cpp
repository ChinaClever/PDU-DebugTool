/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "ad_ledsi.h"

Ad_LedSi::Ad_LedSi(QObject *parent) : BaseThread(parent)
{
    mPacket = sDataPacket::bulid();
}

Ad_LedSi *Ad_LedSi::bulid(QObject *parent)
{
    static Ad_LedSi* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_LedSi(parent);
    return sington;
}

bool Ad_LedSi::transmit(uchar *buf, int len)
{
    bool ret = false;
    uchar recv[64] = {0};
    len = mModbus->transmit(buf, len, recv, 10);
    if(len > 0) {
        if(recv[1] == buf[1]) ret = true;
    } else {
        qDebug() << "Ad_LedSi err " << len;
    }

    return ret;
}

bool Ad_LedSi::writeCmd(uchar fn, uchar line)
{
    uchar cmd[] = {0x7B, 0xA0, 0x00, 0x00, 0x00, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = fn;
    cmd[2] = line;
    uchar crc = mModbus->xorNum(cmd, len-1);
    cmd[len-1] = crc;

    return transmit(cmd, len);
}


bool Ad_LedSi::writeDc()
{
    updatePro(tr("发送直流偏移命令！"));
    bool ret = writeCmd(0xA2, 0);
    if(ret) ret = delay(4);//15
    if(!ret) return ret;

    updatePro(tr("设置标准源电流6A"));
    ret = YC_Dc107::bulid()->setCur(60, 6);
    if(ret) ret = writeCmd(0xA1, 0);

    return ret;
}

bool Ad_LedSi::writeAc()
{
    bool ret = true;
    int line = mDt->lines;
    for(int i=0; i<line; ++i) {
        updatePro(tr("L%1 校准").arg(i+1), ret, 3);
        if(ret) ret = writeCmd(0xA1, i);
        if(!ret) {
            updatePro(tr("L%1 校准失败！").arg(i+1), ret); break;
        }
    }

    return ret;
}


bool Ad_LedSi::sentCmd()
{
    bool ret = true;
    if(DC == mDt->ac) {
        ret = writeDc();
    } else {
        ret = writeAc();
    }

    return ret;
}

bool Ad_LedSi::startAdjust()
{
    mItem->step = Test_Ading;
    bool ret = sentCmd();
    if(ret) {
        mItem->step = Test_vert;
    } else {
        mItem->step = Test_vert;
    }

    return updatePro(tr("校准返回！"), ret, 1);
}
