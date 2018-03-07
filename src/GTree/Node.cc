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

#include <Node.h>
#include <Sink.h>
#include<RANDOM.H>
#include<custMsg_m.h>
#include<algorithm>
#include<string>
#include<stdlib.h>
#include<cUtility.h>
#include<cmath>


Define_Module(Node);

Node::Node() {
    // TODO Auto-generated constructor stub
    this->X = 0;
    this->Y = 0;
    batteryPower = 0.5;
    this->G = 0.0;
    this->type = 'N'; //Normal Node

    //Sleep time
    sleepTime = 0.10;

    Efs = 10 * 0.000000000001;
    Emp = 0.0013 * 0.000000000001;
    Do = sqrt(Efs / Emp);

    CHETx = 0.0;
    CHERx = 0.0;
    NETX = 0.0; //Node ETX
    NERX = 0.0; //Node ERX

    ETX = 50 * 0.000000001;
    ERX = 50 * 0.000000001;

    EDA = 5 * 0.000000001;

    CHIndex = 0;
    roundInterval = 1.0;

    optimalClusterFactor = 100; //Long value as inifinity

    dToBS = 109.06; //average distance to BS , calculated = 109.06//Average do = 87.70

    alpha = 0.1;
    efactor = 0.0;
    efactor2 = 0.25;
    this->thresholdEnergy = 0.50;

    //efactor =  0.0000580;
}

Node::~Node() {
}

void Node::initialize() {

    dataQueue = new cQueue;  //Queue for incoming message
    chDataQueue = new cQueue;

    wakeup = CreateCustMsg("Wakeup");
    noOfWirelessNode = getParentModule()->par("noOfWirelessNode");
    clusterHeadPercentage = getParentModule()->par("clusterHeadPercentage");

    this->netSizeX = getParentModule()->par("netSizeX");
    this->netSizeY = getParentModule()->par("netSizeY");

    SetCoordinate();

    this->K = (sqrt(noOfWirelessNode) / sqrt(2 * PI)) * sqrt(Efs / Emp)
            * ((double) netSizeX / (dToBS * dToBS));


    this->R = sqrt((double) (netSizeX * netSizeX) / (PI * K)); //==55.3215 calculated

    //Set Initial Cluster head and cluster
    if (simTime().dbl() == 0.0 && getIndex() == (noOfWirelessNode - 1)) {
        CalculateAvgDistanceToBS();

        //endSimulation();

        CalculateNeighborNode();
        //SetEnergyMarker();
        //CalculateWGTV();
        //endSimulation();

        //Must need for following
        ClusterHeadSelection(0);
        ClusterFormation(0);
    }

    /***********************************************************************
     * Initial schedule for each node
     * *********************************************************************/
    scheduleAt(
            simTime().dbl()
                    + ((double) noOfWirelessNode - getIndex())
                            / (double) noOfWirelessNode, wakeup);
}

void Node::handleMessage(cMessage *msg) {

    int networkStatus = getParentModule()->par("networkStatus");
    double lastRoundTime = getParentModule()->par("lastRoundTime");
    roundNumber = getParentModule()->par("roundNumber");
    int noOfNodeDied = getParentModule()->par("noOfNodeDied");

    /*Casting incomming msg to custMsg(Customize message)*/
    custMsg *inMsg = check_and_cast<custMsg *>(msg);

    //Handle incoming data message
    if (strcmp("DataMsg", inMsg->getFullName()) == 0) {

        //Power consumtion for incomming message
        if (this->type == 'C' && this->batteryPower >= 0) {
            //For Cluster head
            this->batteryPower = this->batteryPower - this->CHERx;

            if (this->batteryPower <= 0) {
                noOfNodeDied++;
                getParentModule()->par("noOfNodeDied") = noOfNodeDied;

                if (noOfNodeDied == 1) {
                    getParentModule()->par("fstNodeDieRound") = roundNumber;
                }

                if (noOfNodeDied == 10) {
                    getParentModule()->par("tenNodeDieRound") = roundNumber;
                }

                if (noOfNodeDied == noOfWirelessNode) {
                    getParentModule()->par("allNodeDieRound") = roundNumber;
                }
            }
        }

        inMsg->setPacketReachTime(simTime().dbl());
        chDataQueue->insert(inMsg);
    }

    //Initial setup
    if (roundNumber <= 0 && getIndex() == noOfWirelessNode - 1) {
        EV << "Initial Setting round time and round number" << endl;
        roundNumber = roundNumber + 1;
        lastRoundTime = simTime().dbl();
        getParentModule()->par("roundNumber") = roundNumber;
        getParentModule()->par("lastRoundTime") = lastRoundTime;
        getParentModule()->par("networkStatus") = 2; //Means now nodes will send data to sink.
    }

    //Send data to CH
    if (networkStatus == 2) {
        if (this->type == 'N') {
            SendDataToCH();
        }

        if (getIndex() == noOfWirelessNode - 1) {
            //Means all data sent to CH and CH will send data to Sink
            getParentModule()->par("networkStatus") = 3;
        }
    }

    //Send data to Sink
    if (networkStatus == 3) {
        if (this->type == 'C') {
            SendDataToSink();
        }

        if (getIndex() == noOfWirelessNode - 1) {
            //Means all data sent to Sink, ready for setup round
            getParentModule()->par("networkStatus") = 1;
        }
    }

    //Setup state
    if (roundNumber > 0
            && simTime().dbl() >= lastRoundTime + this->roundInterval
            && getIndex() == noOfWirelessNode - 1 && networkStatus == 1) {

        roundNumber = roundNumber + 1;
        lastRoundTime = simTime().dbl();
        getParentModule()->par("roundNumber") = roundNumber;
        getParentModule()->par("lastRoundTime") = lastRoundTime;
        //ClusterHeadSelectionOld(roundNumber);
        //OptimalClusterFormation();

        SetEnergyMarker();
        CalculateWGTV();
        ClusterHeadSelection(roundNumber);
        ClusterFormation(roundNumber);
        getParentModule()->par("networkStatus") = 2; //Means now nodes will send data to sink.
    }

    if (msg->isSelfMessage()) {
        custMsg *dataMessage = CreateCustMsg("DataMsg");
        dataQueue->insert(dataMessage);
        scheduleAt(simTime().dbl() + sleepTime, msg->dup()); //Schedule after 0.5 second
    }
}
void Node::finish() {

    WriteOneTenAllNodeDeadHistory();

    /*    cancelAndDelete(wakeup);
     cancelAndDelete(data);
     delete receiverNode;
     delete senderNode;
     delete receiverNode;
     delete tempTargetModule;
     delete tempBaseModule;*/

}

