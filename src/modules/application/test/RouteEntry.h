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

#ifndef ROUTEENTRY_H_
#define ROUTEENTRY_H_
#include "simtime_t.h"
#include <omnetpp.h>

class RouteEntry {
private:
    //car id
    int carId;
    //car Mac address
    long macAddress;
    // next car id
    int nextCarId;
    //the hop count as distance
    int distance;
    //received time stamp
    simtime_t timestamp;

public:
    RouteEntry(int carId = -1):carId(carId),macAddress(carId+80000) { };
    RouteEntry(int carId,long macAddress,int nextCarId, int distance):carId(carId),macAddress(macAddress),nextCarId(nextCarId),distance(distance) { };
    virtual ~RouteEntry() {
        // TODO Auto-generated destructor stub
    }

    int getCarId() const {
        return carId;
    }

    void setCarId(int carId) {
        this->carId = carId;
        this->macAddress = carId + 80000;
    }

    int getDistance() const {
        return distance;
    }

    void setDistance(int distance) {
        this->distance = distance;
    }

    long getMacAddress() const {
        return macAddress;
    }

    void setMacAddress(long macAddress) {
        this->macAddress = macAddress;
    }

    int getNextCarId() const {
        return nextCarId;
    }

    void setNextCarId(int nextCarId) {
        this->nextCarId = nextCarId;
    }

    const simtime_t& getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(const simtime_t& timestamp) {
        this->timestamp = timestamp;
    }
};

#endif /* ROUTEENTRY_H_ */
