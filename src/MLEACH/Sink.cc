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

#include <Sink.h>
#include <custMsg_m.h>

Define_Module(Sink);
Sink::Sink() {
    // TODO Auto-generated constructor stub

    sleepTime = 0.5;
    noDataInSink = 0;
}

Sink::~Sink() {
    // TODO Auto-generated destructor stub

}

void Sink::initialize() {
    sinkDataQueue = new cQueue;
    custMsg *wakeup = CreateCustMsg("Wakeup");

            //new cMessage("Wakeup");
    X = getParentModule()->par("sinkX");
    Y = getParentModule()->par("sinkY");

    EV<<"SinkX: " <<X<<endl;
    EV<<"SinkY: " <<Y<<endl;

    SetCoordinate();

    /***********************************************************************
     * Initial schedule for each node
     * *********************************************************************/
    scheduleAt(simTime().dbl() + 0.5, wakeup);
}

custMsg* Sink::CreateCustMsg(const char* name) {
    custMsg *createMsg = new custMsg(name);
    createMsg->setPacketGenerateTime(simTime().dbl());
    createMsg->setPacketReachTime(simTime().dbl());
    createMsg->setSourceId(getIndex()); // Set source node's index for the message
    return createMsg;
}

void Sink::handleMessage(cMessage* msg) {



    //Casting incomming msg to custMsg(Customize message)
    custMsg *inMsg = check_and_cast<custMsg *>(msg);
    //Handle incoming data message
    if (strcmp("DataMsg", inMsg->getFullName()) == 0) {
        noDataInSink++;
        inMsg->setPacketReachTime(simTime().dbl());
        sinkDataQueue->insert(inMsg);
    }



    if (msg->isSelfMessage()) {
        scheduleAt(simTime().dbl() + sleepTime, msg->dup()); //Schedule after 0.5 second
    }
}


void Sink::SetCoordinate() {
    getDisplayString().setTagArg("p", 0, X);
    getDisplayString().setTagArg("p", 1, Y);

    //Display nodes icon from code
    getDisplayString().setTagArg("i", 0, "device/antennatower");
}