void Node::SetCoordinate() {

    int corX = 0;
    int corY = 0;
    int startAfterX = 5;
    int endBeforeX = netSizeX - 5;
    int startAfterY = 20;
    int endBeforeY = netSizeX - 5;

    //Take Rand value >10 and <=750
    do {
        corX = intrand(netSizeX); //old = 800
    } while (corX <= startAfterX || corX >= endBeforeX); //Old 10,750

    do {
        corY = intrand(netSizeY);
    } while (corY <= startAfterY || corY >= endBeforeY); //old 70, 550

    this->X = corY;
    this->Y = corY;

    getDisplayString().setTagArg("p", 0, corX);
    getDisplayString().setTagArg("p", 1, corY);
}

void Node::OptimalClusterFormation() {

    //endSimulation();

    int sortedNode[noOfWirelessNode];
    //double sorted

    for (int i = 0; i < noOfWirelessNode; i++) {
        sortedNode[i] = i;
    }

    /*    cModule *testModule = getParentModule()->getSubmodule("node", 0);
     Node    *testNode = check_and_cast<Node *>(testModule);
     testNode->optimalClusterFactor = 10.5;*/

    cModule *tempCurModule;
    Node *tempCurNode;

    cModule *tempNextModule;
    Node *tempNextNode;

    Node *NodeList[noOfWirelessNode];

    //Bubble sort algorithm : Working
    for (int i = 0; i < noOfWirelessNode; i++) {
        //int iValue = sortedNode[i]; //Node index

        for (int j = 0; j < noOfWirelessNode - i - 1; j++) {
            int jValue = sortedNode[j];
            int jNextValue = sortedNode[j + 1];

            tempCurModule = getParentModule()->getSubmodule("node", jValue);
            tempCurNode = check_and_cast<Node *>(tempCurModule);

            tempNextModule = getParentModule()->getSubmodule("node",
                    jNextValue);
            tempNextNode = check_and_cast<Node *>(tempNextModule);

            if (tempCurNode->optimalClusterFactor
                    > tempNextNode->optimalClusterFactor) {
                int temp = sortedNode[j]; //Node index
                sortedNode[j] = sortedNode[j + 1];
                sortedNode[j + 1] = temp;
            }
        }

        //Set Cluster head for first 5 optimum cluster others are consider as normal node
    }


    int optimumCluster = (int) noOfWirelessNode * clusterHeadPercentage;
    int optimumClusterCounter = 0;

    for (int i = 0; i < noOfWirelessNode; i++) {
        int sortedValue = sortedNode[i];
        EV << "sortedValue: " << sortedValue << endl;
        tempCurModule = getParentModule()->getSubmodule("node", sortedValue);
        tempCurNode = check_and_cast<Node *>(tempCurModule);
        if (optimumClusterCounter >= optimumCluster) {
            tempCurNode->type = 'N';
            continue;
        }
        if (tempCurNode->type == 'C') {
            optimumClusterCounter++;

            int noOfCH = getParentModule()->par("noOfCH");
            noOfCH = noOfCH + 1;
            getParentModule()->par("noOfCH") = noOfCH;

            char buffer[33];
            itoa(sortedValue, buffer, 10);
            std::string strCH(buffer);
            //String conversion end

            //std::string clusterIndex = std::to_string(i);

            if (noOfCH <= 1) {
                getParentModule()->par("lstCH") = strCH;
            } else {

                std::string prevList = getParentModule()->par("lstCH");
                getParentModule()->par("lstCH") = prevList + "," + strCH;

            }

        }
    }
}

