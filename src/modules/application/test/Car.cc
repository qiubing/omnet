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

#include <Car.h>

int Car::create_bitset(int length)
{
    int nword = BIT_WORD(length);
    int need_len = (nword+1)* sizeof(uint);
    bitset = (uint*)malloc(need_len);
    if(bitset == NULL) return -1;
    memset(bitset,0,need_len);
    bit_size = need_len * 8;
    return 0;
}

void Car::free_bitset()
{
    free(bitset);
}

int Car::set_bitset(int nbit,int value)
{
    if(nbit < 0 || nbit > bit_size) return -1;
    if(bitset == NULL) return -2;
    if(value != 0 && value != 1) return -3;

    int index = BIT_WORD(nbit);
    if(value == 1)
        bitset[index] |= BIT_MASK(nbit);
    else{
        bitset[index] &= ~BIT_MASK(nbit);
    }
    return 0;
}

int Car::get_bitset(int nbit)
{
    if(nbit < 0 || nbit > bit_size) return -1;
    if(bitset == NULL) return -2;

    int index = BIT_WORD(nbit);
    return (bitset[index] & BIT_MASK(nbit)) >> (nbit%32);
}

bool Car::test_bitset(int nbit)
{
    return (get_bitset(nbit) == 1)?true : false;
}

Car::Car(int id,int length)
{
    this->carId = id;
    create_bitset(length);
}
