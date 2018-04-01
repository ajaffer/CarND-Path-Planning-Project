#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <map>
#include <vector>
#include "classifier.h"
#include <iomanip>

/**
 * Initializes GNB
 */
GNB::GNB()
{
}

GNB::~GNB() {}

void GNB::train(vector<vector<double>> data, vector<string> labels)
{

    /*
		Trains the classifier with N data points and labels.

		INPUTS
		data - array of N observations
		  - Each observation is a tuple with 4 values: s, d, 
		    s_dot and d_dot.
		  - Example : [
			  	[3.5, 0.1, 5.9, -0.02],
			  	[8.0, -0.3, 3.0, 2.2],
			  	...
		  	]

		labels - array of N labels
		  - Each label is one of "left", "keep", or "right".
	*/

    vector<string> actions = {"left", "keep", "right"};
    // std::map<std::string, vector<double>> means;
    vector<double> left;
    left.push_back(0.0);
    left.push_back(0.0);
    left.push_back(0.0);
    left.push_back(0.0);
    left.push_back(0.0);
    this->means["left"] = left;

    vector<double> keep;
    keep.push_back(0.0);
    keep.push_back(0.0);
    keep.push_back(0.0);
    keep.push_back(0.0);
    keep.push_back(0.0);
    this->means["keep"] = keep;

    vector<double> right;
    right.push_back(0.0);
    right.push_back(0.0);
    right.push_back(0.0);
    right.push_back(0.0);
    right.push_back(0.0);
    this->means["right"] = right;

    // std::map<std::string, vector<double>> std_devs;
    vector<double> std_left;
    std_left.push_back(0.0);
    std_left.push_back(0.0);
    std_left.push_back(0.0);
    std_left.push_back(0.0);
    std_left.push_back(0.0);
    this->std_devs["left"] = std_left;

    vector<double> std_keep;
    std_keep.push_back(0.0);
    std_keep.push_back(0.0);
    std_keep.push_back(0.0);
    std_keep.push_back(0.0);
    std_keep.push_back(0.0);
    this->std_devs["keep"] = std_keep;

    vector<double> std_right;
    std_right.push_back(0.0);
    std_right.push_back(0.0);
    std_right.push_back(0.0);
    std_right.push_back(0.0);
    std_right.push_back(0.0);
    this->std_devs["right"] = std_right;

    //gather coord sums
    for (int i = 0; i < data.size(); i++)
    {
        vector<double> coords = data[i];
        string action = labels[i];
        vector<double> means_for_label = this->means[action];

        // std::cout << "Label: " << action << std::endl;
        // std::cout << "Before"<< std::endl;
        // std::cout << means_for_label[0] << " " << means_for_label[1] << " " << means_for_label[2] << " " << means_for_label[3] << " " << means_for_label[4] << std::endl;

        means_for_label[0] += coords[0];
        means_for_label[1] += coords[1];
        // means_for_label[1] += std::fmod(coords[1], 4);
        means_for_label[2] += coords[2];
        means_for_label[3] += coords[3];
        means_for_label[4]++;

        // std::cout << "After" << std::endl;
        // std::cout << means_for_label[0] << " " << means_for_label[1] << " " << means_for_label[2] << " " << means_for_label[3] << " " << means_for_label[4] << std::endl;
        this->means[action] = means_for_label;
    }

    for (int i = 0; i < possible_labels.size(); i++)
    {
        string action = possible_labels[i];
        vector<double> means_for_label = this->means[action];
        std::cout << "total sum for " << action << ":" << std::endl;
        std::cout << "s" << std::setw(10) << "d" << std::setw(10) << "s`" << std::setw(10) << "d`" << std::setw(10) << "count" << std::endl;
        std::cout << means_for_label[0] << std::setw(10) << means_for_label[1] << std::setw(10) << means_for_label[2] << std::setw(10) << means_for_label[3] << std::setw(10) << means_for_label[4] << std::endl;
    }

    //cacluate means
    for (int i = 0; i < actions.size(); i++)
    {
        string action = actions[i];
        vector<double> means_for_label = this->means[action];
        means_for_label[0] /= means_for_label[4];
        means_for_label[1] /= means_for_label[4];
        means_for_label[2] /= means_for_label[4];
        means_for_label[3] /= means_for_label[4];

        // std::cout << "Label: " << action << std::endl;
        // std::cout << means_for_label[0] << " " << means_for_label[1] << " " << means_for_label[2] << " " << means_for_label[3] << std::endl;
        this->means[action] = means_for_label;
    }

    for (int i = 0; i < possible_labels.size(); i++)
    {
        string action = possible_labels[i];
        vector<double> means_for_label = this->means[action];
        std::cout << "mean for " << action << ":" << std::endl;
        std::cout << "s" << std::setw(10) << "d" << std::setw(10) << "s`" << std::setw(10) << "d`" << std::setw(10) << "count" << std::endl;
        std::cout << means_for_label[0] << std::setw(10) << means_for_label[1] << std::setw(10) << means_for_label[2] << std::setw(10) << means_for_label[3] << std::setw(10) << means_for_label[4] << std::endl;
    }

    // 	std::cout << "gather std_dev sums" << std::endl;

    // gather std_dev sums
    for (int i = 0; i < data.size(); i++)
    {
        vector<double> coords = data[i];
        string action = labels[i];
        vector<double> std_devs_for_label = this->std_devs[action];
        vector<double> means_for_label = means[action];

        // std::cout << "Label: " << action << std::endl;
        // std::cout << "Before" << std::endl;
        // std::cout << std_devs_for_label[0] << " " << std_devs_for_label[1] << " " << std_devs_for_label[2] << " " << std_devs_for_label[3] << " " << std_devs_for_label[4] << std::endl;

        std_devs_for_label[0] += pow(coords[0] - means_for_label[0], 2);
        std_devs_for_label[1] += pow(coords[1] - means_for_label[1], 2);
        std_devs_for_label[2] += pow(coords[2] - means_for_label[2], 2);
        std_devs_for_label[3] += pow(coords[3] - means_for_label[3], 2);
        std_devs_for_label[4]++;

        // std::cout << "After" << std::endl;
        // std::cout << std_devs_for_label[0] << " " << std_devs_for_label[1] << " " << std_devs_for_label[2] << " " << std_devs_for_label[3] << " " << std_devs_for_label[4] << std::endl;
        this->std_devs[action] = std_devs_for_label;
    }

    std::cout << "std_dev" << std::endl;
    //cacluate std_dev
    for (int i = 0; i < actions.size(); i++)
    {
        string action = actions[i];
        vector<double> std_devs_for_label = this->std_devs[action];
        std_devs_for_label[0] = pow(std_devs_for_label[0] / std_devs_for_label[4], 0.5);
        std_devs_for_label[1] = pow(std_devs_for_label[1] / std_devs_for_label[4], 0.5);
        std_devs_for_label[2] = pow(std_devs_for_label[2] / std_devs_for_label[4], 0.5);
        std_devs_for_label[3] = pow(std_devs_for_label[3] / std_devs_for_label[4], 0.5);

        std::cout << "Label: " << action << std::endl;
        std::cout << std_devs_for_label[0] << " " << std_devs_for_label[1] << " " << std_devs_for_label[2] << " " << std_devs_for_label[3] << std::endl;
        this->std_devs[action] = std_devs_for_label;
    }

    for (int i = 0; i < possible_labels.size(); i++)
    {
        string action = possible_labels[i];
        vector<double> std_devs_for_label = this->std_devs[action];
        std::cout << "std dev for " << action << ":" << std::endl;
        std::cout << "s" << std::setw(10) << "d" << std::setw(10) << "s`" << std::setw(10) << "d`" << std::setw(10) << "count" << std::endl;
        std::cout << std_devs_for_label[0] << std::setw(10) << std_devs_for_label[1] << std::setw(10) << std_devs_for_label[2] << std::setw(10) << std_devs_for_label[3] << std::setw(10) << std_devs_for_label[4] << std::endl;
    }
}