void Node::ClusterHeadSelection(int roundNo) {

    getParentModule()->par("noOfCH") = 0; //Reset cluster head
    getParentModule()->par("noOfCluster") = 0; //reset no of cluster

    //Calculate noOf dead node before creating new node cluster
    int noOfNodeDied = getParentModule()->par("noOfNodeDied");
    WriteDeadNodeHistory(noOfNodeDied, roundNo - 1);
    WriteNetworkEnergyHistory(roundNo - 1);
    CountSinkPacket(roundNo - 1);

    double distance = 0.0;
    /* int noOfCH = 0;*/
    int noOfCluster = 0;

    //Initially reset all node as normal node
    for (int i = 0; i < noOfWirelessNode; i++) {

        tempModule = getParentModule()->getSubmodule("node", i);
        tempNode = check_and_cast<Node *>(tempModule);
        tempNode->type = 'N';
        tempNode->parentNodeIndex = -1;
        tempNode->CHIndex = 0;
    }



    cModule *tempCurModule;
    Node *tempCurNode;

//    cModule *tempNextModule;
//    Node *tempNextNode;

    cModule *tempNeighborModule;
    Node *tempNeighborNode;


    //Calculate parent node

    for (int i = 0; i < noOfWirelessNode; i++) {

        tempCurModule = getParentModule()->getSubmodule("node", i);
        tempCurNode = check_and_cast<Node *>(tempCurModule);

         double maxTheta = 0;
        int neighborNodeIndex = -1;
        int neighborDistance = 0;

        for (int j = 0; j < tempCurNode->neighborNode.size(); j++) {

            double neighborNodeTheta = tempCurNode->neighborTheta[j];

//            getParentModule()->par("testValueDouble") = neighborNodeTheta;
//            endSimulation();

            if (neighborNodeTheta > maxTheta) {
                maxTheta = neighborNodeTheta;
                neighborNodeIndex = tempCurNode->neighborNode[j];
                neighborDistance = tempCurNode->neighborNodeDist[j];
            }
        }

        //int neighborDistance = 0;
        //Calculate reverse angle
        double reverseTheta = acos(
                sqrt(
                        neighborDistance
                                / sqrt(
                                        pow(neighborDistance, 2)
                                                + pow(tempCurNode->batteryPower,
                                                        2))));

        if (maxTheta > reverseTheta) {
            tempCurNode->parentNodeIndex = neighborNodeIndex;
        } else {
            tempNeighborModule = getParentModule()->getSubmodule("node",
                    neighborNodeIndex);
            tempNeighborNode = check_and_cast<Node *>(tempNeighborModule);
            tempNeighborNode->parentNodeIndex = i; //Current node is the parent of neighbor node
        }
    }




    //Now for all node. If there is a node which has no parent is cluster head


    //Since it is ascending order
    cModule *tempNeigborModule;
    Node *tempNeigborNode;
    bool isCH;


    //getParentModule()->par("testValueDouble") = neighborNodeTheta;
//    endSimulation();


    for (int i = 0; i<noOfWirelessNode; i++) {

        isCH = false;
        tempCurModule = getParentModule()->getSubmodule("node", i);
        tempCurNode = check_and_cast<Node *>(tempCurModule);



        //Means there is no parent of this node
        if(tempCurNode->parentNodeIndex == -1)
        {
            isCH = true;
        }

        if (isCH == 1) {

            distance = CalculateDistanceToBS(i);
            tempCurNode->distanceToBS = distance;

//            //test purpose
//            //if(tempCurNode->distanceToBS >= tempCurNode->Do)
//            if (tempCurNode->distanceToBS >= 150) {
//                continue;
//            }

            tempCurNode->type = 'C';
            noOfCluster = getParentModule()->par("noOfCluster");
            getParentModule()->par("noOfCluster") = noOfCluster + 1;
            //EV << "Cluster No: " << noOfCluster + 1 << endl;

            int noOfCH = getParentModule()->par("noOfCH");
            noOfCH = noOfCH + 1;
            getParentModule()->par("noOfCH") = noOfCH;


            if (tempCurNode->distanceToBS > tempCurNode->Do) {

                EV << "Multipath " << endl;

                tempCurNode->CHETx = (tempCurNode->ETX + tempCurNode->EDA)
                        * (4000)
                        + tempCurNode->Emp * 4000
                                * (distance * distance * distance * distance);
            }
            if (tempCurNode->distanceToBS <= tempCurNode->Do) {

                EV << "Free space " << endl;
               // EV << "Index: " << sortedValue << "distance to bs"
               //           << tempCurNode->distanceToBS << endl;
                //endSimulation();

                tempCurNode->CHETx = (tempCurNode->ETX + tempCurNode->EDA)
                        * (4000)
                        + tempCurNode->Efs * 4000 * (distance * distance);
            }

            //EV<<"tempCurNode->CHETx: " <<tempCurNode->CHETx<<endl;
            //endSimulation();
            //Energy loss of CH to receive data
            tempCurNode->CHERx = (ERX + EDA) * 4000;

            //Added factor - Testing
            tempCurNode->CHETx = tempCurNode->CHETx - efactor;
            tempCurNode->CHERx = tempCurNode->CHERx - efactor;

            //EV<<"tempCurNode->CHETx: " <<tempCurNode->CHETx<<endl;
            //EV<<"tempCurNode->CHERx: " <<tempCurNode->CHERx<<endl;
            //endSimulation();
        }
    }

    //endSimulation();
}

