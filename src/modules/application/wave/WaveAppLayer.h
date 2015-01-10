//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef WAVEAPPLAYER_H_
#define WAVEAPPLAYER_H_

#include <map>
#include <BaseApplLayer.h>
#include <Consts80211p.h>
#include <WaveShortMessage_m.h>
#include "base/connectionManager/ChannelAccess.h"
#include <WaveAppToMac1609_4Interface.h>

using Channels::ChannelNumber;

#ifndef DBG
#define DBG EV
#endif

/*
 * WAVE application layer base class.
 * */

class WaveAppLayer : public BaseApplLayer {
public:
    WaveAppLayer();
    virtual ~WaveAppLayer();
    virtual void initialize(int stage);
    virtual void finish();

    virtual  void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);

    enum WaveApplMessageKinds {
            SERVICE_PROVIDER = LAST_BASE_APPL_MESSAGE_KIND,
            SEND_WSA_EVENT,SEND_BROADCAST_EVENT,
            SEND_UNICAST_EVENT,TIMING_EVNET
        };
protected:
    static const simsignalwrap_t mobilityStateChangedSignal;
    /** @brief handle messages from below */
    virtual void handleLowerMsg(cMessage* msg);
    /** @brief handle self messages */
    virtual void handleSelfMsg(cMessage* msg);

    virtual WaveShortMessage* prepareBaseMSG(std::string name, int dataLengthBits, int priority,int channelNumber,double speed,const Coord &direction,simtime_t timeStamp);
    virtual WaveShortMessage* prepareWSM(WaveShortMessage* msg, int psid, int hopLimit, int flag,int recvId,int originalSender,int usrPriority);
    virtual WaveShortMessage* prepareWSA(WaveShortMessage* msg, int psid, int hopLimit, int flag,int userPriority,int dst);
    virtual WaveShortMessage* prepareWSA2(WaveShortMessage* msg,int psid2, int hopLimit2, int flag2, int userPriority2, int sch2, int dst2,simtime_t timeStamp2);
    /*
    virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int serial=0);
    virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int hopLimit, int flag, int userPriority, int psid,int serial=0);
    */

    virtual void sendWSM(WaveShortMessage* wsm);
    virtual void onWSA(WaveShortMessage* wsm) = 0;
    virtual void onWSM(WaveShortMessage* wsm) = 0;

    virtual void handlePositionUpdate(cObject* obj);

protected:
    int WSALengthBits;
    int WSAPriority;
    //bool sendData;
    //bool sendWSA;
    simtime_t individualOffset;
    int dataLengthBits;
    bool dataOnSch;
    int dataPriority;
    Coord curPosition;
    int mySCH;
    int myId;


    cMessage* sendWSAEvt;

    //WaveAppToMac1609_4Interface* myMac;
};

#endif /* WAVEAPPLAYER_H_ */
