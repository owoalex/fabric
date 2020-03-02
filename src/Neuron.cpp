class Neuron;

#include <random>
#include <stdlib.h>     /* srand, rand */
#include <cmath>

class Connection {
    public:
        Neuron* source;
        Neuron* destination;
        float strength = 0;
        float usage = 0;
        Connection(Neuron* source, Neuron* destination) {
            this->source = source;
            this->destination = destination;
        }
};

#define MAX_NEURONAL_CONNECTIONS 26

class Neuron {
    private:
        Connection* connections[MAX_NEURONAL_CONNECTIONS];
        int connectionCount = 0;
        float currentActivationCumulator;
        bool activated = false;
    public:
        Neuron() {
            currentActivationCumulator = 0;
        }
        void addConnection(Neuron* destination) {
            if (connectionCount < MAX_NEURONAL_CONNECTIONS) {
                connections[connectionCount] = new Connection(this,destination);
                connectionCount++;
            } else {
                throw std::runtime_error("Too many connections");
            }
        }
        void addToAccum(float value) {
            currentActivationCumulator += value;
        }
        void transferSignals() {
            for (int i=0;i<connectionCount;i++) {
                connections[i]->usage = connections[i]->usage * 0.5;
            }
            if (activated) {
                for (int i=0;i<connectionCount;i++) {
                    float sigmoidVal = connections[i]->strength/(1.0+fabs(connections[i]->strength));
                    connections[i]->destination->addToAccum(sigmoidVal);
                    connections[i]->usage += 1;
                }
            }
        }
        void processSignals(float activationThreshold) {
            float sigmoidVal = currentActivationCumulator/(1.0+fabs(currentActivationCumulator));
            //std::cout << currentActivationCumulator << "-->" << sigmoidVal << std::endl;
            if (sigmoidVal > activationThreshold) {
                activated = true;
            } else {
                activated = false;
            }
            currentActivationCumulator = 0;
        }
        void randomAdjustV2(float strength) {
            for (int i=0;i<connectionCount;i++) {
                float randomNum = (float) ((rand() / ((float) RAND_MAX)) - 0.5) * 2.0;
                float extremity = (fabs(connections[i]->strength + randomNum) - fabs(connections[i]->strength)) / 4;
                float sigmoidExtremity = 1.0 - (extremity/(1.0+fabs(extremity)));
                connections[i]->strength += (randomNum * strength) * sigmoidExtremity;
            }
        }
        void randomAdjust(float strength) {
            for (int i=0;i<connectionCount;i++) {
                float randomNum = (float) ((rand() / ((float) RAND_MAX)) - 0.5) * 2.0;
                connections[i]->strength += (randomNum * strength);
            }
        }
        void resetState() {
            currentActivationCumulator = 0;
            activated = false;
            for (int i=0;i<connectionCount;i++) {
                connections[i]->usage = 0;
            }
        }
        void dullConnections(float strength) {
            for (int i=0;i<connectionCount;i++) {
                connections[i]->strength = connections[i]->strength * (1.0 - strength);
            }
        }
        void backPropogate(float strength) {
            for (int i=0;i<connectionCount;i++) {
                connections[i]->strength += (strength * connections[i]->usage);
            }
        }
        bool isActivated() {
            return activated;
        }
        void setActivation(bool state) {
            activated = state;
        }
};
