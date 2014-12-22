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

#include <wave/WaveAppLayer.h>

const simsignalwrap_t WaveAppLayer::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);

WaveAppLayer::WaveAppLayer() {
    // TODO Auto-generated constructor stub
}

WaveAppLayer::~WaveAppLayer() {
    // TODO Auto-generated destructor stub
}

void WaveAppLayer::initialize(int stage){
    BaseApplLayer::initialize(stage);
    if(stage == 0){
    //myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(
    //                getParentModule());
    //assert(myMac);
    myId = getParentModule()->getIndex();


    headerLength = par("headerLength").longValue();
    //double maxOffset = par("maxOffset").doubleValue();
    //sendWSA = par("sendWSA").boolValue();
    WSALengthBits = par("WSALengthBits").longValue();
    WSAPriority = par("WSAPriority").longValue();

    //sendData = par("sendData").boolValue();
    dataLengthBits = par("dataLengthBits").longValue();
    dataOnSch = par("dataOnSch").boolValue();
    dataPriority = par("dataPriority").longValue();

    sendWSAEvt = new cMessage("WSA evt", SEND_WSA_EVENT);
    //simulate asynchronous channel access
    //double offSet = dblrand() * (par("WSAInterval").doubleValue()/2);
    //offSet = offSet + floor(offSet/0.050)*0.050;
    //individualOffset = dblrand() * maxOffset;

    //DBG<<"##WaveAppLayer: the offSet time is: "<< offSet << "and the individualOffset time is: "<< individualOffset << std::endl;


    findHost()->subscribe(mobilityStateChangedSignal, this);

    /*
    if (sendWSA) {
                scheduleAt(simTime() + offSet, sendWSAEvt);
            }
    */
    }
}