double GNB::gaussian(double v, double mean, double std_dev)
{
    double num = exp(-1 * (pow(v - mean, 2) / (2 * pow(std_dev, 2))));
    double denom = (pow(2 * M_PI * pow(std_dev, 2), 0.5));
    return 1 / denom * num;
}

string GNB::predict(vector<double> sample)
{
    /*
		Once trained, this method is called and expected to return 
		a predicted behavior for the given observation.

		INPUTS

		observation - a 4 tuple with s, d, s_dot, d_dot.
		  - Example: [3.5, 0.1, 8.5, -0.2]

		OUTPUT

		A label representing the best guess of the classifier. Can
		be one of "left", "keep" or "right".
		"""
		# TODO - complete this
	*/

    std::map<string, double> probs;
    string predicted_label = "";
    double sum = 0.0;

    for (int i = 0; i < this->possible_labels.size(); i++)
    {
        string label = this->possible_labels[i];
        vector<double> _std_devs = this->std_devs[label];
        vector<double> _means = this->means[label];

        double prob = 1.0;
        for (int j = 0; j < sample.size(); j++)
        {
            // if (j == 1) {
            //     prob *= this->gaussian(label, std::fmod(sample[i], 4), _means[j], _std_devs[j]);
            // } else {
            prob *= this->gaussian(sample[j], _means[j], _std_devs[j]);
            std::cout << "prob:\t" << prob << std::endl;
            // }
        }
        probs[label] = prob;
        sum += prob;
    }

    for (int i = 0; i < this->possible_labels.size(); i++)
    {
        string label = this->possible_labels[i];
        probs[label] /= sum;
    }

    double max = 0.0;
    for (int i = 0; i < this->possible_labels.size(); i++)
    {
        string label = this->possible_labels[i];
        if (probs[label] > max)
        {
            max = probs[label];
            predicted_label = label;
        }
    }

    std::cout << "predicted_label: " << predicted_label << " prob: " << max << std::endl;

    return predicted_label;
}
