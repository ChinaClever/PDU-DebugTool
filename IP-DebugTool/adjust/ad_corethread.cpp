/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"
#include "dblogs.h"
extern QString user_land_name();

Ad_CoreThread::Ad_CoreThread(QObject *parent) : BaseThread(parent)
{
    mLedSi = Ad_LedSi::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
}

Ad_CoreThread *Ad_CoreThread::bulid(QObject *parent)
{
    static Ad_CoreThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_CoreThread(parent);
    return sington;
}


bool Ad_CoreThread::workDown()
{
    bool ret = true;
    if(mDt->devType) {
        ret = mAdjust->startAdjust();
    } else if(mCfg->si_led) {
        ret = mLedSi->startAdjust();
    }

    if(mItem->step == Test_vert) {
        ret = mResult->resEnter();
    } else if(mItem->step == Test_End) {
        ret = mResult->initRtuThread();
    }

    return ret;
}



