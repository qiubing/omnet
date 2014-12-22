/*
 * Wave1609_3.cc
 *
 *  Created on: Nov 27, 2014
 *      Author: bing
 */
#include "Wave1609_3.h"
#include <cassert>
#include "BaseMacLayer.h"
#include "FindModule.h"
#include "WaveShortMessage_m.h"

Define_Module(Wave1609_3);

void Wave1609_3::initialize(int stage)
{
    BaseLayer::initialize(stage);
    if(stage == 0){
        coreDebug = par("coreDebug").boolValue();
        headerLength = par("headerLength");
    }else if(stage == 1){
        coreEV << "Wave1609_3 network layer" << std::endl;
    }
}

/**
 * Decapsulates the packet from the received Wave1609.3 packet
 **/
/*
cMessage* Wave1609_3::decapsMsg(WaveShortMessage *msg)
{
    WaveShortMessage *m = msg;
    delete msg;
    return m;
}
*/

/**
 * Encapsulates the received ApplPkt into a WaveshortMessage and set all needed
 * header fields.
 **/

/*
WaveShortMessage* Wave1609_3::encapsMsg(cPacket *appMsg)
{
    coreEV << "in encaps..\n";
    WaveShortMessage *wsm = new WaveShortMessage(appMsg->getName(),appMsg->getKind());
    wsm->setBitLength(headerLength);

    //encapsulate the application packet
    wsm->encapsulate(appMsg);
    coreEV << "appMsg encapsulateed \n";
    return wsm;
}
*/
/**
 * Redefine this function if you want to process messages from lower
 * layers before they are forwarded to upper layers
 *
 *
 * If you want to forward the message to upper layers please use
 * @ref sendUp which will take care of decapsulation and thelike
 **/

void Wave1609_3::handleLowerMsg(cMessage *msg)
{
    WaveShortMessage* wsm = static_cast<WaveShortMessage*>(msg);
    coreEV << "handling packet from "<< wsm->getSrcProcId() << std::endl;
    sendUp(wsm);
}


/**
 * Redefine this function if you want to process messages from upper
 * layers before they are send to lower layers.
 *
 * For the BaseNetwLayer we just use the destAddr of the network
 * message as a nextHop
 *
 * To forward the message to lower layers after processing it please
 * use @ref sendDown. It will take care of anything needed
 **/

void Wave1609_3::handleUpperMsg(cMessage *msg)
{
    assert(dynamic_cast<cPacket*>(msg));
    //sendDown(encapsMsg(static_cast<cPacket*>(msg)));
    WaveShortMessage *wsm = static_cast<WaveShortMessage*>(msg);
    coreEV << "sending message to the WAVE1609_4 layer" << wsm->getName()<<std::endl;
    sendDown(wsm);
}

/**
 * Redefine this function if you want to process control messages
 * from lower layers.
 *
 * This function currently handles one messagetype: TRANSMISSION_OVER.
 * If such a message is received in the network layer it is deleted.
 * This is done as this type of messages is passed on by the BaseMacLayer.
 *
 * It may be used by network protocols to determine when the lower layers
 * are finished sending a message.
 **/

void Wave1609_3::handleLowerControl(cMessage *msg)
{
    EV <<"Wave1609_3 don't handle control message from the Wave1609_4"<< std::endl;
}





