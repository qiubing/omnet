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

#ifndef APPTEST1_H_
#define APPTEST1_H_

#include <WaveAppLayer.h>
#include "modules/mobility/traci/TraCIMobility.h"
#include "cService.h"
//#include "cCar.h"
#include "Car.h"
#include "CarMove.h"
#include "Coord.h"
#include "stdint.h"
#include "RouteEntry.h"
#include <map>
#include <list>
#include <iostream>
#include <omnetpp.h>
#include <utility>
#include <Consts80211p.h>
#include <WaveAppToMac1609_4Interface.h>


using Veins::TraCIMobility;
using Veins::AnnotationManager;
using std::map;
using std::multimap;

typedef map<int,cService*> psidService;
typedef map<int,cService*>::value_type valType;
typedef map<int,map<int,cService*> > ServiceListType;
typedef map<int,float> ChannelWeightType;

/*
 * application to test the waveApplayer and using 11p
 * */

class AppTest1: public WaveAppLayer {
private:
    simsignal_t hopLimitSignal;
    simsignal_t delaySignal;
    simsignal_t distanceSignal;
public:
    AppTest1();
    virtual ~AppTest1();
    virtual void initialize(int stage);
    virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);
    /** @brief Delete all dynamically allocated objects of the module.*/
    virtual void finish();


    /*allocate the channel by the serviceList provided,return -1 represent not find the appropriate channel */
    virtual int allocateChannel(ServiceListType &serviceList);
    /*
     * caculate the channel weight with the service list */
    virtual void updateChannelWeight(ServiceListType &serviceList,ChannelWeightType &channelWeight);

    //find out the max weight in map<int,float> and return the related channel
    virtual int findMaxWeight(ChannelWeightType channelWeight);

    //find the available service list and subscribe service list whether has common service, if has then return the service
    virtual cService* findCommonService(ServiceListType &aServiceList,psidService &sServiceList);

    //virtual void insertService(ServiceListType *list,cService *service);
    virtual void insertService(ServiceListType &list,cService *service);

    /*delete the  servicelist channel pisd service */
    //virtual void deleteServie(ServiceListType *list,int channel,int psid);
    virtual void deleteServie(ServiceListType &list,int channel,int psid);


    /*update the car list,if the car not in the list then add it, the result is false represent the packet has received and drop it, if the result is true,represent the packet has not received */
    virtual bool  updateCarList(int id, Coord &curpos, simtime_t timestamp,int serial);

    /*update the car position information and remove the car information that timestamp more than 5s*/
    virtual void updateCarStatusList();

    /*update the car route table information periodically the timestamp is the important key to decide how update the list*/
    virtual void updateRouteTable();

    /*update the service list table by expire time*/
    virtual void updateServiceList(ServiceListType &list);

    /*calculate the distance between  two coordiante */
    virtual double distance(const Coord& a,const Coord& b);

    /*judge the two direction is the same direction*/

    virtual bool isSameDirection(const Coord& dir1,const Coord& dir2);

    /*return the radians of the x value,the range is from -pi to pi*/
    virtual double radians(const Coord& direction);

    /*select next forward car for the unicast,the distance must leave the destination nearest and in the same direction beside in the communication range*/
    virtual int selectNextCar(int dest);

    /*select next forward car for the unicast with the route table,which is update without car position information*/
    virtual int selectNextCarWithRouteTable(int dest);

    /*select next forward channel and build the service */
    virtual int selectNextChannel();

    /*Randomly selected a service channel*/
    virtual int randomSelectChannel();

    /*get the current channel state*/
    t_channel getActiveChannel();

    /*return the next time to switch channel,that is there are how much time left in the current slot*/
    double timeToNextSwitch();

    /*display the message*/
    virtual void updateDisplay();

    /*prepare repeat information to send*/
    virtual void sendRepeatInfo(int psid, int receivedId,int hopLimit=9);

protected:
    TraCIMobility* traci;
    AnnotationManager* annotations;
    simtime_t lastDroveAt;


    /*Self Message to wake up at next WSA sent*/
    cMessage* nextWSAEvent;

    /*Self Message to wake up at next WSM sent*/
    cMessage* nextWSMEvent;

    /*update position status event*/
    cMessage* updatePOSEvent;

    /*update route table information event*/
    cMessage* updateRouteTableEvent;

    /*Self Message to timing send message*/
    cMessage *unicastEvent;

    /*broadcast message event*/
    cMessage *broadcastEvent;

    /*update available service list event*/
    cMessage *updateServiceEvent;

    /*send WSA event repeatedly */
    cMessage *repeatSendWSAEvent;

    bool sentWSM;
    bool sentWSA;
    bool isParking;
    bool sendWhileParking;
    bool isProvider;//is provider at the initialize stage
    static const simsignalwrap_t parkingStateChangedSignal;
    /*indicate in which channel provide service */
    int channelNum;
    /*the packet serial*/
    int serial;
    bool positionRoute;//route flag,if it is true,then use car position route,otherwise route with no car position

    int carId;//select different car to simulator different scene
    int sender;
    int hopCount;

    double WSAInterval;//send WSA message interval 0.1s as a unit
    double positionUpdateInterval;//position list update interval
    double routeTableUpdateInterval;//route table update interval
    double serviceUpdateInterval;//service list update interval
    double sendUnicastWSMInterval;//repeat send wsm message interval
    double sendPositionTableInterval;//repeat send position information interval
    double sendRouteTableInterval;//repeat send route table information interval


    /*car move status*/
    CarMove carMove;

    WaveAppToMac1609_4Interface* myMac;

    //three service list to decide provide which services
    map<int,psidService> providerServiceList;
    map<int,psidService> availableServiceList;
    map<int,cService*>   subscribeServiceList;

    /*The wsa message broadcast service,then the wsm will send the service that the wsa broadcasted */
    list<cService*> broadcastServiceList;


    //carlist contain the car communication with me
    map<int,Car*> carList;

    //every node maintain a table about others moving status
    map<int,CarMove*> carMoveStatus;

    //every node maintain a route table about to other moving nodes without position information
    map<int,RouteEntry*> routeTable;

    /*statics send count*/
    int sendCount;

    /*send position flag,flag 0 represent send first 10 car position information and flag 1 represent send last 10 car position information*/
    bool posFlag;


    //stats
    long statsReceivedUnicast;//received unicast packets
    long statsReceivedBroadcasts;//received broadcast packets
    long statsSendUnicast;//send unicast packets
    long statsSendBroadcast;//send broadcast packets

protected:
    virtual void onWSA(WaveShortMessage* wsm);
    virtual void onWSM(WaveShortMessage* wsm);
    void sendWSMMessage(std::string blockedRoadId);
    void unicastWSMMessage(WaveShortMessage* msg);
    void broadcastWSMMessage(WaveShortMessage* msg);
    void sendWSAMessage(std::string blockedRoadId);
    void sendWSAMessage(std::string blockedRoadId,cService *service,int flag);//flag decide send in first 25ms or second 25ms send message
    virtual void unicastForwardMessage(WaveShortMessage* msg);
    virtual void broadcastForwardMessage(WaveShortMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);
    virtual void handleParkingUpdate(cObject* obj);
    /*
     * send wsm packet
     * */
    virtual void sendWSM(WaveShortMessage* wsm);
    /*
     * send wsa packet
     * */
    virtual void sendWSA(WaveShortMessage* wsa);

    virtual void handleSelfMsg(cMessage *msg);
};

#endif /* APPTEST1_H_ */
