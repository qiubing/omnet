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

#ifndef CSERVICE_H_
#define CSERVICE_H_
#include "simtime_t.h"
#include <omnetpp.h>
#include "bitset"
#include "Coord.h"
using namespace std;

class cService {
private:
    //psid present which service to provide
    int psid;
    //sch present which service channel to use
    int sch;
    //priority use for determine which service send first
    int priority;
    //dst present the target receiver,-1 represent all receiver
    int dst;
    //flag tell the receiver whether this service to forward
    int flag;

    //hopLimit tell the forwarder the packet should forward or not
    int hopLimit;

    //the original sender address
    int originalSender;

    //the packet sequence,which to Distinguish the repeat packet
    int serial;

    //the sender position to calculate the distance
    Coord positions;

    //timestamp use for update the service list
    simtime_t timestamp;

public:

    //std::bitset<1024> bitvec;

    cService(){ };
    cService(int psid,int sch,int priority,int dst,int flag, int hopLimit, int originalSender, int serial):psid(psid),sch(sch),priority(priority),dst(dst),flag(flag),hopLimit(hopLimit),originalSender(originalSender),serial(serial) { };
    virtual ~cService() { };

    int getDst() const {
        return dst;
    }

    void setDst(int dst) {
        this->dst = dst;
    }

    int getFlag() const {
        return flag;
    }

    void setFlag(int flag) {
        this->flag = flag;
    }

    int getPriority() const {
        return priority;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }

    int getPsid() const {
        return psid;
    }

    void setPsid(int psid) {
        this->psid = psid;
    }

    int getSch() const {
        return sch;
    }

    void setSch(int sch) {
        this->sch = sch;
    }

    const simtime_t& getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(const simtime_t& timestamp) {
        this->timestamp = timestamp;
    }

    int getHopLimit() const {
        return hopLimit;
    }

    void setHopLimit(int hopLimit) {
        this->hopLimit = hopLimit;
    }

    int getOriginalSender() const {
        return originalSender;
    }

    void setOriginalSender(int originalSender) {
        this->originalSender = originalSender;
    }

    int getSerial() const {
        return serial;
    }

    void setSerial(int serial = 0) {
        this->serial = serial;
    }

    const Coord& getPositions() const {
        return positions;
    }

    void setPositions(const Coord& positions) {
        this->positions = positions;
    }
};

#endif /* CSERVICE_H_ */
