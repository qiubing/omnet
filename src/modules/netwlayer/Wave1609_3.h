/*
 * Wave1609_3.h
 *
 *  Created on: Nov 27, 2014
 *      Author: bing
 */

#ifndef WAVE1609_3_H_
#define WAVE1609_3_H_

#include "BaseLayer.h"
#include "SimpleAddress.h"
#include "WaveShortMessage_m.h"


#ifndef coreEV
#define coreEV_clear (ev.isDisabled()||!coreDebug) ? ev : ev
#define coreEV (ev.isDisabled()||!coreDebug) ? ev : ev << logName() << "::" << getClassName() <<": "
#endif

/*
 * implement for network interface
 * */

class Wave1609_3 : public BaseLayer
{
private:
    /** @brief Copy constructor is not allowed.
    */
    Wave1609_3(const Wave1609_3&);

    /** @brief Assignment operator is not allowed.
    */
    Wave1609_3& operator=(const Wave1609_3&);

public:

protected:
    /**
         * @brief Length of the WaveShortMessage header
         * Read from omnetpp.ini
         **/
    int headerLength;

    /** @brief Enables debugging of this module.*/
    bool coreDebug;

public:
    Wave1609_3(): BaseLayer(),headerLength(0),coreDebug(false) {}

    Wave1609_3(unsigned stacksize): BaseLayer(stacksize),headerLength(0),coreDebug(false) {}

    /** @brief Enables debugging of this module.*/
    virtual void initialize(int);

protected:
    /**
        * @name Handle Messages
        * @brief Functions to redefine by the programmer
        *
        * These are the functions provided to add own functionality to your
        * modules. These functions are called whenever a self message or a
        * data message from the upper or lower layer arrives respectively.
        *
        **/
       /*@{*/

    /** @brief Handle messages from upper layer */
      virtual void handleUpperMsg(cMessage* msg);

      /** @brief Handle messages from lower layer */
      virtual void handleLowerMsg(cMessage* msg);

      /** @brief Handle self messages */
      virtual void handleSelfMsg(cMessage* /*msg*/){
      error("Wave1609_3 Layer does not handle self messages");
      }

      /** @brief Handle control messages from lower layer */
      virtual void handleLowerControl(cMessage* msg);

      /** @brief Handle control messages from lower layer */
      virtual void handleUpperControl(cMessage* /*msg*/){
          error("Wave1609_3 Layer does not handle control messages");
      }

      /*@}*/

      /** @brief decapsulate higher layer message from waveShortMessage*/
      //virtual cMessage* decapsMsg(WaveShortMessage*);

      /** @brief Encapsulate higher layer packet into an WaveShortMessage*/
     //virtual WaveShortMessage*  encapsMsg(cPacket*);


      /**
       * @brief Attaches a "control info" (NetwToMac) structure (object) to the message pMsg.
       *
       * This is most useful when passing packets between protocol layers
       * of a protocol stack, the control info will contain the destination MAC address.
       *
       * The "control info" object will be deleted when the message is deleted.
       * Only one "control info" structure can be attached (the second
       * setL3ToL2ControlInfo() call throws an error).
       *
       * @param pMsg      The message where the "control info" shall be attached.
       * @param pDestAddr The MAC address of the message receiver.
       */
      //virtual cObject* setDownControlInfo(cMessage *const pMsg, const LAddress::L2Type& pDestAddr);
      /**
       * @brief Attaches a "control info" (NetwToUpper) structure (object) to the message pMsg.
       *
       * This is most useful when passing packets between protocol layers
       * of a protocol stack, the control info will contain the destination MAC address.
       *
       * The "control info" object will be deleted when the message is deleted.
       * Only one "control info" structure can be attached (the second
       * setL3ToL2ControlInfo() call throws an error).
       *
       * @param pMsg      The message where the "control info" shall be attached.
       * @param pSrcAddr  The MAC address of the message receiver.
       */
      //virtual cObject* setUpControlInfo(cMessage *const pMsg, const LAddress::L3Type& pSrcAddr);




};


#endif /* WAVE1609_3_H_ */
