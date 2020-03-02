/*

fabric v0.0.0
Copyright (c) Alex Baldwin 2019-2020.

fabric is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License,
version 2 exclusively, as published by the Free Software Foundation.

fabric is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fabric. If not, see:
https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt/

*/

#include <iostream>

#include "json.hpp"
#include <stdlib.h>
#include <cstring>

#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST
#if defined(_WIN32)
#include <direct.h>   // _mkdir
#endif

#include "Fabric.cpp"

using json = nlohmann::json;


std::string dataFolder;
std::string fabricFile;

bool isDirExist(const std::string& path) {
#if defined(_WIN32)
    struct _stat info;
    if (_stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & _S_IFDIR) != 0;
#else 
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
#endif
}

bool makePath(const std::string& path) {
#if defined(_WIN32)
    int ret = _mkdir(path.c_str());
#else
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
#endif
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            int pos = path.find_last_of('/');
            if (pos == (int) std::string::npos)
#if defined(_WIN32)
                pos = path.find_last_of('\\');
            if (pos == (int) std::string::npos)
#endif
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        // now, try to create again
#if defined(_WIN32)
        return 0 == _mkdir(path.c_str());
#else 
        return 0 == mkdir(path.c_str(), mode);
#endif

    case EEXIST:
        // done!
        return isDirExist(path);

    default:
        return false;
    }
}

int main(int argc, char** argv) {
    std::cout << "fabric v0.0.0\nCopyright (c) Alex Baldwin 2019-2020.\n\n";
    std::string fabricSize = "";
    std::string trainingDataFile = "";
    dataFolder = std::string(getenv("HOME")) + "/data";
    float activationThreshold = 0.5;
    float learningRate = 0.5;
    float novelizingRate = 0.5;

    int mode = 2;
    /*  MODES
        0 = Configuration Only
        1 = Training
        2 = Run (Discard Changes)
        3 = Hybrid train/run
    */

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f") {
            i++;
            std::string arg2 = argv[i];
            dataFolder = arg2;
            continue;
        }
        if (arg == "-g") {
            i++;
            std::string arg2 = argv[i];
            fabricSize = arg2;
            mode = 0;
            continue;
        }
        if (arg == "-t") {
            i++;
            std::string arg2 = argv[i];
            trainingDataFile = arg2;
            mode = 1;
            continue;
        }
        //bootVmImmediate = arg;
    }
    
    fabricFile = dataFolder + "/fabric.json";
    
    std::cout << "Data folder                :" << dataFolder << "\n";
    std::cout << "Fabric file                :" << fabricFile << "\n";
    
    if (fabricSize.length() > 0) {
        std::cout << "Fabric size to generate    :" << fabricSize << "\n";
    }
    
    if (trainingDataFile.length() > 0) {
        std::cout << "Training data file         :" << trainingDataFile << "\n";
    }
    
    switch(mode) {
        case 0: {
            std::cout << "\nConfiguration only mode\n";
            break;
        }
        case 1: {
            std::cout << "\nTraining mode\n";
            break;
        }
        case 2: {
            std::cout << "\nRun network mode\n";
            break;
        }
    }

    makePath(dataFolder);
    
    Fabric* fabric = new Fabric(4,4,3);
    for(int i=0;i<(1024*1024);i++) {
        fabric->resetNeuronStates();
        fabric->getNeuron(2,2,0)->setActivation(true);
        fabric->getNeuron(1,1,0)->setActivation(true);
        for(int j=0;j<(int) ceil(fabric->getSize(-1));j++) {
            fabric->timeStep(activationThreshold);
        }
        //std::cout << floor(fabric->getSize(-1));
        fabric->print();
        float rewardFunction = 0;
        if (fabric->getNeuron(1,1,2)->isActivated()) {
            rewardFunction += 1;
        } else {
            rewardFunction -= 1;
        }
        if (!(fabric->getNeuron(1,2,2)->isActivated())) {
            rewardFunction += 1;
        } else {
            rewardFunction -= 1;
        }
        if (!(fabric->getNeuron(2,1,2)->isActivated())) {
            rewardFunction += 1;
        } else {
            rewardFunction -= 1;
        }
        if (fabric->getNeuron(2,2,2)->isActivated()) {
            rewardFunction += 1;
        } else {
            rewardFunction -= 1;
        }
        rewardFunction = rewardFunction;
        fabric->backPropogate(learningRate * rewardFunction);
        fabric->randomAdjust(novelizingRate);
        fabric->dullConnections(novelizingRate/5000);
    }
    
    return 1;
}
