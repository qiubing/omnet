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

#include <test/AppTest1.h>
using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;
using std::make_pair;

const simsignalwrap_t AppTest1::parkingStateChangedSignal = simsignalwrap_t(TRACI_SIGNAL_PARKING_CHANGE_NAME);

Define_Module(AppTest1);

void AppTest1::initialize(int stage){
    WaveAppLayer::initialize(stage);
    if(stage == 0){
        traci = TraCIMobilityAccess().get(getParentModule());
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(getParentModule());
        ASSERT(myMac);

        sentWSM = par("sentWSM").boolValue();
        sentWSA = par("sentWSA").boolValue();
        lastDroveAt = simTime();
        findHost()->subscribe(parkingStateChangedSignal,this);
        sendWhileParking = par("sendWhileParking").boolValue();
        isParking = false;
        isProvider = true;
        channelNum = -1;
        serial = 1;
        cycleTime = 0.0;

        /*get the car move status*/
        carMove.setCarId(myId);
        carMove.setSpeed(traci->getSpeed());
        carMove.setStart(traci->getCurrentPosition(),simTime());
        carMove.setDirectionByVector(Coord::ZERO);

        nextWSAEvent = new cMessage("next WSA Event");
        nextWSMEvent = new cMessage("next WSM Event");
        updatePOSEvent = new cMessage("next update position Event");

        //simulate asynchronous channel access
        double maxOffset = par("maxOffset").doubleValue();
        //double offSet = dblrand()*(par("WSAInterval").doubleValue()/2);
        //offSet = offSet + floor(offSet/0.050)*0.050;
        double offSet = dblrand()*0.01;//[0,0.025)
        individualOffset = dblrand() * maxOffset;

        /*send message to the Id is 6 */
        if(myId == 0)
        {
            cycleTime = 3*individualOffset + 0.1;
            timeEvent = new cMessage("time Event",SEND_UNICAST_EVENT);
            scheduleAt(simTime() + 43.00 + individualOffset,timeEvent);
        }


        DBG<<"##WaveAppLayer: the offSet time is: "<< offSet << "and the individualOffset time is: "<< individualOffset << std::endl;

        //initialize the provider service list
        if(myId%4 == 0)
        {
            providerServiceList.clear();
            int psid = intuniform(1,10);
            int priority = intuniform(1,6);
            cService* service = new cService(psid,Channels::SCH1,priority,-1,0,2,myId,1);
            service->setTimestamp(simTime());
            //map<int,cService*> pService;
            psidService pService;
            pService.insert(make_pair(psid,service));
            providerServiceList.insert(make_pair(Channels::SCH1,pService));

            DBG<<"node["<<myId<<"] provide " << psid << "service at the channel : "<< service->getSch() << "with priority: " << priority <<
                    " and flag :" << service->getFlag() << "at the time:" << service->getTimestamp() << std::endl;
        }else if(myId%4 == 1){
            providerServiceList.clear();
            int psid = intuniform(11,20);
            int priority = intuniform(1,6);
            cService* service = new cService(psid,Channels::SCH2,priority,-1,1,3,myId,1);
            service->setTimestamp(simTime());
            psidService pService;
            pService.insert(make_pair(psid,service));
            providerServiceList.insert(make_pair(Channels::SCH2,pService));
            DBG<<"node["<<myId<<"] provide " << psid << "service at the channel : "<< service->getSch() << "with priority: " << priority <<
            " and flag :" << service->getFlag() << "at the time:" << service->getTimestamp() << std::endl;

        }else if(myId%4 == 2)
        {
            providerServiceList.clear();
            int psid = intuniform(21,30);
            int priority = intuniform(1,6);
            cService* service = new cService(psid,Channels::SCH3,priority,-1,2,3,myId,1);
            service->setTimestamp(simTime());
            psidService pService;
            pService.insert(make_pair(psid,service));
            providerServiceList.insert(make_pair(Channels::SCH3,pService));
            DBG<<"node["<<myId<<"] provide " << psid << "service at the channel : "<< service->getSch() << "with priority: " << priority <<
            " and flag :" << service->getFlag() << "at the time:" << service->getTimestamp() << std::endl;

        }else{
            providerServiceList.clear();
            int psid = intuniform(21,30);
            int priority = intuniform(1,6);
            cService* service = new cService(psid,Channels::SCH4,priority,-1,1,4,myId,1);
            service->setTimestamp(simTime());
            psidService pService;
            pService.insert(make_pair(psid,service));
            providerServiceList.insert(make_pair(Channels::SCH4,pService));
            DBG<<"node["<<myId<<"] provide " << psid << "service at the channel : "<< service->getSch() << "with priority: " << priority <<
            " and flag :" << service->getFlag() << "at the time:" << service->getTimestamp() << std::endl;

        }
        availableServiceList.clear();
        subscribeServiceList.clear();
        carList.clear();
        carMoveStatus.clear();

        if(isProvider && !sentWSA )
        {
            findHost()->getDisplayString().updateWith("r=16,red");
            //channelNum = allocateChannel(providerServiceList);
            if(nextWSAEvent->isScheduled())
            {
                cancelEvent(nextWSAEvent);
            }
            scheduleAt(simTime() + 2.0 + offSet,nextWSAEvent);
            isProvider = false;
        }

        //begin send wsm packet at the SCH time
        if(!sentWSM)
        {
            scheduleAt(simTime()+ 2.0 + SWITCHING_INTERVAL_11P + offSet,nextWSMEvent);
        }

        /*the update interval of the position is 5s */
        double randomOffset = dblrand()*0.01+ intuniform(0,9)*0.1;
        scheduleAt(simTime() + 5.0 + randomOffset,updatePOSEvent);
    }
}

