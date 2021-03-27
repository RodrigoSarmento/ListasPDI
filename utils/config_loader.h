
#ifndef INCLUDE_CONFIG_LOADER_H_
#define INCLUDE_CONFIG_LOADER_H_

#include <cstdio>
#include <cstdlib>
#include <event_logger.h>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace std;

class ConfigLoader
{
public:
    cv::FileStorage fs;
    /**
     * Load configuration file.
     * Uses the default path "../config_files/tum_odometry.yaml"
     */
    ConfigLoader();
    /**
     * Load configuration file.
     * @param filename: path where the config. file is located
     */
    ConfigLoader(string filename) { loadFile(filename); }

    void loadFile(const string& filename);

    /**
     * Get a int parameter in ConfigFile
     * @param parameter with the name of the parameter in configfile
     * @param parameter_int int where the value will be returned
     * @return boolean, false if the parameter is not in configfile and true if it is
     */
    bool checkAndGetInt(const string& parameter, int& parameter_int);
    /**
     * Get a float parameter in ConfigFile
     * @param parameter with the name of the parameter in configfile
     * @param parameter_float float where the value will be returned
     * @return boolean, false if the parameter is not in configfile and true if it is
     */
    bool checkAndGetFloat(const string& parameter, float& parameter_float);
    /**
     * Get a string parameter in ConfigFile
     * @param parameter with the name of the parameter in configfile
     * @param parameter_string string where the value will be returned
     * @return boolean, false if the parameter is not in configfile and true if it is
     */
    bool checkAndGetString(const string& parameter, string& parameter_string);
};

#endif