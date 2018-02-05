
    //Bubble Sort
/*    void bubble_sort (int arr[], int n)
     {
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n - i - 1; ++j)
          if (arr[j] > arr[j + 1])
         {
            int temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
          }
      }*/


    /*
    //Region for Leach Mac : Begin
    //Cluster head selection End: Choose CH from all nodes

    double p = clusterHeadPercentage;
    int optimumCluster = p * noOfWirelessNode;
    int optimumCh[5];
    int countOptimumCluster = 0;
    cModule* tempCHModule;
    Node* tempCHNode;
    //Choose a random value
    double randOptCluster = dblrand(); //Generate a random variable 0 to 1
    //Sorting CH according to energy in ascending order
    for (int i = 0; i < noOfWirelessNode; i++) {

        double tempEnergy;
        double minEnergy = 1000.00;

        tempModule = getParentModule()->getSubmodule("node", i);
        tempNode = check_and_cast<Node *>(tempModule);

        if (countOptimumCluster > optimumCluster)
            break;

        if (tempNode->type == 'C') {
            for (int ch = 0; ch < noOfWirelessNode; ch++) {
                tempCHModule = getParentModule()->getSubmodule("node", ch);
                tempCHNode = check_and_cast<Node *>(tempCHModule);
                if (tempCHNode->type == 'C') {
                    double chEnergy = (double) randOptCluster
                            / tempCHNode->batteryPower;
                    tempEnergy = std::min(minEnergy, chEnergy);
                    if (tempEnergy < minEnergy) {
                        minEnergy = tempEnergy;
                        optimumCh[countOptimumCluster] = i;
                        //Test
                    }

                }
            }

            if (countOptimumCluster == 0) {
                EV << optimumCh[countOptimumCluster] << endl;
                endSimulation();
            }
            countOptimumCluster++;
        }
    }
    //Clear list of CH
    getParentModule()->par("lstCH") = "";
    //getParentModule()->par("noOfCH") = "0";
    // noOfCH = getParentModule()->par("noOfCH");
    //noOfCH = noOfCH + 1;
    /*    if (noOfCH <= 1) {
     getParentModule()->par("lstCH") = strCH;
     } else {

     std::string prevList = getParentModule()->par("lstCH");
     getParentModule()->par("lstCH") = prevList + ","
     + strCH;

     }*/
    //Testing//

    /*
    for (int j = 0; j < (sizeof(optimumCh) / sizeof(*optimumCh)); j++) {
        EV << optimumCh[j] << endl;
    }
    endSimulation();
    //Keep optimum cluster and other make as normal node
    for (int i = 0; i < noOfWirelessNode; i++) {
        tempModule = getParentModule()->getSubmodule("node", i);
        tempNode = check_and_cast<Node*>(tempModule);
        bool isOptimalCH = false;
        for (int j = 0; j < optimumCluster; j++) {
            if (optimumCh[j] == i) {
                isOptimalCH = true;
                char buffer[33];
                itoa(i, buffer, 10);
                std::string strCH(buffer);
                std::string prevList = getParentModule()->par("lstCH");
                getParentModule()->par("lstCH") = prevList + "," + strCH;
            }
        }
        if (isOptimalCH != 1) {
            tempNode->type = 'N';
        }
    }
   */