void AppTest1::onWSA(WaveShortMessage* msg){
    /*1.update the available service list and the car status list*/
    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    if(wsm->getPsid() != 0)
    {
        int psid = wsm->getPsid();
        int priority = wsm->getUserPriority();
        int sch = wsm->getChannelNumber();
        int dst = wsm->getRecipientAddress();
        int flag = wsm->getFlag();
        int hopLimit = wsm->getHopLimit();
        int originalSender = wsm->getOriginalAddress();
        int serial = wsm->getSerial();
        cService *service = new cService(psid,sch,priority,dst,flag,hopLimit,originalSender,serial);
        service->setTimestamp(wsm->getTimestamp());
        insertService(&availableServiceList,service);
    }

    if(wsm->getPsid2() != 0)
    {
        cService *service2 = new cService(wsm->getPsid2(),wsm->getSch2(),wsm->getUserPriority2(),wsm->getDst2(),wsm->getFlag2(),wsm->getHopLimit2(),wsm->getOriginalAddress(),wsm->getSerial());
        service2->setTimestamp(wsm->getTimeStamp2());
        insertService(&availableServiceList,service2);
    }

    /*update other car status*/
    CarMove *carstatus = new CarMove(wsm->getSenderAddress());
    carstatus->setDirectionByVector(wsm->getDirection());
    carstatus->setSpeed(wsm->getSpeed());
    carstatus->setStart(wsm->getSenderPos(),wsm->getTimestamp());
    carMoveStatus[wsm->getSenderAddress()] = carstatus;
    for(int i = 0; i < 10;i++)
    {
        int carId = wsm->getCarlist(i).getCarId();
        if(carId != -1)
        {
            if(carId == myId)
            {
                /*itself position status,skip*/
                continue;
            }

            map<int,CarMove*>::iterator it = carMoveStatus.find(carId);
            if(it != carMoveStatus.end())
            {
                simtime_t time1 = it->second->getStartTime();
                simtime_t time2 = wsm->getCarlist(i).getStartTime();
                if(time2 - time1 <= 0)
                {
                    /*the car position is old than the previous position,so do not update*/
                    continue;
                }
            }
            CarMove &car = wsm->getCarlist(i);
            CarMove *carstatus1 = new CarMove(carId);
            carstatus1->setDirectionByVector(car.getDirection());
            carstatus1->setSpeed(car.getSpeed());
            carstatus1->setStart(car.getStartPos(),car.getStartTime());
            carMoveStatus[carId] = carstatus1;
        }else
        {
            break;
        }
    }


    /*2. then choose which service to use*/
    cService *service3 = NULL;
    if((service3 = findCommonService(availableServiceList,subscribeServiceList)) != NULL)
    {
        /*change the channel to the service channel and set the mode as RX*/
        myMac->changeServiceChannel(service3->getSch());
        return;
    }

    int aChannel = -1;
    if(wsm->getRecipientAddress() == myId)
    {
        aChannel = wsm->getChannelNumber();
    }else if(wsm->getDst2() == myId)
    {
        aChannel = wsm->getSch2();
    }else
    {
        aChannel =  allocateChannel(availableServiceList);
    }

    /*find the max weight channel*/
    if(aChannel != -1)
    {
        /*change the service channel to listen*/
        myMac->changeServiceChannel(aChannel);
        return;
    }
}

