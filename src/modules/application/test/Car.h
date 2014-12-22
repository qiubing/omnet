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

#ifndef CAR_H_
#define CAR_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include "simtime_t.h"
#include "Coord.h"
using namespace std;

//find index of array
#define BIT_WORD(n) ((n)/32)
//get number of bit value
#define BIT_MASK(n) (1<<((n)%32))
typedef unsigned int unit;

class Car {
private:
    //car id to identify the car
    int carId;
    //car current position
    Coord curPos;
    //the time stamp
    simtime_t timeStamp;

public:
    //the bitmap store the related bit set
    int bit_size ;
    unit *bitset;

    Car() {
        // TODO Auto-generated constructor stub

    }
    Car(int id,int length = 1024);
    virtual ~Car() {
        // TODO Auto-generated destructor stub
        delete bitset;
    }

    //return <0, have error; =0 successful
    //@param length is bitset have length bits
    int create_bitset(int length);

    void free_bitset();

    //return < 0, have error; =0 successful
    //@param nbit is bit index
    //@param value avalid 0 or 1

    int set_bitset(int nbit,int value);

    //return < 0 have error; =0 succesful
    //@param nbit is bit index
    int get_bitset(int nbit);


    //test nbit is 1 or 0, if 1 then return true,otherwise return false
    bool test_bitset(int nbit);

    int getCarId() const {
        return carId;
    }

    void setCarId(int carId) {
        this->carId = carId;
    }

    const Coord& getCurPos() const {
        return curPos;
    }

    void setCurPos(const Coord& curPos) {
        this->curPos = curPos;
    }

    const simtime_t& getTimeStamp() const {
        return timeStamp;
    }

    void setTimeStamp(const simtime_t& timeStamp) {
        this->timeStamp = timeStamp;
    }
};

#endif /* CAR_H_ */