/*
 void Node::ClusterHeadSelectionOld(int roundNo) {

 //Calculate noOf dead node before creating new node cluster
 int noOfNodeDied = getParentModule()->par("noOfNodeDied");
 WriteDeadNodeHistory(noOfNodeDied, roundNo - 1);
 WriteNetworkEnergyHistory(roundNo - 1);
 CountSinkPacket(roundNo - 1);

 double temp_rand;
 double p = clusterHeadPercentage;
 int roundNumber = roundNo;
 double distance = 0.0;
 int noOfCH = 0;
 int noOfCluster = 0;
 int initialCH = 0;
 double optimalRandValue = dblrand();

 //Initially reset all node as normal node
 for (int i = 0; i < noOfWirelessNode; i++) {
 tempModule = getParentModule()->getSubmodule("node", i);
 tempNode = check_and_cast<Node *>(tempModule);
 tempNode->type = 'N';
 tempNode->CHIndex = 0;
 }

 //Cluster head selection begain: Choose CH from all nodes
 for (int i = 0; i < noOfWirelessNode; i++) {
 //EV << "Id : " << i << endl;
 tempModule = getParentModule()->getSubmodule("node", i);
 tempNode = check_and_cast<Node *>(tempModule);

 if (tempNode->batteryPower > 0.0) {
 temp_rand = dblrand(); //Generate a random variable 0 to 1

 if (tempNode->G <= 0) {

 //if(temp_rand<= (p/(1-p*mod(r,round(1/p)))))  % Our recharch point
 double rechargeValue = p
 / (1 - p * (roundNumber % (int) (round)(1 / p)));

 if (temp_rand <= rechargeValue) {

 //                    noOfCH = getParentModule()->par("noOfCH");
 //                    noOfCH = noOfCH + 1;
 //                    getParentModule()->par("noOfCH") = noOfCH;

 tempNode->type = 'C'; //Cluster node
 tempNode->optimalClusterFactor = (double) optimalRandValue
 / tempNode->batteryPower;
 initialCH++;
 //Populate global CH list

 //String conversion Begin
 //                    char buffer[33];
 //                    itoa(i, buffer, 10);
 //                    std::string strCH(buffer);
 //                    //String conversion end
 //
 //                    //std::string clusterIndex = std::to_string(i);
 //
 //                    if (noOfCH <= 1) {
 //                        getParentModule()->par("lstCH") = strCH;
 //                    } else {
 //
 //                        std::string prevList = getParentModule()->par("lstCH");
 //                        getParentModule()->par("lstCH") = prevList + ","
 //                                + strCH;
 //
 //                    }

 tempNode->G = round(1 / p) - 1;

 //distance = // calculate distance this node to base station

 distance = CalculateDistanceToBS(i);

 tempNode->distanceToBS = distance;

 noOfCluster = getParentModule()->par("noOfCluster");
 getParentModule()->par("noOfCluster") = noOfCluster + 1;
 //EV << "Cluster No: " << noOfCluster + 1 << endl;

 EV << "DO: " << tempNode->Do << endl;
 EV << "distanceToBS" << tempNode->distanceToBS << endl;

 if (tempNode->distanceToBS > tempNode->Do) {
 tempNode->CHETx = (tempNode->ETX + tempNode->EDA)
 * (4000)
 + tempNode->Emp * 4000
 * (distance * distance * distance
 * distance);

 //EV<<"CHETx" << tempNode->CHETx <<endl;
 //endSimulation();
 }
 if (tempNode->distanceToBS <= tempNode->Do) {
 tempNode->CHETx = (tempNode->ETX + tempNode->EDA)
 * (4000)
 + tempNode->Efs * 4000 * (distance * distance);
 }

 //Energy loss of CH to receive data
 tempNode->CHERx = (ERX + EDA) * 4000;
 }
 }

 }
 }

 //Region for Leach Mac : Begin

 //Cluster head selection End: Choose CH from all nodes
 OptimalClusterFormation();
 }


 */