void AppTest1::onWSM(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    findHost()->getDisplayString().updateWith("r=16,green");
    annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), traci->getPositionAt(simTime()), "blue"));
    if (traci->getRoadId()[0] != ':') traci->commandChangeRoute(wsm->getWsmData(), 9999);

    /*1.first let we delete the service we have receive in the available servicelist*/
    int channel = wsm->getChannelNumber();
    int psid = wsm->getPsid();
    deleteServie(&availableServiceList,channel,psid);

    /*2.judge the packet is to me.if the packet is to me and the destination address is unicast, then receive the packet and complete the communication*/
    int dst = wsm->getRecipientAddress();// destination address
    if(dst == myId)
    {
        DBG<< "#####AppTEST1::receive a packet to me from  the sender" << wsm->getOriginalAddress() << "at the time " << wsm->getTimestamp()<< std::endl;
        delete wsm;
        return;
    }

    /*3.judge the packet whether is a broadcast packet and if the hopLimit greater than 0 and is aready send or not*/
    if(dst == -1)
    {
        broadcastForwardMessage(wsm);
        /*
        int hopLimit = wsm->getHopLimit();
        if(hopLimit < 1)
        {
            DBG<<"##AppTEST1::The packet do not need forward again,receive the packet from the sender: " << wsm->getSenderAddress() << "at the time: "<< wsm->getArrivalTime() << std::endl;
            delete wsm;
            return;
        }
        */
        /*XXX:to do*/
        /*
         * judge the packet whether has been forward yet
         * */
        /*
         int carId = wsm->getOriginalAddress();
        Coord curPos = wsm->getSenderPos();
        simtime_t timeStamp = wsm->getTimestamp();
        int serial = wsm->getSerial();
        */
        /*the packet has forword then dicard the packet */
        /*
        if(!updateCarList(carId,curPos,timeStamp,serial))
        {
            DBG<<"##AppTEST1::the packet has been forwarded then dicard the packet" << std::endl;
            delete wsm;
            return;
        }
        */

        /*4.if the hopLimit greater than 1 and the packet has not been forward yet,then prepare to forward the packet
         * a.first reduce the hoplimit by 1
         * b.insert the service into the provider service list
         * c.broadcast the wsa packet in the cch time slot and contain the service
         * d.send wsm packet in the specified channel
         * */

        //hopLimit = hopLimit - 1;
        /*choose the next service channel*/
        /*
        int sch = selectNextChannel();
        cService *service = new cService(wsm->getPsid(),sch, wsm->getUserPriority(), wsm->getRecipientAddress(), wsm->getFlag(),hopLimit);
        service->setTimestamp(wsm->getTimestamp());
        insertService(&providerServiceList,service);
        */
        /*begin to broadcast the wsa packet*/
        //channelNum = sch;
        //sendWSAMessage(wsm->getWsmData());

        /*after the 50ms then send the wsm message*/
        /*
        if(nextWSMEvent->isScheduled())
        {
            cancelEvent(nextWSMEvent);
        }
        scheduleAt(simTime()+ SWITCHING_INTERVAL_11P + individualOffset,nextWSMEvent);
        */
    }else
    {
        /*the unicast forward */
        unicastForwardMessage(wsm);
    }
}