WaveShortMessage* WaveAppLayer::prepareBaseMSG(std::string name, int dataLengthBits, int priority,int channelNum,double speed, const Coord &direction,simtime_t timeStamp)
{
    WaveShortMessage* wsm = new WaveShortMessage(name.c_str());
    wsm->addBitLength(headerLength);
    wsm->addBitLength(dataLengthBits);

    switch(channelNum){
    case 172: wsm->setChannelNumber(Channels::CRIT_SOL);break;
    case 174: wsm->setChannelNumber(Channels::SCH1);break;
    case 176: wsm->setChannelNumber(Channels::SCH2);break;
    case 178: wsm->setChannelNumber(Channels::CCH);break;
    case 180: wsm->setChannelNumber(Channels::SCH3);break;
    case 182: wsm->setChannelNumber(Channels::SCH4);break;
    case 184: wsm->setChannelNumber(Channels::HPPS);break;
    }

    wsm->setPriority(priority);
    wsm->setWsmVersion(1);
    wsm->setTimestamp(timeStamp);
    wsm->setSenderAddress(myId);
    //wsm->setOriginalAddress(myId);
    wsm->setSenderPos(curPosition);
    //wsm->setSerial(serial);
    wsm->setSpeed(speed);
    wsm->setDirection(direction);

    if (name == "wsa") {
        DBG << "Creating WSA with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }
    if (name == "wsm") {
        DBG << "Creating WSM with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }

    return wsm;
}

WaveShortMessage* WaveAppLayer::prepareWSM(WaveShortMessage *msg, int psid, int hopLimit, int flag, int rcvId,int originalSender)
{
    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    wsm->setPsid(psid);
    wsm->setHopLimit(hopLimit);
    wsm->setFlag(flag);
    wsm->setOriginalAddress(originalSender);
    wsm->setNextForwardAddress(rcvId);
    wsm->setRecipientAddress(rcvId);

    DBG << "creating WSM packet with hopLimit " << hopLimit << "to the receiver " << rcvId << "at the time " << wsm->getTimestamp() << std::endl;

    return wsm;
}

WaveShortMessage* WaveAppLayer::prepareWSA(WaveShortMessage *msg, int psid, int hopLimit, int flag, int userPriority,int dst)
{
    WaveShortMessage* wsa = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsa);
    wsa->setPsid(psid);
    wsa->setFlag(flag);
    wsa->setUserPriority(userPriority);
    wsa->setOriginalAddress(myId);
    wsa->setNextForwardAddress(-1);
    wsa->setRecipientAddress(dst);
    wsa->setHopLimit(hopLimit);
    wsa->setSerial(0);

    DBG << "creating WSA packet with userPriority " << userPriority << "at the time " << wsa->getTimestamp() << std::endl;

    return wsa;
}

WaveShortMessage* WaveAppLayer::prepareWSA2(WaveShortMessage* msg,int psid2, int hopLimit2, int flag2, int userPriority2, int sch2, int dst2,simtime_t timeStamp2)
{
    WaveShortMessage* wsa = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsa);
    wsa->setPsid2(psid2);
    wsa->setFlag2(flag2);
    wsa->setUserPriority2(userPriority2);
    wsa->setSch2(sch2);
    wsa->setDst2(dst2);
    wsa->setHopLimit2(hopLimit2);
    wsa->setTimeStamp2(timeStamp2);

    DBG << "create WSA second service with psid" << wsa->getPsid2() << " flag :" << wsa->getFlag2() <<
            " userPriority: " << wsa->getUserPriority2() << "sch2:" << wsa->getSch2() << "dst: " << wsa->getDst2() << std::endl;
    return wsa;
}
/*
WaveShortMessage* WaveAppLayer::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int serial)
{
        WaveShortMessage* wsm =     new WaveShortMessage(name.c_str());
        wsm->addBitLength(headerLength);
        wsm->addBitLength(lengthBits);

        switch (channel) {
            case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
            case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
        }
        wsm->setPsid(0);
        wsm->setPriority(priority);
        wsm->setWsmVersion(1);
        wsm->setTimestamp(simTime());
        wsm->setSenderAddress(myId);
        wsm->setRecipientAddress(rcvId);
        wsm->setSenderPos(curPosition);
        wsm->setSerial(serial);

        if (name == "wsa") {
            DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
        }
        if (name == "data") {
            DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
        }

        return wsm;
}

WaveShortMessage* WaveAppLayer::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int hopLimit, int flag, int userPriority, int psid,int serial=0)
{
    WaveShortMessage *wsm = new WaveShortMessage(name.c_str());
    wsm->addBitLength(headerLength);
    wsm->addBitLength(lengthBits);
    switch(channel){
    case type_SCH: wsm->setChannelNumber(Channels::SCH1);break;
    case type_CCH: wsm->setChannelNumber(Channels::CCH);break;
    }

    wsm->setPsid(psid);
    wsm->setPriority(priority);
    wsm->setWsmVersion(1);
    wsm->setTimestamp(simTime());
    wsm->setSenderAddress(myId);
    wsm->setSenderPos(curPosition);
    wsm->setRecipientAddress(rcvId);
    wsm->setSerial(serial);

    if (name == "wsa") {
        DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }
    if (name == "data") {
        DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }
}

*/

void WaveAppLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj){
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
}

void WaveAppLayer::handlePositionUpdate(cObject* obj) {
    ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
    curPosition = mobility->getCurrentPosition();
}

void WaveAppLayer::handleLowerMsg(cMessage* msg) {

    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);

    if (std::string(wsm->getName()) == "wsa") {
        onWSA(wsm);
    }
    else if (std::string(wsm->getName()) == "wsm") {
        onWSM(wsm);
    }
    else {
        DBG << "unknown message (" << wsm->getName() << ")  received\n";
    }
    //delete(msg);
}

void WaveAppLayer::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
            case SEND_WSA_EVENT: {
                WaveShortMessage *wsa = prepareBaseMSG("wsa",WSALengthBits,WSAPriority,178,0.0,Coord::ZERO,simTime());
                wsa = prepareWSA(wsa,10,1,0,5,-1);
                sendWSM(wsa);
                //sendWSM(prepareWSM("wsa", WSALengthBits, type_CCH, WSAPriority, 0, -1));
                scheduleAt(simTime() + par("WSAInterval").doubleValue(), sendWSAEvt);
                break;
            }
            default: {
                if (msg)
                    DBG << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
                break;
            }
        }
}

void WaveAppLayer::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}

void WaveAppLayer::finish(){
    if (sendWSAEvt->isScheduled()) {
            cancelAndDelete(sendWSAEvt);
        }
        else {
            delete sendWSAEvt;
        }

        findHost()->unsubscribe(mobilityStateChangedSignal, this);
}