//Calculate node distance from Sink
int Node::CalculateDistanceToBS(int senderIndex) {

    int senderX, senderY, receiverX, receiverY, baseDistance;

//    cModule *senderNode;
//    cModule *receiverNode;
//    Node *tempTargetModule;

    senderNode = getParentModule()->getSubmodule("node", senderIndex);
    receiverNode = getParentModule()->getSubmodule("sink"); //Base station node
    //getParentModule()->getSubmodule("node", receiverIndex);

    tempTargetModule = check_and_cast<Node *>(senderNode);
    senderX = tempTargetModule->X;
    senderY = tempTargetModule->Y;

    tempBaseModule = check_and_cast<Sink *>(receiverNode);
    receiverX = tempBaseModule->X;
    receiverY = tempBaseModule->Y;

    baseDistance = (int) sqrt(
            (receiverX - senderX) * (receiverX - senderX)
                    + (receiverY - senderY) * (receiverY - senderY));

    return baseDistance;
}

//        EV << "Testing--Start" << endl;
//        EV << "Output-01: " << i << endl;
//        EV << "Output-02: " << rechargeValue << endl;
//        EV << "Testing--End" << endl;
//        endSimulation();

void Node::ClusterFormation(int roundNo) {

    //int noOfCluster = getParentModule()->par("noOfCluster");

    for (int i = 0; i < noOfWirelessNode; i++) {

        tempModule = getParentModule()->getSubmodule("node", i);
        tempNode = check_and_cast<Node *>(tempModule);


        //Skip loop if it is not normal node and its battery power is less than or equal to zero
        if (tempNode->type != 'N' && tempNode->batteryPower <= 0)
            continue;

        //if(noOfCluster-1 >= 1)
        int minDistance = 32000;    //infinity
        //cModule *tempCHModule;
        //Node *tempCHNode;
        //int tempDistance = 0;

        EV<<"Parent Node:" << tempNode->parentNodeIndex <<endl;

        //They are already CH. They don't have parent
        if(tempNode->parentNodeIndex == -1)
        {
            return;
        }


        //Identify parent of current node
        parentModule = getParentModule()->getSubmodule("node", tempNode->parentNodeIndex);
        parentNode = check_and_cast<Node *>(parentModule);


        if(parentNode->type == 'C')
        {
            tempNode->CHIndex = parentNode->getIndex();
        }
        else {
            int treeLevel = 0;
            while (treeLevel <= 3)    //Consider max tree level is 3
            {
                parentModule = getParentModule()->getSubmodule("node",
                        parentNode->parentNodeIndex);



                parentNode = check_and_cast<Node *>(parentModule);


                if (parentNode->type == 'C') {
                    tempNode->CHIndex = parentNode->getIndex();
                    break;
                }

                treeLevel++;
            }

        }

        minDistance = CalculateDistance(i, tempNode->CHIndex);

        if (minDistance > this->Do) {

            EV << "Node in Multipath " << endl;
            //endSimulation();

            tempNode->NETX = ETX * (4000)
                    + Emp * 4000
                            * (minDistance * minDistance * minDistance
                                    * minDistance);

            tempNode->NETX = tempNode->NETX * efactor2;
        }

        if (minDistance <= Do) {
            tempNode->NETX = ETX * (4000)
                    + Efs * 4000 * (minDistance * minDistance);
        }

        tempNode->NETX = tempNode->NETX - efactor;

    }

    //endSimulation();
}

custMsg* Node::CreateCustMsg(const char *name) {

//    if(strcmp("Beacon", name) == 0 || strcmp("Preamble", name) == 0 || strcmp("ACK", name) == 0 )
//    {
//        this->countControlPkt++;
//    }
//
    custMsg *createMsg = new custMsg(name);
    createMsg->setPacketGenerateTime(simTime().dbl());
    createMsg->setPacketReachTime(simTime().dbl());

    createMsg->setSourceId(getIndex()); // Set source node's index for the message

//    int curMsgId = getParentModule()->par("gMessageId");
//    curMsgId = curMsgId + 1;
//    createMsg->setMessageId(curMsgId);
//    getParentModule()->par("gMessageId") = curMsgId;

    return createMsg;
}

int Node::CalculateDistance(int senderIndex, int receiverIndex) {

    int senderX, senderY, receiverX, receiverY, baseDistance;

    cModule *senderNode;
    cModule *receiverNode;
    Node *tempTargetModule;

    senderNode = getParentModule()->getSubmodule("node", senderIndex);
    receiverNode = getParentModule()->getSubmodule("node", receiverIndex);

    tempTargetModule = check_and_cast<Node *>(senderNode);
    senderX = tempTargetModule->X;
    senderY = tempTargetModule->Y;

    tempTargetModule = check_and_cast<Node *>(receiverNode);
    receiverX = tempTargetModule->X;
    receiverY = tempTargetModule->Y;

    baseDistance = (int) sqrt(
            (receiverX - senderX) * (receiverX - senderX)
                    + (receiverY - senderY) * (receiverY - senderY));

    return baseDistance;
}