void AppTest1::sendWSMMessage(std::string blockedRoadId){
    sentWSM = true;
    /*judge the current channel state first,SCH or CCH*/
    t_channel state = getActiveChannel();
    double timeToNextChannel = timeToNextSwitch();
    double offSet = dblrand()*0.005;
    if(state == type_CCH)
    {
        /*if the current channel is CCH,then we send the wsm message in the next SCH time solt*/
        if(nextWSMEvent->isScheduled())
        {
            cancelEvent(nextWSMEvent);
        }
        scheduleAt(simTime() + timeToNextChannel + offSet,nextWSMEvent);
        return;
    }else{/*SCH*/
        if(timeToNextChannel <= 0.005)
        {
            /*the left time slot is not enough to send the wsm message,we send in the next SCH time slot;
             * but we should send the wsa first, to indicate the receiver to receive*/
            if(nextWSAEvent->isScheduled())
            {
                cancelEvent(nextWSAEvent);
            }
            scheduleAt(simTime() + timeToNextChannel + offSet,nextWSAEvent);
            broadcastServiceList.clear();
            return;
        }
    }

    /*begin to send wsm message*/
    /*if the broadcast is not empty,then send the message and update the providerserviceList*/
    if(!broadcastServiceList.empty()){
        cService *service = broadcastServiceList.front();
        WaveShortMessage *wsm = prepareBaseMSG("wsm",dataLengthBits,dataPriority,service->getSch(),carMove.getSpeed(),carMove.getDirection(),service->getTimestamp());
        wsm = prepareWSM(wsm,service->getPsid(),service->getHopLimit(),service->getFlag(),service->getDst(),service->getOriginalSender());
        wsm->setWsmData(blockedRoadId.c_str());
        if(wsm->getOriginalAddress() == myId)
        {
            wsm->setSerial(serial);
        }else
        {
            wsm->setSerial(service->getSerial());
        }
        if(service->getDst() != -1)
        {
            /*unicast the wsm message */
            unicastWSMMessage(wsm);
        }else{
            /*broadcast the wsm message*/
            broadcastWSMMessage(wsm);
        }
        /*delete the servie that have provided,update the providerserviceList */
        broadcastServiceList.pop_front();
        deleteServie(&providerServiceList,service->getSch(),service->getPsid());

        /*send the next wsm message until the sch time is arrived or there is no service to provide*/
        if(nextWSMEvent->isScheduled())
        {
            cancelEvent(nextWSMEvent);
        }
        scheduleAt(simTime()+ 0.010,nextWSMEvent);
        return;
    }else{
        DBG<< "##AppTest1::there is no message to send !!!"<< std::endl;
        return;
    }
}

void AppTest1::sendWSAMessage(std::string blockedRoadId)
{
    sentWSA = true;
    /*judge the current channel state first,SCH or CCH*/
    t_channel state = getActiveChannel();
    double timeToNextChannel = timeToNextSwitch();
    double offSet = dblrand()*0.005;//the 0.005 offset
    if(state == type_SCH)
    {
        /*if the current channel is SCH,then we send the wsa message in the next CCH interval*/
        if(nextWSAEvent->isScheduled())
        {
            cancelEvent(nextWSAEvent);
        }

        scheduleAt(simTime() + timeToNextChannel + offSet,nextWSAEvent);//send the wsa in the next CCH time slot
        broadcastServiceList.clear();//first wsa message then clear the list
        return;
    }else/*type_CCH*/
    {
        if(timeToNextChannel <= 0.005)
        {
            /*the left time slot is not enough to send the wsa message,we send the wsa message in the next CCH time slot*/
            if(nextWSAEvent->isScheduled())
            {
                cancelEvent(nextWSAEvent);
            }
            scheduleAt(simTime() + timeToNextChannel+SWITCHING_INTERVAL_11P+offSet,nextWSAEvent);
            return;
        }
    }

    /*begin to send wsa message*/
    if(!providerServiceList.empty())
    {
        /*select the channel first*/
        channelNum = allocateChannel(providerServiceList);
        if(channelNum != -1)
        {
            map<int,psidService>::iterator it = providerServiceList.find(channelNum);
            if(it != providerServiceList.end())
            {
                if(!(it->second).empty())
                {
                    /*send the first psid service*/
                    map<int,cService*>::iterator iter,iter1;
                    iter = it->second.begin();
                    cService *service = iter->second;
                    WaveShortMessage *wsa = prepareBaseMSG("wsa",dataLengthBits,dataPriority,service->getSch(),carMove.getSpeed(),carMove.getDirection(),service->getTimestamp());
                    wsa = prepareWSA(wsa,service->getPsid(),service->getHopLimit(),service->getFlag(),service->getPriority(),service->getDst());
                    wsa->setWsmData(blockedRoadId.c_str());
                    /*record the broadcasted service */
                    broadcastServiceList.push_back(service);

                    iter++;
                    if(iter != it->second.end())
                    {
                        cService *service1 = iter->second;
                        wsa = prepareWSA2(wsa,service1->getPsid(),service1->getHopLimit(),service1->getFlag(),service1->getPriority(),service1->getSch(),service1->getDst(),service1->getTimestamp());
                        /*record the broadcasted service*/
                        broadcastServiceList.push_back(service1);
                    }

                    /*send other nodes position information, no more than 10*/
                    map<int,CarMove*>::iterator iter2 = carMoveStatus.begin();
                    for(int i = 0;(iter2 != carMoveStatus.end())&&(i<10);iter2++)
                    {
                        CarMove *carmove = iter2->second;
                        wsa->setCarlist(i,*carmove);
                        i++;
                    }

                    /*set the service channel to provide service */
                    //myMac->changeServiceChannel(channelNum);
                    sendWSA(wsa);

                    /*prepare to send wsm message in the next SCH channel time slot*/
                    if(nextWSMEvent->isScheduled())
                    {
                        cancelEvent(nextWSMEvent);
                    }
                    double offset2 = dblrand()*0.0050;
                    scheduleAt(simTime()+ timeToNextChannel + offset2,nextWSMEvent);
                }
            }else
            {
                DBG << "##AppTEST1::can not find the channel service !" << std::endl;
                return;
            }
        }
    }else{
     DBG<<"##AppTest1::there is no service to provide!!" <<std::endl;
     return;
    }
}

