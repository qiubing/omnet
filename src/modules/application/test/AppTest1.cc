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

        /*register the record signal*/
        hopLimitSignal = registerSignal("hopLimit");
        delaySignal = registerSignal("delay");
        distanceSignal = registerSignal("distance");

        sentWSM = par("sentWSM").boolValue();
        sentWSA = par("sentWSA").boolValue();
        lastDroveAt = simTime();
        findHost()->subscribe(parkingStateChangedSignal,this);
        sendWhileParking = par("sendWhileParking").boolValue();
        isParking = false;
        isProvider = true;
        channelNum = -1;
        serial = 1;
        posFlag = false;
        positionRoute = par("positionRoute").boolValue();//choose route table without car position
        carId = par("carId").longValue();
        positionUpdateInterval = par("positionUpdateInterval").doubleValue();
        routeTableUpdateInterval = par("routeTableUpdateInterval").doubleValue();
        serviceUpdateInterval = par("serviceUpdateInterval").doubleValue();
        WSAInterval = par("WSAInterval").doubleValue();
        sendUnicastWSMInterval = par("sendUnicastWSMInterval").doubleValue();
        sendPositionTableInterval = par("sendPositionTableInterval").doubleValue();
        sendRouteTableInterval = par("sendRouteTableInterval").doubleValue();

        /*get the car move status*/
        carMove.setCarId(myId);
        carMove.setSpeed(traci->getSpeed());
        carMove.setStart(traci->getCurrentPosition(),simTime());
        carMove.setDirectionByVector(Coord::ZERO);

        nextWSAEvent = new cMessage("next WSA Event");
        nextWSMEvent = new cMessage("next WSM Event");
        updatePOSEvent = new cMessage("next update position Event");
        updateRouteTableEvent = new cMessage("next update route information Event");
        updateServiceEvent = new cMessage("next update available service Event");
        repeatSendWSAEvent = new cMessage("repeat send wsa message Event");
        updateCarNeighborListEvent = new cMessage("next update car neighbor list");

        //simulate asynchronous channel access
        double maxOffset = par("maxOffset").doubleValue();
        double offSet = dblrand()*0.01;//[0,0.025)
        individualOffset = dblrand() * maxOffset;
        /*statistics the send times*/
        sendCount = 0;

        /*route entry sequence*/
        routeSeq = 0;

        //stats
        statsReceivedBroadcasts = 0;
        statsReceivedUnicast = 0;
        statsSendBroadcast = 0;
        statsSendUnicast = 0;

        /*the carId 0 send message to the carId 20 */
        //carId change from 0 to 7,different value simulate different scene
        if(myId == carId){
            unicastEvent = new cMessage("time Event",SEND_UNICAST_EVENT);
            unicastEvent->setSrcProcId(myId);
            double offsetTime = dblrand()*0.025 + 0.015;
            double startTime = offsetTime;
            switch (carId) {
                case 0:

                    //startTime += 212.0;
                    startTime += 141.0;
                    //startTime += 116.0;
                    //startTime += 90.0;
                    //startTime += 132.0;
                    //startTime += 148.0;
                    //startTime += 140.0;
                    break;
                case 1:

                    //startTime += 196.0;
                    startTime += 130.0;
                    //startTime += 108.0;
                    //startTime += 88.0;
                    //startTime += 125.0;
                    //startTime += 136.0;
                    //startTime += 128.0;
                    break;
                case 2:

                    //startTime += 180.0;
                    startTime += 120.0;
                    //startTime += 100.0;
                    //startTime += 80.0;
                    //startTime += 118.0;
                    //startTime += 124.0;
                    //startTime += 116.0;
                    break;
                case 3:

                    //startTime += 164.0;
                    startTime += 110.0;
                    //startTime += 92.0;
                    //startTime += 76.0;
                    //startTime += 111.0;
                    //startTime += 112.0;
                    //startTime += 104.0;
                    break;
                case 4:
                    //startTime += 148.0;
                    startTime += 100.0;
                    //startTime += 84.0;
                    //startTime += 68.0;
                    //startTime += 104.0;
                    //startTime += 100.0;
                    //startTime += 96.0;
                    break;
                case 5:

                    //startTime += 135.0;
                    startTime += 90.0;
                    //startTime += 76.0;
                    //startTime += 64.0;
                    //startTime += 97.0;
                    //startTime += 88.0;
                    //startTime += 84.0;
                    break;
                case 6:

                    //startTime += 116.0;
                    startTime += 80.0;
                    //startTime += 68.0;
                    //startTime += 56.0;
                    //startTime += 90.0;
                    //startTime += 76.0;
                    //startTime += 72.0;
                    break;
                case 7:

                    //startTime += 100.0;//speed 20km/h 1/5s
                    startTime += 70.0; //speed 30km/h 1/5s
                    //startTime += 60.0;//speed 40km/h  1/5s
                    //startTime += 50.0;//speed 50km/h  1/5s
                    //startTime += 83.0;//speed 30km/h  1/7s
                    //startTime += 64.0;//speed 30km/h  1/4s
                    //startTime += 58.0;//speed 33.6km/h  1/3s
                    break;
                default:
                    startTime += 60.0;
                    break;
            }
            scheduleAt(startTime,unicastEvent);
        }


        /*the carId 10 send the broadcast packets at the 120s*/
        /*
        if(myId == 10){
            broadcastEvent = new cMessage("broadcast Event",SEND_BROADCAST_EVENT);
            broadcastEvent->setSrcProcId(myId);
            scheduleAt(120.00+individualOffset,broadcastEvent);
        }
        */

        DBG<<"##WaveAppLayer: the offSet time is: "<< offSet << "and the individualOffset time is: "<< individualOffset << std::endl;
        //initialize the provider service list

        availableServiceList.clear();
        subscribeServiceList.clear();
        carList.clear();
        carMoveStatus.clear();
        routeTable.clear();
        neighborCarList.clear();


        if(isProvider && !sentWSA){
            findHost()->getDisplayString().updateWith("r=16,red");
            scheduleAt(simTime() + 2.0 + offSet,repeatSendWSAEvent);
            isProvider = false;
        }


        /*the update interval of the position or route table information is 5s */
        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        double offset1 =  dblrand()*0.020;
        double offset2 = 0.01 + dblrand()*0.015;
        double randomOffset = timeToNextChannel + 5.0 + intuniform(0,9)*0.10;
        double randomOffset1 = timeToNextChannel + 3.0 + intuniform(0,9)*0.10;
        double randomOffset2 = timeToNextChannel + 4.0 + intuniform(0,9)*0.10;
        if(state == type_SCH){
            randomOffset += offset1;
            randomOffset1 -= offset2;
        }else{
            randomOffset -= offset2;
            randomOffset1 += offset1;
        }

        if(positionRoute){
            scheduleAt(simTime() + randomOffset,updatePOSEvent);
        }else{
            scheduleAt(simTime() + randomOffset,updateRouteTableEvent);
        }
        scheduleAt(simTime() + randomOffset1,updateServiceEvent);
        scheduleAt(simTime() + randomOffset2,updateCarNeighborListEvent);
    }
}
void AppTest1::finish(){
    if(nextWSAEvent->isScheduled()){
        cancelAndDelete(nextWSAEvent);
    }else
    {
        delete nextWSAEvent;
    }

    if(nextWSMEvent->isScheduled()){
        cancelAndDelete(nextWSMEvent);
    }else
    {
        delete nextWSMEvent;
    }
    if(updatePOSEvent->isScheduled()){
        cancelAndDelete(updatePOSEvent);
    }else
    {
        delete updatePOSEvent;
    }

    //stats
    recordScalar("ReceivedUnicasts",statsReceivedUnicast);
    recordScalar("ReceivedBroadcasts",statsReceivedBroadcasts);
    recordScalar("SentUnicasts",statsSendUnicast);
    recordScalar("SentBroadcasts",statsSendBroadcast);
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
        service->setExpire(wsm->getExpireTime1());
        service->setExpireTime(simTime()+wsm->getExpireTime1());
        insertService(availableServiceList,service);
    }

    if(wsm->getPsid2() != 0)
    {
        cService *service2 = new cService(wsm->getPsid2(),wsm->getSch2(),wsm->getUserPriority2(),wsm->getDst2(),wsm->getFlag2(),wsm->getHopLimit2(),wsm->getOriginalAddress(),wsm->getSerial());
        service2->setTimestamp(wsm->getTimeStamp2());
        service2->setExpire(wsm->getExpireTime2());
        service2->setExpireTime(simTime()+wsm->getExpireTime2());
        insertService(availableServiceList,service2);
    }

    if(positionRoute){
        /*update other car status*/
        CarMove *carstatus = new CarMove(wsm->getSenderAddress());
        carstatus->setDirectionByVector(wsm->getDirection());
        carstatus->setSpeed(wsm->getSpeed());
        carstatus->setStart(wsm->getSenderPos(),wsm->getTimestamp());
        double lifetime = calculateLifetime(carMove,*carstatus);
        carstatus->setLifetime(lifetime);//set lifetime
        carstatus->setFlag(0);//good flag
        carMoveStatus[wsm->getSenderAddress()] = carstatus;
        neighborCarList[wsm->getSenderAddress()] = carstatus;//save the neighbor car move status

        for(int i = 0; i < 10;i++)//20
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
            }else{
                break;
            }
        }
    }else{
        /*route without car position information*/
        RouteEntry *entry = new RouteEntry(wsm->getSenderAddress());
        //entry->setNextCarId(-1);//my neighbors
        entry->setNextCarId(wsm->getSenderAddress());
        entry->setDistance(1);
        entry->setTimestamp(wsm->getArrivalTime());
        entry->setSeq(wsm->getSerial());
        routeTable[wsm->getSenderAddress()] = entry;
        for(int i = 0; i < 10;i++)
        {
            int carId = wsm->getRouteTable(i).getCarId();
            if(carId != -1){
                if(carId == myId)
                {
                    /*itself skip*/
                    continue;
                }
                map<int,RouteEntry*>::iterator it = routeTable.find(carId);
                if(it != routeTable.end()){
                    /*compare the route sequence first*/
                    int oldSeq = it->second->getSeq();
                    int newSeq = wsm->getRouteTable(i).getSeq();
                    if(newSeq < oldSeq){
                        /*ignore this route entry update*/
                        continue;
                    }
                    int distance1 = it->second->getDistance();
                    int distance2 = wsm->getRouteTable(i).getDistance() + 1;//distance add by 1
                    int nextCarId1 = it->second->getNextCarId();
                    int nextCarId2 = wsm->getSenderAddress();
                    if(nextCarId1 != nextCarId2){//next car id is different
                        //select the small distance one
                        if(distance1 < distance2){
                            continue;
                        }else{
                            RouteEntry *entry2 = it->second;
                            entry2->setDistance(distance2);
                            entry2->setNextCarId(nextCarId2);
                            entry2->setSeq(newSeq);
                            entry2->setTimestamp(wsm->getArrivalTime());
                        }
                    }else{//next car id is same,select the new route entry
                        RouteEntry *entry1 = it->second;
                        entry1->setDistance(distance2);
                        entry1->setNextCarId(nextCarId2);
                        entry1->setSeq(newSeq);
                        entry1->setTimestamp(wsm->getArrivalTime());
                    }
                }else{//add new route entry
                    RouteEntry &temp = wsm->getRouteTable(i);
                    RouteEntry *entry2 = new RouteEntry(carId);
                    entry2->setDistance(temp.getDistance()+1);
                    entry2->setNextCarId(wsm->getSenderAddress());
                    entry2->setTimestamp(wsm->getArrivalTime());
                    entry2->setSeq(temp.getSeq());
                    routeTable[carId] = entry2;
                }

            }else{
                break;
            }
        }//for
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
    deleteServie(availableServiceList,channel,psid);

    /*2.judge the packet is to me.if the packet is to me and the destination address is unicast, then receive the packet and complete the communication*/
    int dst = wsm->getRecipientAddress();// destination address
    if(dst == myId)
    {
        /*received message to me then emit the signal to listener*/
        hopCount = 9 - wsm->getHopLimit() + 1;
        sender = wsm->getOriginalAddress();
        double time = simTime().dbl() - wsm->getTimestamp().dbl();
        double dist = distance(curPosition,wsm->getSenderPos());
        DBG<< "#####AppTEST1::receive a packet to me from  the sender" << wsm->getOriginalAddress() << "at the time " << wsm->getTimestamp()<< std::endl;
        if(ev.isGUI()){
            //updateDisplay();
            char msgname[40];
            sprintf(msgname,"from %d to %d with %d hopLimit take %f s",wsm->getOriginalAddress(),wsm->getRecipientAddress(),hopCount,time);
            findHost()->bubble(msgname);
            findHost()->getDisplayString().setTagArg("t",0,msgname);
        }
        if(wsm->getPsid() >= 90){
            statsReceivedUnicast++;
            //send a signal
            emit(hopLimitSignal,hopCount);
            emit(delaySignal,time);
            emit(distanceSignal,dist);
        }
        delete wsm;
        return;
    }

    /*3.judge the packet whether is a broadcast packet and if the hopLimit greater than 0 and is aready send or not*/
    if(dst == -1)
    {
        /*broadcast forward*/
        broadcastForwardMessage(wsm);
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
    if(state == type_CCH){
        /*if the current channel is CCH,then we send the wsm message in the next SCH time solt*/
        if(nextWSMEvent->isScheduled())
        {
            cancelEvent(nextWSMEvent);
        }
        scheduleAt(simTime() + timeToNextChannel + offSet,nextWSMEvent);
        broadcastServiceList.clear();
        return;
    }else{/*SCH*/
        if(timeToNextChannel <= 0.001)
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
        int sch = service->getSch();
        ASSERT(sch >= 174);
        ASSERT(sch <= 182);
        WaveShortMessage *wsm = prepareBaseMSG("wsm",dataLengthBits,dataPriority,service->getSch(),carMove.getSpeed(),carMove.getDirection(),service->getTimestamp());
        wsm = prepareWSM(wsm,service->getPsid(),service->getHopLimit(),service->getFlag(),service->getDst(),service->getOriginalSender(),service->getPriority());
        wsm->setExpireTime1(service->getExpire());
        wsm->setSenderPos(service->getPositions());
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
        /*delete the servie that have provided,update the providerserviceList for the unicast*/
        if(service->getDst() == -1 && service->getPsid() >50){
            double offset = 0.0006;
            scheduleAt(simTime()+offset,nextWSMEvent);
        }else{//repeat send broadcast packets
            broadcastServiceList.pop_front();
            deleteServie(providerServiceList,service->getSch(),service->getPsid());
            /*send the next wsm message until the sch time is arrived or there is no service to provide*/

            if(!broadcastServiceList.empty()){
                if(nextWSMEvent->isScheduled()){
                    cancelEvent(nextWSMEvent);
                }
                scheduleAt(simTime()+ 0.010,nextWSMEvent);
            }
        }
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
        if(timeToNextChannel <= 0.001)
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
                    wsa->setExpireTime1(service->getExpire());
                    wsa->setWsmData(blockedRoadId.c_str());
                    wsa->setSerial(routeSeq);//set the send route entry sequence
                    routeSeq++;
                    /*record the broadcasted service */
                    broadcastServiceList.push_back(service);

                    iter++;
                    if(iter != it->second.end())
                    {
                        cService *service1 = iter->second;
                        wsa = prepareWSA2(wsa,service1->getPsid(),service1->getHopLimit(),service1->getFlag(),service1->getPriority(),service1->getSch(),service1->getDst(),service1->getTimestamp());
                        wsa->setExpireTime2(service1->getExpire());
                        /*record the broadcasted service*/
                        broadcastServiceList.push_back(service1);
                    }

                    if(positionRoute){
                        /*send other nodes position information, no more than 10*/
                        /*fix me: we should send the position information randomly*/
                        if(!posFlag){//first 10 car position information
                            map<int,CarMove*>::iterator iter2 = carMoveStatus.begin();
                            for(int i = 0;(iter2 != carMoveStatus.end())&&(i<10);iter2++){//20
                                CarMove *carmove = iter2->second;
                                wsa->setCarlist(i,*carmove);
                                i++;
                            }
                        }else{//last 10 car position information
                            map<int,CarMove*>::reverse_iterator iter3 = carMoveStatus.rbegin();
                            for(int i = 0;(iter3 != carMoveStatus.rend())&&(i<10);iter3++){//20
                                CarMove *carmove = iter3->second;
                                wsa->setCarlist(i,*carmove);
                                i++;
                            }
                        }
                     }else{
                         //send route table information to other nodes
                         if(!posFlag){//send first car route information
                             map<int,RouteEntry*>::iterator iter4 = routeTable.begin();
                             for(int i = 0;(iter4 != routeTable.end())&&(i<10);iter4++){
                                 RouteEntry *entry = iter4->second;
                                 wsa->setRouteTable(i,*entry);
                                 i++;
                             }
                         }else{// send last 10 route information
                             map<int,RouteEntry*>::reverse_iterator iter5 = routeTable.rbegin();
                             for(int i = 0;(iter5 != routeTable.rend())&&(i<10);iter5++){
                                 RouteEntry *entry = iter5->second;
                                 wsa->setRouteTable(i,*entry);
                                 i++;
                             }
                         }
                     }

                    posFlag = (!posFlag);//turn around
                    sendWSA(wsa);

                    /*prepare to send wsm message in the next SCH channel time slot*/
                    if(nextWSMEvent->isScheduled())
                    {
                        cancelEvent(nextWSMEvent);
                    }
                    double offset2 = dblrand()*0.0050;
                    scheduleAt(simTime()+ timeToNextChannel + offset2,nextWSMEvent);
                }
            }else{
                DBG << "##AppTEST1::can not find the channel service !" << std::endl;
                return;
            }
        }
    }else{
        DBG<<"##AppTest1::there is no service to provide!!" <<std::endl;
        return;
    }

}
void AppTest1::sendWSAMessage(std::string blockedRoadId,cService *service, int flag){
    ASSERT(service);
    int sch = service->getSch();
    WaveShortMessage *wsa = prepareBaseMSG("wsa",dataLengthBits,dataPriority,sch,carMove.getSpeed(),carMove.getDirection(),service->getTimestamp());
    wsa = prepareWSA(wsa,service->getPsid(),service->getHopLimit(),service->getFlag(),service->getPriority(),service->getDst());
    wsa->setExpireTime1(service->getExpire());
    wsa->setWsmData(blockedRoadId.c_str());
    wsa->setSerial(routeSeq);
    routeSeq++;
    broadcastServiceList.push_back(service);
    if(positionRoute){
        /*send other nodes position information, no more than 10*/
        /*fix me: we should send the position information randomly*/
        if(!posFlag){//first 10 car position information
            map<int,CarMove*>::iterator iter2 = carMoveStatus.begin();
            for(int i = 0;(iter2 != carMoveStatus.end())&&(i<10);iter2++){//20
                CarMove *carmove = iter2->second;
                wsa->setCarlist(i,*carmove);
                i++;
            }
        }else{//last 10 car position information
            map<int,CarMove*>::reverse_iterator iter3 = carMoveStatus.rbegin();
            for(int i = 0;(iter3 != carMoveStatus.rend())&&(i<10);iter3++){//20
                CarMove *carmove = iter3->second;
                wsa->setCarlist(i,*carmove);
                i++;
            }
        }
    }else{
        //send route table information to other nodes
        if(!posFlag){//send first car route information
            map<int,RouteEntry*>::iterator iter4 = routeTable.begin();
            for(int i = 0;(iter4 != routeTable.end())&&(i<10);iter4++){
                RouteEntry *entry = iter4->second;
                wsa->setRouteTable(i,*entry);
                i++;
            }
        }else{// send last 10 route information
            map<int,RouteEntry*>::reverse_iterator iter5 = routeTable.rbegin();
            for(int i = 0;(iter5 != routeTable.rend())&&(i<10);iter5++){
                RouteEntry *entry = iter5->second;
                wsa->setRouteTable(i,*entry);
                i++;
            }
        }
    }
    posFlag = (!posFlag);//turn around

    t_channel state = getActiveChannel();
    double timeToNextChannel = timeToNextSwitch();
    double offSet = dblrand()*0.025;//the 0.005 offset
    if(state == type_SCH){
        double delaytime = timeToNextChannel + offSet;
        if(flag == 1){
            delaytime += 0.015;
        }
        sendDelayedDown(wsa,delaytime);
        if(nextWSMEvent->isScheduled()){
            cancelEvent(nextWSMEvent);
        }
        scheduleAt(simTime()+delaytime + SWITCHING_INTERVAL_11P,nextWSMEvent);
    }else{
        if(timeToNextChannel <= 0.025){
            double delaytime = timeToNextChannel + offSet + 0.05;
            if(flag == 1){
                delaytime = individualOffset.dbl();
            }
            sendDelayedDown(wsa,delaytime);
            if(nextWSMEvent->isScheduled()){
                cancelEvent(nextWSMEvent);
            }
            scheduleAt(simTime()+delaytime + SWITCHING_INTERVAL_11P,nextWSMEvent);
        }else{
            double delaytime = individualOffset.dbl();
            if(flag == 1){
                delaytime += 0.015;
            }
            sendDelayedDown(wsa,delaytime);
            if(nextWSMEvent->isScheduled()){
                cancelEvent(nextWSMEvent);
            }
            scheduleAt(simTime() + delaytime + SWITCHING_INTERVAL_11P,nextWSMEvent);
        }
    }
}
void AppTest1::sendWSM(WaveShortMessage* wsm){
    if (isParking && !sendWhileParking) return;
    EV <<"##individaulOffset Time is " << individualOffset << std::endl;
    if((wsm->getChannelNumber() >= 174) && (wsm->getChannelNumber() <= 182)){
    sendDelayedDown(wsm,individualOffset);
    }
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
        for(cIter = pIter->second.begin(); cIter != pIter->second.end(); cIter++)
        {
            //mandatory forward base = 10
            if(cIter->second->getFlag() == 0)
            {
                base = 10;
            }else{ //optional forward and dont forward base =5
                base = 5;
            }
            int priority = cIter->second->getPriority();
            weight += base*0.8 + 0.2*priority;
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
    if(msg == nextWSAEvent){
        sendWSAMessage(traci->getRoadId());
    }else if(msg == nextWSMEvent){
        sendWSMMessage(traci->getRoadId());
    }else if(msg == updatePOSEvent){
        scheduleAt(simTime() + sendPositionTableInterval,updatePOSEvent);
        updateCarStatusList();
        /*debug*/
//
//        if(myId == 4){
//            int a =10;
//            int b= 100;
//            int c= a+b;
//            c = c + a;
//        }

        WaveShortMessage *wsa = prepareBaseMSG("wsa",dataLengthBits,dataPriority,178,carMove.getSpeed(),carMove.getDirection(),simTime());
        wsa = prepareWSA(wsa,0,0,0,0,-1);
        wsa->setWsmData("update position information");
        wsa = prepareWSA2(wsa,0,0,0,0,178,-1,simTime());
        /*send other car position information*/
        if(!posFlag){//first 10 car position information
            map<int,CarMove*>::iterator iter2 = carMoveStatus.begin();
            for(int i = 0;(iter2 != carMoveStatus.end())&&(i<10);iter2++){//20
            CarMove *carmove = iter2->second;
            wsa->setCarlist(i,*carmove);
            i++;
            }
        }else{//last 10 car position information
            map<int,CarMove*>::reverse_iterator iter3 = carMoveStatus.rbegin();
            for(int i = 0;(iter3 != carMoveStatus.rend())&&(i<10);iter3++){//20
            CarMove *carmove = iter3->second;
            wsa->setCarlist(i,*carmove);
            i++;
            }
        }
        posFlag = (!posFlag);//turn around

        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        double offSet = dblrand()*0.005 + 0.01;
        if(state == type_SCH){
            sendDelayedDown(wsa,timeToNextChannel + offSet);
        }else{/*CCH*/
            if(timeToNextChannel <= 0.005){
                /*next CCH channel slot send the message*/
                sendDelayedDown(wsa,timeToNextChannel+SWITCHING_INTERVAL_11P+offSet);
            }else{
                sendWSA(wsa);
            }
        }
    }else if(msg == updateRouteTableEvent){
        scheduleAt(simTime() + sendRouteTableInterval,updateRouteTableEvent);
        updateRouteTable();
        WaveShortMessage *wsa = prepareBaseMSG("wsa",dataLengthBits,dataPriority,178,carMove.getSpeed(),carMove.getDirection(),simTime());
        wsa = prepareWSA(wsa,0,0,0,0,-1);
        wsa->setWsmData("update route table information");
        wsa = prepareWSA2(wsa,0,0,0,0,178,-1,simTime());
        /*send route table information*/
        if(!posFlag){//send first car route information
            map<int,RouteEntry*>::iterator iter4 = routeTable.begin();
            for(int i = 0;(iter4 != routeTable.end())&&(i<10);iter4++){
                RouteEntry *entry = iter4->second;
                wsa->setRouteTable(i,*entry);
                i++;
            }
        }else{// send last 10 route information
            map<int,RouteEntry*>::reverse_iterator iter5 = routeTable.rbegin();
            for(int i = 0;(iter5 != routeTable.rend())&&(i<10);iter5++){
                RouteEntry *entry = iter5->second;
                wsa->setRouteTable(i,*entry);
                i++;
            }
        }
        posFlag = (!posFlag);
        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        double offSet = dblrand()*0.005 + 0.01;
        if(state == type_SCH){
            sendDelayedDown(wsa,timeToNextChannel + offSet);
        }else{
            if(timeToNextChannel <= 0.005){
                sendDelayedDown(wsa,timeToNextChannel+SWITCHING_INTERVAL_11P + offSet);
            }else{
                sendWSA(wsa);
            }
        }
    }else if(msg == updateCarNeighborListEvent){
        scheduleAt(simTime()+ WSAInterval + 1.0,updateCarNeighborListEvent);
        updateCarNeighborList();
    }else if(msg == updateServiceEvent){
        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        if(state == type_SCH){
            double offset = serviceUpdateInterval + dblrand()*0.005 - 0.03;
            scheduleAt(simTime()+timeToNextChannel + offset,updateServiceEvent);
        }else{
            double offset = serviceUpdateInterval + dblrand()*0.005 + 0.02;
            scheduleAt(simTime()+ timeToNextChannel + offset,updateServiceEvent);
        }
        updateServiceList(availableServiceList);
    }else if(msg == unicastEvent){
        int SrcProcId = unicastEvent->getSrcProcId();
        if(SrcProcId == myId)
        {
            if(sendCount < 90){
                /*
                t_channel state = getActiveChannel();
                double timeToNextChannel = timeToNextSwitch();
                if(state == type_SCH){
                    double offset = sendUnicastWSMInterval + timeToNextChannel+ 0.020 + dblrand()*0.025;
                    scheduleAt(simTime() + offset,unicastEvent);
                }else{
                    double offset = sendUnicastWSMInterval + timeToNextChannel - 0.030 + dblrand()*0.025;
                    scheduleAt(simTime() + offset,unicastEvent);
                }
                */
                scheduleAt(simTime()+ sendUnicastWSMInterval,unicastEvent);
                int psid = 90+myId;
                int receiveId = -1;
               /*
                switch (carId) {
                    case 0:
                        receiveId = 23;
                        break;
                    case 1:
                        receiveId = 21;
                        break;
                    case 2:
                        receiveId = 18;
                        break;
                    case 3:
                        receiveId = 15;
                        break;
                    case 4:
                        receiveId = 12;
                        break;
                    case 5:
                        receiveId = 9;
                        break;
                    case 6:
                        receiveId = 6;
                        break;
                    case 7:
                        receiveId = 3;
                        break;
                }*/

                if(myId == 0){
                    //receiveId = 38;
                    //receiveId = 30;
                    receiveId = 23;
                    //receiveId = 16;
                }else if(myId == 1){
                    //receiveId = 36;
                    //receiveId = 29;
                    receiveId = 22;
                    //receiveId = 15;
                }else if(myId == 2){
                    //receiveId = 32;
                    //receiveId = 26;
                    receiveId = 20;
                    //receiveId = 14;
                }else if(myId == 3){
                    //receiveId = 28;
                    //receiveId = 23;
                    receiveId = 18;
                    //receiveId = 13;
                }else if(myId == 4){
                    //receiveId = 24;
                    //receiveId = 20;
                    receiveId = 16;
                    //receiveId = 12;
                }else if(myId == 5){
                    //receiveId = 20;
                    //receiveId = 17;
                    receiveId = 14;
                    //receiveId = 11;
                }else if(myId == 6){
                    //receiveId = 16;
                    //receiveId = 14;
                    receiveId = 12;
                    //receiveId = 10;
                }else if(myId== 7){
                    //receiveId = 12;
                    //receiveId = 11;
                    receiveId = 10;
                    //receiveId = 9;
                }
                sendRepeatInfo(psid,receiveId);
                sendCount++;
                return;
            }
        }
    }else if (msg == broadcastEvent){
        int SrcProcId = broadcastEvent->getSrcProcId();
        if (SrcProcId == myId){
            int psid = 100;
            int receiveId = -1;
            sendRepeatInfo(psid,receiveId,0);
        }
    }else if(msg == repeatSendWSAEvent){
        t_channel state = getActiveChannel();
        double timeToNextChannel = timeToNextSwitch();
        if(state == type_SCH){
            double offSet = WSAInterval + dblrand()*0.025;
            scheduleAt(simTime()+ timeToNextChannel + offSet,repeatSendWSAEvent);
        }else{
            double offSet = WSAInterval - 0.05 + dblrand()*0.025;
            scheduleAt(simTime()+ timeToNextChannel + offSet,repeatSendWSAEvent);
        }
        int sch = randomSelectChannel();

        ASSERT(sch >= 174);
        ASSERT(sch <= 182);

        int priority = intuniform(1,6);
        int psid = intuniform(1,40);
        int receivedId = intuniform(0,40);
        cService *service = new cService(psid,sch,priority,receivedId,1,9,myId,1);
        service->setTimestamp(simTime());
        service->setPositions(curPosition);
        service->setExpire(0.20);
        service->setExpireTime(simTime()+0.20);
        insertService(providerServiceList,service);
        sendWSAMessage("send repeat wsa message",service,0);
    }
}
void AppTest1::insertService(ServiceListType &list,cService *service)
{
    int psid = service->getPsid();
    int sch = service->getSch();
    map<int,map<int,cService*> >::iterator it = list.find(sch);
    /*find out if the channel is aready exist */
    if(it != list.end())
    {
        /*aready exist,insert the service in the related channnel*/
        it->second.insert(make_pair(psid,service));
    }else
    {
        /*not exist,then insert the channel and service*/
        psidService pService;
        pService.insert(make_pair(psid,service));
        list.insert(make_pair(sch,pService));
    }
}
void AppTest1::deleteServie(ServiceListType &list,int channel,int psid){
    if(list.empty())
    {
        DBG << "##AppTEST1::ERROR:delete an empty servicelist!" << std::endl;
        return;
    }

    map<int,map<int,cService*> >::iterator it = list.find(channel);
    /*find the channel*/
    if(it != list.end())
    {
        map<int,cService*>::iterator iter = (it->second).find(psid);
        if(iter != (it->second).end())
        {
            /*we must delete the content of the pointer direct and then delete the iterator */
            cService *service = iter->second;
            delete service;
            //delete (iter->second);
            (it->second).erase(iter);
            /*after delete the service,you need check the channel whether has other service,if not we need the channel*/
            if((it->second).empty())
            {
                list.erase(it);
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
            if(currentTime - startTime >= positionUpdateInterval)
            {
                delete (iter->second);//callback the memory
                carMoveStatus.erase(iter);
            }
            ++iter;
        }
    }
}
void AppTest1::updateCarNeighborList(){
    if(!neighborCarList.empty())
    {
        map<int,CarMove*>::iterator iter = neighborCarList.begin();
        while(iter != neighborCarList.end())
        {
            simtime_t startTime = iter->second->getStartTime();
            simtime_t currentTime = simTime();
            double Interval = currentTime.dbl() - startTime.dbl();
            double lifetime = iter->second->getLifetime();
            if(Interval > WSAInterval && Interval > lifetime){
                delete (iter->second);
                neighborCarList.erase(iter);
            }
            if(Interval > WSAInterval && Interval < lifetime){
                iter->second->setFlag(1);//set flag is bad
            }
            ++iter;
        }
    }
}
void AppTest1::updateRouteTable(){
    if(!routeTable.empty()){
        map<int,RouteEntry*>::iterator iter = routeTable.begin();
        while(iter != routeTable.end()){
            simtime_t startTime = iter->second->getTimestamp();
            simtime_t currentTime = simTime();
            if(currentTime - startTime >= routeTableUpdateInterval ){
                delete (iter->second);
                routeTable.erase(iter);
            }
            ++iter;
        }
    }
}
void AppTest1::updateServiceList(ServiceListType &list){
    if(list.empty()){
        return;
    }
    ServiceListType::iterator iter = list.begin();
    for(;iter != list.end(); iter++){
        map<int,cService*>::iterator iter2 = iter->second.begin();
        for(;iter2 != iter->second.end(); iter2++){
            simtime_t expireTime = iter2->second->getExpireTime();
            simtime_t currentTime = simTime();
            if(currentTime > expireTime){
                cService *service = iter2->second;
                delete service;
                iter->second.erase(iter2);
            }
        }

        if(iter->second.empty()){
            list.erase(iter);
        }
    }
}
double AppTest1::distance(const Coord& a,const Coord& b){
    return a.distance(b);
}
double AppTest1::directionAngle(const Coord& dir1,const Coord& dir2){
    double arc1 = radians(dir1);
    double arc2 = radians(dir2);
    double diff = abs(arc1 - arc2);
    return diff;
    /*two direction angle is diff than 90 degree,so they are not in the same direction*/
    /*
    if(diff >= M_PI_2)
    {
        return false;
    }else{
        return true;
    }
    */
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
double AppTest1::calculateLifetime(CarMove &carA,CarMove &carB){
    double R = 500.0;//the radius of the communication
    double speedA = carA.getSpeed();
    double speedB = carB.getSpeed();
    Coord posA = carA.getPositionAt(simTime());
    Coord posB = carB.getPositionAt(simTime());
    double n = abs(posB.y - posA.y);//n is represent CB length
    double p = abs(posB.x - posA.x);//p is represent AC length
    double m = sqrt(R*R - p*p);//m is represent CD length
    double dist = distance(posA,posB);
    Coord directionA = carA.getDirection();
    Coord directionB = carB.getDirection();
    double angle = directionAngle(directionA,directionB);

    /*we have 6 situation to calculate the life time */
    double lifetime = WSAInterval + 1.0;
    double abs1 = abs(speedA - speedB);
    double abs2 = abs(speedA + speedB);
    /*1.first situation:same direction and speedA great than speedB*/
    if(angle == 0.0 && speedA < speedB){
        lifetime = (R - dist)/abs1;
    }

    /*2.second situation:same direction and speedA is less than speedB*/
    if(angle == 0.0 && speedA > speedB){
        lifetime = (R + dist)/abs1;
    }

    /*3. third situation:same direction but not in same lane and speedA is less speedB */
    if((angle >0 && angle < M_PI_2) && speedA < speedB){
        lifetime = (m - n)/abs1;
    }

    /*4.fourth situation:same direction but not in same lane and speedA is great than speedB*/
    if((angle >0 && angle < M_PI_2)&& speedA > speedB){
        lifetime = (m+n)/abs1;
    }

    /*5.five situation:opposite direction:they are opposite move*/
    if(angle >= M_PI_2 && posB.x > posA.x){
        lifetime = (m + n)/abs2;
    }

    /*6.six situation:opposite direction:they are Divergence motion*/
    if(angle >= M_PI_2 && posB.x < posA.x){
        lifetime = (m - n)/abs2;
    }
    return lifetime;
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
int AppTest1::selectNextCarWithNeighborCarList(int destId,Coord &destPos){
    int carId = -1;
    /*the neighbor car list is empty,so we no car to choose*/
    if(neighborCarList.empty()){
        return carId;
    }

    /*1.Test the dest car is neighbor car?*/
    map<int,CarMove*>::iterator iter,iter1,iter2;
    iter = neighborCarList.find(destId);
    if(iter != neighborCarList.end()){
        /*find the car*/
        return destId;
    }

    Coord curPos = traci->getCurrentPosition();
    double SD = distance(curPos,destPos);//the source and destination car distance
    //double R = 500.0;//the communication Radius
    /*2.use the Greedy forwarding select the next carID */
    double nearestDist = SD;
    for(iter1 = neighborCarList.begin(); iter1 != neighborCarList.end(); iter1++)
    {
        CarMove *carmove = iter1->second;
        Coord pos = carmove->getPositionAt(simTime());
        double ND = distance(pos,destPos);//the next car and the destination car distance
        /*choose the car in the communication and leave the destination nearest*/
        if(ND < nearestDist)
        {
            nearestDist = ND;
            carId = iter1->first;
        }
    }

    /*3.if the greedy forward failed,then we choose the GPSR-AD Algorithm select the forward car id*/
    if(carId == -1){
        double XSD = 0.5;//we choose the specific angle to compare,we assume the angle is 0.5
        CarMove *tempMove = (neighborCarList.begin())->second;
        Coord tempPos = tempMove->getPositionAt(simTime());
        double MD = distance(tempPos,destPos);/*the next car and destination car distance*/
        double MS = distance(tempPos,curPos);/*the next car and the source car distance*/
        double angleMSD = (MS*MS + SD*SD - MD*MD)/(2*MS*SD);//the angle MSD cos() function value
        double MSD = acos(angleMSD);//the angle MSD acos() function value
        double GAD = MD/SD + MSD/XSD;//the GPSR-AD value
        double smallestGAD = GAD;//the smallest GPSR-AD value
        carId = tempMove->getCarId();

        /*begin use the GPSR-AD algorithm choose the next forward car*/
        for(iter2 = neighborCarList.begin(); iter2 != neighborCarList.end(); iter2++)
        {
            CarMove *carmove1 = iter2->second;
            Coord pos1 = carmove1->getPositionAt(simTime());
            MD = distance(pos1,destPos);
            MS = distance(pos1,curPos);
            angleMSD = (MS*MS + SD*SD - MD*MD)/(2*MS*SD);
            MSD = acos(angleMSD);
            GAD = MD/SD + MSD/XSD;
            /*choose the smallest GAD as the next forward car*/
            if(GAD < smallestGAD){
                smallestGAD = GAD;
                carId = iter2->first;
            }
        }
    }

    return carId;
}
int AppTest1::selectNextCarWithRouteTable(int dest){
    int carId = -1;// not found
    map<int,RouteEntry*>::iterator it = routeTable.find(dest);
    if(it != routeTable.end()){//find
        int nextCarId = it->second->getNextCarId();
        if(nextCarId == -1){
            return dest;//in the communication range
        }else{
            return nextCarId;
        }
    }
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
    int forwardId = -1;
    if(positionRoute){
        Coord destPos = wsm->getDestPos();
        forwardId = selectNextCarWithNeighborCarList(destId,destPos);
        /*if the forwardId is the sender car Id,then we forbidden forward this message,so we set the forward Id as -1*/
        if(forwardId == wsm->getSenderAddress()){
            forwardId = -1;
        }
        //forwardId = selectNextCar(destId);
    }else{
        forwardId = selectNextCarWithRouteTable(destId);
    }

    if(forwardId != -1)
    {

        hopLimit = hopLimit -1;
        /*choose the next service channel*/
        int sch = selectNextChannel();

        ASSERT(sch >= 174);
        ASSERT(sch <= 182);

        cService *service = new cService(wsm->getPsid(),sch,wsm->getUserPriority(),wsm->getRecipientAddress(),wsm->getFlag(),hopLimit,wsm->getOriginalAddress(),wsm->getSerial());
        service->setTimestamp(wsm->getTimestamp());
        service->setPositions(wsm->getSenderPos());
        service->setExpire(0.20);
        service->setExpireTime(simTime()+0.20);
        insertService(providerServiceList,service);
        sendWSAMessage("forward message",service,1);
        if(service->getPsid() >= 90){
            if(ev.isGUI()){
            char msgname[40];
            sprintf(msgname,"forward a packet from %d to %d",service->getOriginalSender(),service->getDst());
            findHost()->bubble(msgname);
            findHost()->getDisplayString().setTagArg("t",0,msgname);
            }
        }
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
        if(wsm->getPsid() > 50){
            statsReceivedBroadcasts++;
            int hop = wsm->getHopLimit() + 1;
            double time = simTime().dbl() - wsm->getTimestamp().dbl();
            double dist = distance(curPosition,wsm->getSenderPos());
            emit(delaySignal,time);
            emit(distanceSignal,dist);
            DBG<<"######AppTest1::received a broadcast packet from the sender: "<< wsm->getOriginalAddress() << "at the time " << wsm->getTimestamp() << std::endl;
            if(ev.isGUI()){
                char msgname[40];
                sprintf(msgname,"from %d with %d hopLimit take %f s",wsm->getOriginalAddress(),hop,time);
                findHost()->bubble(msgname);
            }
        }
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

    ASSERT(sch >= 174);
    ASSERT(sch <= 182);

    cService *service = new cService(wsm->getPsid(),sch, wsm->getUserPriority(), wsm->getRecipientAddress(), wsm->getFlag(),hopLimit,wsm->getOriginalAddress(),wsm->getSerial());
    service->setTimestamp(wsm->getTimestamp());
    service->setPositions(wsm->getSenderPos());
    service->setExpire(0.20);
    service->setExpireTime(simTime()+0.20);
    insertService(providerServiceList,service);

    /*begin to broadcast the wsa packet*/
    double timeToNextChannel = timeToNextSwitch();
    double offSet = dblrand()*0.005;
    if(nextWSAEvent->isScheduled()){
        cancelEvent(nextWSAEvent);
    }
    scheduleAt(simTime()+ timeToNextChannel + offSet,nextWSAEvent);
    //sendWSAMessage(wsm->getWsmData());
}
void AppTest1::unicastWSMMessage(WaveShortMessage* msg){
    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);
    int destId = wsm->getRecipientAddress();
    int forwardId = -1;
    if(positionRoute){
        /*find out the destId related position*/
        map<int,CarMove*>::iterator iter = carMoveStatus.find(destId);
        if(iter != carMoveStatus.end())
        {
            CarMove *carmove = iter->second;
            Coord destPos = carmove->getPositionAt(simTime());
            wsm->setDestPos(destPos);
            forwardId = selectNextCarWithNeighborCarList(destId,destPos);
        }
    }else{
        forwardId = selectNextCarWithRouteTable(destId);
    }
    if(forwardId != -1)
    {
        /*update the next forward address*/
        if((wsm->getPsid() == (90+myId)) && (wsm->getOriginalAddress() == myId)){
            statsSendUnicast++;
        }
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

    if(wsm->getPsid() > 50){
        wsm->setTimestamp(simTime());
        statsSendBroadcast++;
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
        sch = randomSelectChannel();
    }
    return sch;
}
int AppTest1::randomSelectChannel(){
    int randomv = intuniform(1,4);
    int sch = -1;
    switch (randomv) {
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
void AppTest1::updateDisplay(){
    char buf[40];
    sprintf(buf, "from %d to %d with %d hopLimit",sender,myId,hopCount);
    findHost()->getDisplayString().setTagArg("t",0,buf);
}
void AppTest1::sendRepeatInfo(int psid,int receiveId,int hopLimit){
    int priority = intuniform(1,6);
    int sch = selectNextChannel();
    ASSERT(sch >= 174);
    ASSERT(sch <= 182);
    cService *service = new cService(psid,sch,priority,receiveId,0,hopLimit,myId,1);
    service->setTimestamp(simTime());
    service->setPositions(curPosition);
    service->setExpire(0.20);
    service->setExpireTime(simTime()+0.20);
    insertService(providerServiceList,service);
    sendWSAMessage("unicast wsm message",service,1);
}
AppTest1::AppTest1() {
    // TODO Auto-generated constructor stub

}
AppTest1::~AppTest1() {
    // TODO Auto-generated destructor stub
}