//Not used
void Node::TempDataSendToCH() {
    int noOfCH = getParentModule()->par("noOfCH");
    EV << "DataSendToCH, No of CH: " << noOfCH << endl;

    std::string lstCH = getParentModule()->par("lstCH");

    EV << "CH Lists: " << lstCH << endl;
    cUtility *objcUtility = new cUtility;
    std::vector<std::string> result = objcUtility->split(lstCH, ',');
    std::vector<int> vlstCH = objcUtility->convertToInt(result);

    for (int i = 0; i < vlstCH.size(); i++) {
        EV << "Converted CH: " << vlstCH[i] << endl;
    }

    //endSimulation();
    for (int ch = 0; ch < vlstCH.size(); ch++) {
        int destNodeIndex = vlstCH[ch];
        tempDestModule = getParentModule()->getSubmodule("node", destNodeIndex);
        tempDestNode = check_and_cast<Node *>(tempDestModule);

        for (int i = 0; i < noOfWirelessNode; i++) {
            tempSrcModule = getParentModule()->getSubmodule("node", i);
            tempSrcNode = check_and_cast<Node *>(tempSrcModule);
            if (tempSrcNode->CHIndex == destNodeIndex
                    && tempSrcNode->type == 'N') {
                EV << "Node index: " << i << "CH index: " << destNodeIndex
                          << endl;

                int queueLen = tempSrcNode->dataQueue->length();
                if (queueLen > 0) {
                    cObject *cObj = tempSrcNode->dataQueue->pop();
                    custMsg *qMsg = check_and_cast<custMsg *>(cObj);
                    //sendDirect(qMsg, baseModule, "radioIn");
                }

                //endSimulation();
//                cObject *cObj = dataQueue->pop();
//                custMsg *qMsg = check_and_cast<custMsg *>(cObj);
//                sendDirect(qMsg, baseModule, "radioIn");

            }
        }

    }

    //endSimulation();

}

void Node::TempDataSendToSink() {
}

void Node::SendDataToCH() {

    int noOfNodeDied = getParentModule()->par("noOfNodeDied");
    int destNodeIndex = this->CHIndex;
    tempDestModule = getParentModule()->getSubmodule("node", destNodeIndex);

    int queueLen = dataQueue->length();

    if (queueLen > 0) {
        cObject *cObj = dataQueue->pop();
        custMsg *qMsg = check_and_cast<custMsg *>(cObj);

        if (this->batteryPower > 0) {
            this->batteryPower = this->batteryPower - this->NETX;

            if (this->batteryPower <= 0) {
                noOfNodeDied++;
                getParentModule()->par("noOfNodeDied") = noOfNodeDied;

                if (noOfNodeDied == 1) {
                    getParentModule()->par("fstNodeDieRound") = roundNumber;
                }

                if (noOfNodeDied == 10) {
                    getParentModule()->par("tenNodeDieRound") = roundNumber;
                }

                if (noOfNodeDied == noOfWirelessNode) {
                    getParentModule()->par("allNodeDieRound") = roundNumber;
                }

                deadTime = simTime().dbl();
            }
        }

        //int sendPacketToCH = getParentModule()->par("sendPacketToCH");
        //getParentModule()->par("sendPacketToCH") = sendPacketToCH++;

        sendDirect(qMsg, tempDestModule, "radioIn");
    }

}

void Node::SendDataToSink() {

    int noOfNodeDied = getParentModule()->par("noOfNodeDied");
    sinkModule = getParentModule()->getSubmodule("sink");

    int queueLen = chDataQueue->length();

    int noPacketSentToSink = getParentModule()->par("noPacketSentToSink");
    getParentModule()->par("noPacketSentToSink") = noPacketSentToSink
            + queueLen;

    while (queueLen > 0) {
        cObject *cObj = chDataQueue->pop();
        custMsg *qMsg = check_and_cast<custMsg *>(cObj);
        sendDirect(qMsg, sinkModule, "radioIn");
        queueLen--;
    }

    //chDataQueue->empty();
    //->clear(); //Clear after sending data.

    if (this->batteryPower > 0) {
        this->batteryPower = this->batteryPower - this->CHETx;

        if (this->batteryPower <= 0) {
            noOfNodeDied++;
            getParentModule()->par("noOfNodeDied") = noOfNodeDied;
            deadTime = simTime().dbl();

            if (noOfNodeDied == 1) {
                getParentModule()->par("fstNodeDieRound") = roundNumber;
            }

            if (noOfNodeDied == 10) {
                getParentModule()->par("tenNodeDieRound") = roundNumber;
            }

            if (noOfNodeDied == noOfWirelessNode) {
                getParentModule()->par("allNodeDieRound") = roundNumber;
            }
        }
    }

}