void AppTest1::sendWSM(WaveShortMessage* wsm){
    if (isParking && !sendWhileParking) return;
    EV <<"##individaulOffset Time is " << individualOffset << std::endl;
    sendDelayedDown(wsm,individualOffset);
}

void AppTest1::sendWSA(WaveShortMessage* wsa){
    sendDelayedDown(wsa,individualOffset);
}
void AppTest1::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj){
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
    else if (signalID == parkingStateChangedSignal) {
        handleParkingUpdate(obj);
    }
}

void AppTest1::handleParkingUpdate(cObject* obj){
    isParking = traci->getParkingState();
    if (sendWhileParking == false) {
        if (isParking == true) {
            (FindModule<BaseConnectionManager*>::findGlobalModule())->unregisterNic(this->getParentModule()->getSubmodule("nic"));
        }
        else {
            Coord pos = traci->getCurrentPosition();
            (FindModule<BaseConnectionManager*>::findGlobalModule())->registerNic(this->getParentModule()->getSubmodule("nic"), (ChannelAccess*) this->getParentModule()->getSubmodule("nic")->getSubmodule("phy80211p"), &pos);
        }
    }
}

void AppTest1::handlePositionUpdate(cObject* obj){
    WaveAppLayer::handlePositionUpdate(obj);
    /*update the speed and position and dir message */
    carMove.setSpeed(traci->getSpeed());
    if(!FWMath::close((traci->getCurrentPosition() - carMove.getDirection()).length(),0.0))
    {
        carMove.setDirectionByTarget(traci->getCurrentPosition());
    }
    carMove.setStart(traci->getCurrentPosition(),simTime());


    DBG<<"the direction of the car angle:"<<traci->getAngleRad() << std::endl;
    // stopped for for at least 10s?
    if (traci->getSpeed() < 1) {
        if (simTime() - lastDroveAt >= 10) {
            findHost()->getDisplayString().updateWith("r=16,red");
            if (!sentWSM) sendWSMMessage(traci->getRoadId());
        }
    }
    else {
            lastDroveAt = simTime();
    }
}

int AppTest1::allocateChannel(ServiceListType &serviceList){
    if(serviceList.empty())
    {
        DBG << "##AppTEST1::The service list is empty,no service to provide !" << std::endl;
        return -1;
    }
    map<int,float> channelWeight;
    updateChannelWeight(serviceList,channelWeight);
    int chan = findMaxWeight(channelWeight);
    return chan;
}

void AppTest1::updateChannelWeight(ServiceListType &serviceList,ChannelWeightType &channelWeight)
{
    map<int,cService*>::iterator cIter;
    map<int,psidService>::iterator pIter;
    int base;
    // find the channel first
    for(pIter = serviceList.begin(); pIter != serviceList.end(); pIter++)
    {
        float weight = 0.0;
        //find the service in channel and calculate the weight
        for(cIter = (pIter->second).begin(); cIter != pIter->second.end(); cIter++)
        {
            //mandatory forward base = 10
            if(cIter->second->getFlag() == 0)
            {
                base = 10;
            }else{ //optional forward and dont forward base =5
                base = 5;
            }

            weight += base*0.8 + 0.2*cIter->second->getPriority();
        }
        channelWeight[pIter->first] = weight;
    }
}

int AppTest1::findMaxWeight(ChannelWeightType channelWeight)
{
    // the map is empty and return -1
    if(channelWeight.empty())
    {
        return -1;
    }

    map<int,float>::iterator iter,temp;

    //store the max weight of iterator
    temp = channelWeight.begin();
    for(iter = channelWeight.begin(); iter != channelWeight.end(); iter++)
    {
        if(iter->second >= temp->second)
        {
            temp = iter;
        }
    }
    return (temp->first);
}

