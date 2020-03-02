#include "Neuron.cpp"
#include <vector>
#include <random>
#include <stdlib.h>     /* srand, rand */
#include <cmath>
#include <time.h>       /* time */

class Fabric {
    private:
        int fabricSizeX = 0;
        int fabricSizeY = 0;
        int fabricSizeZ = 0;
        Neuron** neurons;
    public:
        Fabric(int sizex, int sizey, int sizez) {
            srand(time(NULL));
            fabricSizeX = sizex;
            fabricSizeY = sizey;
            fabricSizeZ = sizez;
            neurons = new Neuron*[fabricSizeX * fabricSizeY * fabricSizeZ];
            
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                setNeuron(x,y,z,new Neuron());
            }}}

            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                for (int xo=-1;xo<=1;xo++) { for (int yo=-1;yo<=1;yo++) { for (int zo=-1;zo<=1;zo++) {
                    if ((x+xo >= 0 && x+xo < fabricSizeX) && (y+yo >= 0 && y+yo < fabricSizeY) && (z+zo >= 0 && z+zo < fabricSizeZ)) {
                        if (!((xo == 0) && (yo == 0) && (zo == 0))) {
                            getNeuron(x,y,z)->addConnection(getNeuron(x+xo,y+yo,z+zo));
                        }
                    }
                }}}
            }}}
        }
        void setNeuron(int x, int y, int z, Neuron* neuron) {
            neurons[(x * fabricSizeY * fabricSizeZ) + (y * fabricSizeZ) + z] = neuron;
        }
        Neuron* getNeuron(int x, int y, int z) {
            return neurons[(x * fabricSizeY * fabricSizeZ) + (y * fabricSizeZ) + z];
        }
        void timeStep(float activationThreshold) {
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->transferSignals();
            }}}
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->processSignals(activationThreshold);
            }}}
        }
        void resetNeuronStates() {
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->resetState();
            }}}
        }
        void randomAdjustV1(float strength) {
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->randomAdjust(strength);
            }}}
        }
        void randomAdjust(float strength) {
            float randomNum;
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                randomNum = (float) (rand() / ((float) RAND_MAX));
                if (randomNum > (1.0-(1.0/(float)getSize(-1)))) {
                    getNeuron(x,y,z)->randomAdjust(strength);
                }
            }}}
        }
        void backPropogate(float strength) {
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->backPropogate(strength);
            }}}
        }
        void dullConnections(float strength) {
            for (int x=0;x<fabricSizeX;x++) { for (int y=0;y<fabricSizeY;y++) { for (int z=0;z<fabricSizeZ;z++) {
                getNeuron(x,y,z)->dullConnections(strength);
            }}}
        }
        int getSize(int index) {
            switch (index) {
                case 0:
                    return fabricSizeX;
                    break;
                case 1:
                    return fabricSizeY;
                    break;
                case 2:
                    return fabricSizeZ;
                    break;
                default:
                    return pow(pow(fabricSizeX,2) + pow(fabricSizeY,2) + pow(fabricSizeZ,2),0.5);
                    break;
            }
        }
        void print() {
            for (int y=0;y<fabricSizeY;y++) { 
                for (int z=0;z<fabricSizeZ;z++) {
                    for (int x=0;x<fabricSizeX;x++) { 
                        if (getNeuron(x,y,z)->isActivated()) {
                            std::cout << "#";
                        } else {
                            std::cout << "-";
                        }
                    }
                    std::cout << " | ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
};
