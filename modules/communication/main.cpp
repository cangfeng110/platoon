#include <thread>


#include "modules/main.h"

#include "modules/communication/communication.h"
#include "modules/communication/receivehighfreinfo.h"
#include "modules/communication/receivelowfreinfo.h"
#include "modules/communication/receiveudp.h"
#include "modules/communication/highfredatacontainer.h"
#include "modules/communication/lowfredatacontainer.h"
#include "modules/communication/udpdatacontainer.h"
#include "modules/communication/senddatacontanier.h"

using namespace platoon::communication;

void CreatRecHighLoop()
{
    ReceiveHighFreInfo receive_high_fre;
    receive_high_fre.Loop();
}

void CreatRecLowLoop()
{
    ReceiveLowFreInfo receive_low_fre;
    receive_low_fre.Loop();
}

void CreatRecUdpLoop()
{
    ReceiveUDP receiveudp; 
    receiveudp.Loop();
}

//MAIN(communication)
int main()
{
    std::thread RCV1(CreatRecHighLoop);
    std::thread RCV2(CreatRecLowLoop);
    std::thread RCV3(CreatRecUdpLoop);
    RCV1.join();
    RCV2.join();
    RCV3.join();
    return 0;
}