cService* AppTest1::findCommonService(ServiceListType &aServiceList,psidService &sServiceList)
{
    if(sServiceList.empty() || aServiceList.empty())
        return NULL;

    map<int,cService*>::iterator iter,it;
    map<int,psidService>::iterator pIter;
    for(iter = sServiceList.begin(); iter != sServiceList.end(); iter++)
    {
        for(pIter = aServiceList.begin(); pIter != aServiceList.end(); pIter++)
        {
            it = pIter->second.find(iter->first);
            //find out the same psid service
            if(it != pIter->second.end())
            {
                return it->second;
            }
        }
    }
    return NULL;
}

void AppTest1::handleSelfMsg(cMessage* msg)
{
    if(msg == nextWSAEvent)
    {
        sendWSAMessage(traci->getRoadId());
    }else if(msg == nextWSMEvent)
    {
        sendWSMMessage(traci->getRoadId());
    }else if(msg == updatePOSEvent)
    {
        scheduleAt(simTime() + 5.0,updatePOSEvent);
        updateCarStatusList();
        WaveShortMessage *wsa = prepareBaseMSG("wsa",dataLengthBits,dataPriority,178,carMove.getSpeed(),carMove.getDirection(),simTime());
        wsa = prepareWSA(wsa,0,0,0,0,-1);
        wsa->setWsmData("update position information");
        wsa = prepareWSA2(wsa,0,0,0,0,178,-1,simTime());
        /*send other car position information*/
        map<int,CarMove*>::iterator iter = carMoveStatus.begin();
        for(int i = 0; iter != carMoveStatus.end()&& i < 10;iter++)
        {
            CarMove *carmove = iter->second;
            wsa->setCarlist(i,*carmove);
            i++;
        }

        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        double offSet = dblrand()*0.005 + 0.01;
        if(state == type_SCH){
            sendDelayedDown(wsa,timeToNextChannel + offSet);
        }else{/*CCH*/
            if(timeToNextChannel <= 0.005){
                /*next CCH channel slot send the message*/
                sendDelayedDown(wsa,timeToNextChannel+SWITCHING_INTERVAL_11P+offSet);
            }else
            {
                sendWSA(wsa);
            }
        }
    }else if(msg == timeEvent)
    {
        switch(msg->getKind()){
             case SEND_UNICAST_EVENT:{
                 if(cycleTime > 0){
                     int psid = 99;
                     int priority = 6;
                     int receiveId = 7;
                     int hopLimit = 5;
                     int sch = selectNextChannel();
                     cService *service = new cService(psid,sch,priority,receiveId,0,hopLimit,myId,1);
                     service->setTimestamp(simTime());
                     insertService(&providerServiceList,service);
                     sendWSAMessage("hello world");
                     cycleTime -= 0.025;
                     break;
                 }
             }
             default:{
                 if(msg){
                     DBG  << "##AppTEST1::App:Error:Got Self Message of unknow kind!Name:"<<msg->getName()<< std::endl;
                 }
                 break;
             }
        }
    }
}

void AppTest1::insertService(ServiceListType *list,cService *service)
{
    int psid = service->getPsid();
    int sch = service->getSch();
    map<int,map<int,cService*> >::iterator it = list->find(sch);
    /*find out if the channel is aready exist */
    if(it != list->end())
    {
        /*aready exist,insert the service in the related channnel*/
        it->second.insert(make_pair(psid,service));
    }else
    {
        /*not exist,then insert the channel and service*/
        psidService pService;
        pService.insert(make_pair(psid,service));
        list->insert(make_pair(sch,pService));
    }
}

void AppTest1::deleteServie(ServiceListType *list,int channel,int psid){
    if(list->empty())
    {
        DBG << "##AppTEST1::ERROR:delete an empty servicelist!" << std::endl;
        return;
    }

    map<int,map<int,cService*> >::iterator it = list->find(channel);
    /*find the channel*/
    if(it != list->end())
    {
        map<int,cService*>::iterator iter = it->second.find(psid);
        if(iter != it->second.end())
        {
            /*we must delete the content of the pointer direct and then delete the iterator */
            delete (iter->second);
            it->second.erase(iter);
            /*after delete the service,you need check the channel whether has other service,if not we need the channel*/
            if(it->second.empty())
            {
                list->erase(it);
            }
        }else
        {
            DBG << "##AppTEST1::ERROR: no such service!" << std::endl;
            return;
        }
    }else
    {
        DBG << "##AppTEST1::ERROR:no such channel!" << std::endl;
        return;
    }
}

