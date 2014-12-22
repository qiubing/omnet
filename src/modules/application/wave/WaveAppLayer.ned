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

package org.car2x.veins.modules.application.wave;
import org.car2x.veins.base.modules.IBaseApplLayer;

//wave application layer base class

simple WaveAppLayer like IBaseApplLayer
{
    parameters:
        bool debug = default(false);//whether to print debug output
        int headerLength = default(0bit) @unit(bit);//header length of the application
        
        //bool sendWSA = default(false); //tell the applayer to periodically send beacons
        int WSALengthBits = default(256bit) @unit(bit); //the length of a beacon packet
        int WSAPriority = default(3); //the access category of the beacon messages
        //volatile double WSAInterval = default(1s) @unit(s); //the intervall between 2 beacon messages
        
        //double maxOffset = default(0s) @unit(s); //the maximum delay of messages sent down to avoid synchronous channel access
        //bool sendData = default(false); //whether this applayer should send data
        int dataLengthBits = default(1024bit) @unit(bit); //the length of a data packet
        bool dataOnSch = default(true); //tells the applayer whether to use a service channel for datapackets or the control channe
        int dataPriority = default(3); //the access category for data packets
        
        gates:
            input lowerLayerIn; // from wave1609.3 layer
            output lowerLayerOut; // to wave1609.3 layer
            input lowerControlIn;
            output lowerControlOut;    
}
