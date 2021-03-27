#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <config_loader.h>
#include <event_logger.h>

using namespace std;
using namespace cv;

void ConfigLoader::loadFile(const string& filename)
{
    fs.open(filename, FileStorage::READ); // Reading config file
    if (fs.isOpened() == false) {
        printf("[config_loader.cpp] ERROR: ConfigFile couldn't be opened, check if your path is right\n");
        exit(0);
    }
}

bool ConfigLoader::checkAndGetInt(const string& parameter, int& parameter_int)
{
    if (fs[parameter].empty()) {
        printf(
            "[config_loader.cpp] INFO: Parameter %s couldn't be loaded, check your ConfigFile.yaml\n",
            parameter.c_str());
        return false;
    }
    else {
        fs[parameter] >> parameter_int;
        printf("[config_loader.cpp] INFO: Parameter %s = %i\n", parameter.c_str(), parameter_int);
        return true;
    }
}

bool ConfigLoader::checkAndGetFloat(const string& parameter, float& parameter_float)
{
    if (fs[parameter].empty()) {
        printf(
            "[config_loader.cpp] INFO: Parameter %s couldn't be loaded, check your ConfigFile.yaml\n",
            parameter.c_str());
        return false;
    }
    else {
        fs[parameter] >> parameter_float;
        printf("[config_loader.cpp] INFO: Parameter %s = %f\n", parameter.c_str(), parameter_float);
        return true;
    }
}

bool ConfigLoader::checkAndGetString(const string& parameter, string& parameter_string)
{
    if (fs[parameter].empty()) {
        printf(
            "[config_loader.cpp] INFO: Parameter %s couldn't be loaded, check your ConfigFile.yaml\n",
            parameter.c_str());
        return false;
    }
    else {
        fs[parameter] >> parameter_string;
        printf("[config_loader.cpp] INFO: Parameter %s = %s\n", parameter.c_str(), parameter_string.c_str());
        return true;
    }
}