bool AppTest1::updateCarList(int id, Coord &curpos, simtime_t timestamp,int serial){
    /*first exclude itself,we could not forward the packet that itself create */
    if(id == myId)
    {
        DBG<<"##AppTest1::the packet is myself product,drop it!"<<std::endl;
        return false;
    }

    /*find the car whether exist in the carlist first*/
    map<int,Car*>::iterator it = carList.find(id);
    //find
    if(it != carList.end())
    {
        Car *car = it->second;
        bool test = car->test_bitset(serial);
        if(test){
            return false;
        }else
        {
            car->setCurPos(curpos);
            car->setTimeStamp(timestamp);
            car->set_bitset(serial,1);
        }
    }else
    {
        //not find
        Car *car = new Car(id,1024);
        car->setCurPos(curpos);
        car->setTimeStamp(timestamp);
        car->set_bitset(serial,1);
        carList.insert(make_pair(id,car));
    }
    return true;
}

void AppTest1::updateCarStatusList()
{
    if(!carMoveStatus.empty())
    {
        map<int,CarMove*>::iterator iter = carMoveStatus.begin();
        while(iter != carMoveStatus.end())
        {
            simtime_t startTime = iter->second->getStartTime();
            simtime_t currentTime = simTime();
            if(currentTime - startTime >= 30.0)
            {
                delete (iter->second);//callback the memory
                carMoveStatus.erase(iter);
            }
            ++iter;
        }
    }
}

double AppTest1::distance(const Coord& a,const Coord& b){
    return a.distance(b);
}

bool AppTest1::isSameDirection(const Coord& dir1,const Coord& dir2){
    double arc1 = radians(dir1);
    double arc2 = radians(dir2);
    double diff = abs(arc1 - arc2);
    /*two direction angle is diff than 90 degree,so they are not in the same direction*/
    if(diff >= M_PI)
    {
        return false;
    }else{
        return true;
    }
}

double AppTest1::radians(const Coord& direction)
{
    double x = direction.x;
    double y = direction.y;
    double arc = 0.0;
    if(x>0)
    {
        arc = asin(y);
    }else
    {
        arc = (y/abs(y))*(M_PI - asin(y));
    }
    return arc;
}

int AppTest1::selectNextCar(int dest)
{
    int carId = -1;
    map<int,CarMove*>::iterator iter,iter1;
    iter = carMoveStatus.find(dest);
    if(iter != carMoveStatus.end())
    {
        CarMove *carmove = iter->second;
        Coord lastPos = carmove->getPositionAt(simTime());
        Coord curPos = traci->getCurrentPosition();
        double dist = distance(curPos,lastPos);
        /*the distance is less than 500 meters then they are in the communication range each other*/
        if(dist <= 500){
            return dest;
        }

        /*store the nearest distance between the forward car and the destination car*/
        double nearestDist = dist;
        for(iter1 = carMoveStatus.begin(); iter1 != carMoveStatus.end(); iter1++)
        {
            /*calculate the position first,then calculate the distance*/
            CarMove *carmove1 = iter1->second;
            Coord lastPos1 = carmove1->getPositionAt(simTime());
            double dist1 = distance(curPos,lastPos1);
            /*choose the car in the communication and leave the destination nearest*/
            if(dist1 <= 500)
            {
                double dist2 = distance(lastPos,lastPos1);
                if(dist2 < nearestDist)
                {
                    nearestDist = dist2;
                    carId = iter1->first;
                }
            }
        }
    }
    //not find,return -1 represent error
    return carId;
}

void AppTest1::unicastForwardMessage(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    int hopLimit = wsm->getHopLimit();
    if(hopLimit < 1)
    {
        DBG<<"###The packet can not forward again,just drop it!"<<std::endl;
        delete wsm;
        return;
    }
    int destId = wsm->getRecipientAddress();
    int forwardId = selectNextCar(destId);
    if(forwardId != -1)
    {
        //wsm->setNextForwardAddress(forwardId);
        hopLimit = hopLimit -1;
        //wsm->setHopLimit(hopLimit);
        /*choose the next service channel*/
        int sch = selectNextChannel();
        cService *service = new cService(wsm->getPsid(),sch,wsm->getUserPriority(),wsm->getRecipientAddress(),wsm->getFlag(),hopLimit,wsm->getOriginalAddress(),wsm->getSerial());
        service->setTimestamp(wsm->getTimestamp());
        insertService(&providerServiceList,service);
        //channelNum = sch;
        double timeToNextChannel = timeToNextSwitch();
        double offSet = dblrand()*0.005;
        scheduleAt(simTime()+ timeToNextChannel + offSet,nextWSAEvent);
        //sendWSAMessage(wsm->getWsmData());
        //double offsetTime = dblrand()*0.020 + 0.050;
        //wsm->setChannelNumber(channelNum);
        //sendDelayedDown(wsm,offsetTime);
        DBG<<"##The dest:"<<destId <<"'s packet will forward by " << forwardId <<" with the hoplimit: "<< hopLimit << std::endl;
        return;
    }
    DBG<<"## Not find the forward car Id!"<< std::endl;
    delete wsm;
    return;
}

