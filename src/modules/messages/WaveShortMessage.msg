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

cplusplus {{
#include "Coord.h"
#include "CarMove.h"
}}

class noncobject Coord;
class noncobject CarMove;

packet WaveShortMessage{
    //Version of the Wave Short Message
	int wsmVersion = 0;
	//Determine which security mechanism was used
	int securityType = 0;
	//Channel Number on which this packet was sent
	int channelNumber;
	//Data rate with which this packet was sent
	int dataRate = 1;
	//Power Level with which this packet was sent
	int priority = 3;
	//Unique number to identify the service
	int psid = 0;
	//Provider Service Context
	string psc = "Service with some Data";
	//Length of Wave Short Message
	int wsmLength;
	//Data of Wave Short Message
	string wsmData = "Some Data";

	int senderAddress = 0;
	int recipientAddress = -1;
	int serial = 0;
	Coord senderPos;
	simtime_t timestamp = 0;
	Coord direction;
	double speed = 0;
	
	CarMove carlist[10] = 0;
	
	//extend keyword for mutilhop forward
	//we intend to use WaveShortMessage to contain the WSA and WSM message 
	
	//hopLimit is like the TTL to limit the hops
	int hopLimit = 1;
	//the forward flag to indicate the packet shoule forward or not,
	//0 is stand for mandatory forward and 1 is stand for optional forward and other value stand for not forward
	int flag = 0;
	
	//The original source address use for look who is the first sender
	int originalAddress = -1;
	
	//the user priority to decide which service to serve first
	int userPriority = 0;
	
	//next forward node address use for forward
	int nextForwardAddress; 
	
	//the second service for WSA advertisement
	int psid2 = 0;
	int flag2 = 0;
	int userPriority2 = 0;
	int sch2 = 0;
	int dst2 = -1;
	int hopLimit2 = 1;
	simtime_t timeStamp2 = 0;
}