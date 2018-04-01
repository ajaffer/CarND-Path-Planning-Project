#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <map>

using namespace std;

class GNB
{
  public:
    vector<string> possible_labels = {"left", "keep", "right"};
    int lane_width = 4;
    map<string, vector<double>> means;
    map<string, vector<double>> std_devs;

    /**
  	* Constructor
  	*/
    GNB();

    /**
 	* Destructor
 	*/
    virtual ~GNB();

    void train(vector<vector<double>> data, vector<string> labels);

    string predict(vector<double>);

    double gaussian(double v, double mean, double std_dev);
};

#endif
