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

#ifndef CARMOVE_H_
#define CARMOVE_H_
#include <Move.h>

class CarMove: public Move {
private:
    int carId;
public:
    //CarMove() {
        // TODO Auto-generated constructor stub

    //}
    CarMove(int id = -1): carId(id) { }
    CarMove(const CarMove& cSrc): carId(cSrc.carId){ }
    virtual ~CarMove() {
        // TODO Auto-generated destructor stub
    }


    int getCarId() const {
        return carId;
    }

    void setCarId(int carId) {
        this->carId = carId;
    }
};

#endif /* CARMOVE_H_ */