void Node::WriteDeadNodeHistory(int noOfDeadNode, int roundNumber) {

    const char* filename = "02-RoundVsDeadNode.csv";
    const char* filenameAliveNode = "03-RoundVsAliveNode.csv";

    FILE *file;
    FILE *fileAliveNode;

    if (roundNumber < 1) {
        file = fopen(filename, "w+");
        fileAliveNode = fopen(filenameAliveNode, "w+");
        fprintf(file, "Round Number, Dead Node\n");
        fprintf(fileAliveNode, "Round Number, Alive Node\n");
    } else {
        //Append or update mode
        file = fopen(filename, "a+");
        fileAliveNode = fopen(filenameAliveNode, "a+");
    }

    //FILE *file = fopen(filename, "w+");

    fprintf(file, "%d,%d\n", roundNumber, noOfDeadNode);
    fclose(file);

    fprintf(fileAliveNode, "%d,%d\n", roundNumber,
            noOfWirelessNode - noOfDeadNode);
    fclose(fileAliveNode);

}

void Node::WriteOneTenAllNodeDeadHistory() {
    const char* filename = "01-1st10thAllDie.csv";
    //const char* filenameAliveNode = "03-RoundVsAliveNode.csv";

    FILE *file;
    FILE *fileAliveNode;

    file = fopen(filename, "w+");
    fprintf(file, "Dead Node, Round Number\n");

    int fstNodeDieRound = getParentModule()->par("fstNodeDieRound");
    fprintf(file, "%d,%d\n", 1, fstNodeDieRound);

    int tenNodeDieRound = getParentModule()->par("tenNodeDieRound");
    fprintf(file, "%d,%d\n", 10, tenNodeDieRound);

    int allNodeDieRound = getParentModule()->par("allNodeDieRound");
    ;
    fprintf(file, "%d,%d\n", 50, allNodeDieRound);

    fclose(file);
}

void Node::WriteNetworkEnergyHistory(int roundNumber) {

    //if(roundNumber <1 ) return;
    double initialBatteryPower = 0.5;
    double netInitialEnergy = initialBatteryPower * (double) noOfWirelessNode;
    double netRemainingEnergy = 0.0;
    double netConsumptionEnergy = 0.0;

    //EV<<"Net initial energy: " << netInitialEnergy <<endl;
    //endSimulation();

    for (int i = 0; i < noOfWirelessNode; i++) {
        calModule = getParentModule()->getSubmodule("node", i);
        calNode = check_and_cast<Node *>(calModule);
        netRemainingEnergy = netRemainingEnergy + calNode->batteryPower;

    }


    if(netRemainingEnergy <= 0.0)
    {
        endSimulation();
    }

    if(netRemainingEnergy <= thresholdEnergy)
    {
        netRemainingEnergy = 0.0;
        getParentModule()->par("allNodeDieRound") = roundNumber;

        for (int i = 0; i < noOfWirelessNode; i++) {
            calModule = getParentModule()->getSubmodule("node", i);
            calNode = check_and_cast<Node *>(calModule);
            calNode->batteryPower = 0;

        }

        //endSimulation();
    }

    getParentModule()->par("totalRemainingEnergy") = netRemainingEnergy;
    getParentModule()->par("avgRemainingEnergy") = netRemainingEnergy
            / (double) noOfWirelessNode;



    netConsumptionEnergy = netInitialEnergy - netRemainingEnergy;

    const char* filenameRemainingEnergy = "04-RoundVsRemainingEnergy.csv";
    const char* filenameEnergyConsumption = "05-RoundVsEnergyConsumption.csv";

    FILE *fileRemainingEnergy;
    FILE *fileEnergyConsumption;

    if (roundNumber < 1) {
        fileRemainingEnergy = fopen(filenameRemainingEnergy, "w+");
        fileEnergyConsumption = fopen(filenameEnergyConsumption, "w+");
        fprintf(fileRemainingEnergy,
                "Round Number, Network Remaining Energy\n");
        fprintf(fileEnergyConsumption,
                "Round Number, Total Energy Consumption\n");
    } else {
        //Append or update mode
        fileRemainingEnergy = fopen(filenameRemainingEnergy, "a+");
        fileEnergyConsumption = fopen(filenameEnergyConsumption, "a+");
    }

    fprintf(fileRemainingEnergy, "%d,%lf\n", roundNumber, netRemainingEnergy);
    fclose(fileRemainingEnergy);

    fprintf(fileEnergyConsumption, "%d,%lf\n", roundNumber,
            netConsumptionEnergy);
    fclose(fileEnergyConsumption);
    //FILE *file = fopen(filename, "w+");

}