void AppTest1::broadcastForwardMessage(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    int hopLimit = wsm->getHopLimit();
    if(hopLimit < 1){
        DBG<< "##AppTest1:: The broadcast packet do not need forward again,receive the packet from the sender: " << wsm->getSenderAddress() << "at the time: "<< wsm->getArrivalTime() << std::endl;
        delete wsm;
        return;
    }
    /* judge the packet whether has been forward yet*/
    int carId = wsm->getOriginalAddress();
    Coord curPos = wsm->getSenderPos();
    simtime_t timeStamp = wsm->getTimestamp();
    int serial = wsm->getSerial();

    /*the packet has forword then dicard the packet */
    if(!updateCarList(carId,curPos,timeStamp,serial)){
        DBG<<"##AppTEST1::the packet has been forwarded then dicard the packet" << std::endl;
        delete wsm;
        return;
    }

    /*if the hopLimit greater than 1 and the packet has not been forward yet,then prepare to forward the packet
     * * a.first reduce the hoplimit by 1
     * * b.insert the service into the provider service list
     * * c.broadcast the wsa packet in the cch time slot and contain the service
     * * d.send wsm packet in the specified channel
     */

    hopLimit = hopLimit - 1;

    /*choose the next service channel*/
    int sch = selectNextChannel();
    cService *service = new cService(wsm->getPsid(),sch, wsm->getUserPriority(), wsm->getRecipientAddress(), wsm->getFlag(),hopLimit,wsm->getOriginalAddress(),wsm->getSerial());
    service->setTimestamp(wsm->getTimestamp());
    insertService(&providerServiceList,service);

    /*begin to broadcast the wsa packet*/
    double timeToNextChannel = timeToNextSwitch();
    double offSet = dblrand()*0.005;
    scheduleAt(simTime()+ timeToNextChannel + offSet,nextWSAEvent);
    //sendWSAMessage(wsm->getWsmData());
}

void AppTest1::unicastWSMMessage(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    int destId = wsm->getRecipientAddress();
    int forwardId = selectNextCar(destId);
    if(forwardId != -1)
    {
        /*update the next forward address*/
        wsm->setNextForwardAddress(forwardId);
        wsm->setSerial(0);
        DBG<<"###unicast the packet with hoplimit:"<< wsm->getHopLimit() << "to the destination: " << wsm->getRecipientAddress() << "the forward node is:" << wsm->getNextForwardAddress() << std::endl;
        sendWSM(wsm);
        return;
    }
    DBG<<"## Not find the forward car Id!"<< std::endl;
    delete wsm;
    return;
}

void AppTest1::broadcastWSMMessage(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    if(wsm->getOriginalAddress() == myId)
    {
        serial++;
    }
    DBG<<"###broadcast packet with hoplimit: "<< wsm->getHopLimit() << std::endl;
    sendWSM(wsm);
}

int AppTest1::selectNextChannel(){
    /*choose the next service channel*/
    int sch = allocateChannel(providerServiceList);
    // no service in the providerlist then we random choose a service channel
    if(sch == -1)
    {
        int random = intuniform(1,4);
        switch (random) {
        case 1:
            sch = Channels::SCH1;
            break;
        case 2:
            sch = Channels::SCH2;
            break;
        case 3:
            sch = Channels::SCH3;
            break;
        default:
            sch = Channels::SCH4;
            break;
        }
    }
    return sch;
}

t_channel AppTest1::getActiveChannel(){
      uint64_t currentTime = simTime().raw();
      uint64_t swithingTime = SWITCHING_INTERVAL_11P.raw();
      if((currentTime/swithingTime)%2 == 0)
      {
          return type_CCH;
      }else{
          return type_SCH;
      }
}

double AppTest1::timeToNextSwitch(){
    uint64_t currentTime = simTime().raw();
    uint64_t switchingTime = SWITCHING_INTERVAL_11P.raw();
    double timetoNextSwitch = (double)(switchingTime - (currentTime % switchingTime))/simTime().getScale();
    return timetoNextSwitch;
}

AppTest1::AppTest1() {
    // TODO Auto-generated constructor stub

}

AppTest1::~AppTest1() {
    // TODO Auto-generated destructor stub
}
