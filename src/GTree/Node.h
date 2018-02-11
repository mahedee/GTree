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

#ifndef NODE_H_
#define NODE_H_

#include <omnetpp.h>
#include <Sink.h>
#include <custMsg_m.h>
#include<vector>

class Node: public cSimpleModule {

public:
    int X;
    int Y;
    int netSizeX;
    int netSizeY;

    int noOfWirelessNode;
    double batteryPower;
    double G;
    double clusterHeadPercentage;
    char type;
    double distanceToBS;
    double sleepTime;

    double Efs; //10*0.000000000001;
    double Emp; //0.0013*0.000000000001;
    double Do;
    double CHETx;
    double CHERx;
    double NETX; //Node ETX
    double NERX; //Node ERX

    double ETX;
    double ERX;

    double EDA;
    int CHIndex;
    double roundInterval;
    double deadTime;
    int roundNumber;
    double optimalClusterFactor;

    int M; //Network size
    double R; //Radius
    //double PI; //const value 3.14
    double K; //optimum cluster
    double dToBS;
    int energyMarker;
    double alpha;
    double efactor;

    std::vector<int> neighborNode;
    std::vector<double> neighborNodeDist;
    std::vector<double> neighborNodeEnergy;
    std::vector<double> neighborTheta;

    int parentNodeIndex;

    double WGTV;

    custMsg *wakeup;
    custMsg *data;
    cQueue *dataQueue;
    cQueue *chDataQueue;

    cModule *senderNode;
    cModule *receiverNode;
    Node *tempTargetModule;
    Sink *tempBaseModule;

    cModule *tempModule;
    Node *tempNode;

    cModule *tempSrcModule;
    Node *tempSrcNode;

    cModule *tempDestModule;
    Node *tempDestNode;

    cModule *calModule;
    Node *calNode;

    cModule *calSinkModule;
    Sink *calSinkNode;

    cModule *sinkModule;

    Node();
    virtual ~Node();

private:
    void SetCoordinate();
    void CalculateAvgDistanceToBS();
    void CalculateNeighborNode();
    void SetEnergyMarker();
    void CalculateWGTV();
    void ClusterHeadSelection(int roundNo);

    //void ClusterHeadSelectionOld(int roundNo);
    void ClusterFormation(int roundNo);

    int CalculateDistanceToBS(int nodeindex);
    custMsg* CreateCustMsg(const char *name);
    int CalculateDistance(int senderIndex, int receiverIndex);

    void SendDataToCH();
    void SendDataToSink();

    void WriteDeadNodeHistory(int noOfDeadNode, int roundNumber);
    void WriteOneTenAllNodeDeadHistory();
    void WriteNetworkEnergyHistory(int roundNumber);
    void CountSinkPacket(int roundNumber);


    void TempDataSendToCH();
    void TempDataSendToSink();
    void OptimalClusterFormation();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void finish();
};

#endif /* NODE_H_ */
