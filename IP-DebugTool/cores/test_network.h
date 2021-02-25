#ifndef TEST_NETWORK_H
#define TEST_NETWORK_H

#include "test_logs.h"
#include "udprecvsocket.h"

class Test_NetWork : public BaseThread
{
    Q_OBJECT
    explicit Test_NetWork(QObject *parent = nullptr);
public:
    static Test_NetWork *bulid(QObject *parent = nullptr);
    bool mac;
protected:
    void run();
    void workDown();

protected slots:
    void initFunSlot();

private:
    Test_Logs *mLogs;
    UdpRecvSocket *mUdp;
};

#endif // TEST_NETWORK_H
