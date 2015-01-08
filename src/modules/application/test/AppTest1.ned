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

package org.car2x.veins.modules.application.test;
import  org.car2x.veins.modules.application.wave.WaveAppLayer;

simple AppTest1 extends WaveAppLayer{
    parameters:
        @signal[hopLimit](type="long");
        @statistic[hopLimit](title="hop limit";source="hopLimit"; record=vector,stats;interpolationmode=none);
        
        @signal[delay](type="double");
        @statistic[delay](title="delay time";source="delay"; record=vector,stats;interpolationmode=none);
        
        @signal[distance](type="double");
        @statistic[distance](title="distance";source="distance"; record=vector,stats;interpolationmode=none);
        
        @class(AppTest1);
        @display("i=block/app2");
        
        bool sendWhileParking = default(false);//send message when the car is parked
        bool sentWSA = default(false); //tell the applayer to periodically send beacons
        volatile double WSAInterval = default(0.1s) @unit(s); //the intervall between 2 beacon messages
        double maxOffset = default(0s) @unit(s); //the maximum delay of messages sent down to avoid synchronous channel access
        bool sentWSM = default(false); //whether this applayer should send data
        volatile double SendInterval = default(0.1s) @unit(s); //repeat send packets interval
        bool positionRoute = default(false);//is route with car position information
        volatile int carId;//select the car id to simulate different scene
}