void Node::CountSinkPacket(int roundNumber) {
    if (roundNumber <= 0) {
        return;
    }

    int noPacketSentToSink = getParentModule()->par("noPacketSentToSink");
    //getParentModule()->par("noPacketSentToSink") = noPacketSentToSink + queueLen;

    int sinkPackets = 0;

    calSinkModule = getParentModule()->getSubmodule("sink");
    calSinkNode = check_and_cast<Sink *>(calSinkModule);
    sinkPackets = calSinkNode->noDataInSink;
    //noDataInSink
    //calSinkNode->sinkDataQueue->getLength();

    //sinkPackets = ;

    const char* filenameSinkPackets = "06-RoundVsSinkPackets.csv";
    FILE *fileSinkPackets;

    if (roundNumber < 1) {
        fileSinkPackets = fopen(filenameSinkPackets, "w+");
        fprintf(fileSinkPackets, "Round Number, Number of packets in Sink\n");
    } else {
        //Append or update mode
        fileSinkPackets = fopen(filenameSinkPackets, "a+");
    }

    fprintf(fileSinkPackets, "%d,%d\n", roundNumber, noPacketSentToSink);
    fclose(fileSinkPackets);

}

void Node::CalculateNeighborNode() {

    int neighborDistance = 0;

    for (int i = 0; i < noOfWirelessNode; i++) {
        cModule *curModule;
        Node *curNode;

        curModule = getParentModule()->getSubmodule("node", i);
        curNode = check_and_cast<Node *>(curModule);

        for (int j = 0; j < noOfWirelessNode; j++) {
            if (i == j) {
                continue;
            }

            neighborDistance = CalculateDistance(i, j);
            if (neighborDistance <= curNode->R) {

                curNode->neighborNode.push_back(j);
                curNode->neighborNodeDist.push_back(neighborDistance);

                cModule *neighborModule;
                Node *neighborNode;

                neighborModule = getParentModule()->getSubmodule("node", j);
                neighborNode = check_and_cast<Node *>(neighborModule);
                curNode->neighborNodeEnergy.push_back(
                        neighborNode->batteryPower);

                //calulate theta here
                //acos means cos inverse
                double theta = acos(sqrt(neighborDistance / sqrt( pow(neighborDistance,2) + pow(neighborNode->batteryPower,2))));

                curNode->neighborTheta.push_back(theta);
            }


//          EV<<"Hello world"<<endl;
//          EV<<"Neighbor no: " << curNode->neighborNode.size();

        }

    }


   //endSimulation();
}


void Node::SetEnergyMarker() {

    //int roundNumber =  getParentModule()->par("roundNumber");
    int sortedNode[noOfWirelessNode];

    for (int i = 0; i < noOfWirelessNode; i++) {
        sortedNode[i] = i;
    }

    cModule *tempCurModule;
    Node *tempCurNode;

    cModule *tempNextModule;
    Node *tempNextNode;

    //Node *NodeList[noOfWirelessNode];

    //Bubble sort algorithm (according to energy) : Working
    for (int i = 0; i < noOfWirelessNode; i++) {
        for (int j = 0; j < noOfWirelessNode - i - 1; j++) {
            int jValue = sortedNode[j];
            int jNextValue = sortedNode[j + 1];

            tempCurModule = getParentModule()->getSubmodule("node", jValue);
            tempCurNode = check_and_cast<Node *>(tempCurModule);

            tempNextModule = getParentModule()->getSubmodule("node",
                    jNextValue);
            tempNextNode = check_and_cast<Node *>(tempNextModule);

            if (tempCurNode->batteryPower > tempNextNode->batteryPower) {
                int temp = sortedNode[j]; //Node index
                sortedNode[j] = sortedNode[j + 1];
                sortedNode[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < noOfWirelessNode; i++) {
        int sortedValue = sortedNode[i];
        tempCurModule = getParentModule()->getSubmodule("node", sortedValue);
        tempCurNode = check_and_cast<Node *>(tempCurModule);
        tempCurNode->energyMarker = i + 1;

        EV << "Node: " << sortedValue << " Energy Marker: "
                  << tempCurNode->energyMarker << endl;
    }

    //endSimulation();
}

void Node::CalculateAvgDistanceToBS() {

    //double dToBS = getParentModule()->par("dToBS");
    double totalDistance = 0.0;
    double avgDistance = 0.0;
    int distanceToBS = 0.0;
    for (int i = 0; i < noOfWirelessNode; i++) {
        distanceToBS = CalculateDistanceToBS(i);
        totalDistance = totalDistance + distanceToBS;
    }

    EV << "totalDistance: " << totalDistance << endl;
    avgDistance = totalDistance / (double) noOfWirelessNode;
    getParentModule()->par("dToBS") = avgDistance;
    //endSimulation();
}

void Node::CalculateWGTV() {
    //int roundNumber =  getParentModule()->par("roundNumber");

    for (int i = 0; i < noOfWirelessNode; i++) {
        cModule *curModule;
        Node *curNode;

        curModule = getParentModule()->getSubmodule("node", i);
        curNode = check_and_cast<Node *>(curModule);

        //WGTV = RE + Alpha *NRBCNT;
        curNode->WGTV = curNode->energyMarker
                + curNode->alpha * curNode->neighborNode.size();
    }

}
