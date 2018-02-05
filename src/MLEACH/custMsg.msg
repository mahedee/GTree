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
//enum PacketType
//{
//    RTS_DATA = 0;
//    CTS_DATA = 1;
//};

packet custMsg
{
    //Initial source id of the message
    int sourceId;
    
    //Unique message id for each message 
    int messageId;
    
 	//Message Id, which acknowledgemnt is send
    int ackMsgId;
    
     //Immediate sender id of the message
    int senderId;
    
    //Message intended receiver id
    int intendedReceiverId;
    
    //Initial packet or message generation time
    double packetGenerateTime;
    
    double packetReachToSinkTime;
    
    //Is this message a relay message or not
    bool isRelayMsg;
    
    double packetReachTime;
    
    double overheadDelay;
    
    
//    int rId;
//    int pkType @enum(PacketType);
};