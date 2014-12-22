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

package org.car2x.veins.nodes;

import org.car2x.veins.base.modules.*;
import org.car2x.veins.modules.nic.INic80211p;
import org.car2x.veins.modules.nic.Nic80211p;

module User
{
    parameters:
        string applType; //type of the application layer
        string networkType = default("Wave1609_3");//type of the network layer
        string nicType = default("Nic80211p"); // type of network interface card
        string usermobilityType; //type of the mobility module
        @display("bgb=249,329");
    gates:
        input userradioIn; // gate for sendDirect
    submodules:

        nic: <nicType> like INic80211p {
            @display("p=61,264");
        }

        netwl: <networkType> like IBaseNetwLayer {
            parameters:
                @display("p=61,158;i=block/layer");
        }

        appl: <applType> like IBaseApplLayer {
            @display("p=61,50");
        }

        usermobility: <usermobilityType> like IMobility {
            @display("p=159,264");
        }

    connections:

        userradioIn --> nic.radioIn;

        nic.upperLayerOut --> netwl.lowerLayerIn;
        nic.upperControlOut --> netwl.lowerControlIn;
        netwl.lowerLayerOut --> nic.upperLayerIn;
        netwl.lowerControlOut --> nic.upperControlIn;
        netwl.upperControlOut --> appl.lowerControlIn;
        netwl.upperLayerOut --> appl.lowerLayerIn;
        appl.lowerControlOut --> netwl.upperControlIn;
        appl.lowerLayerOut --> netwl.upperLayerIn;
}
