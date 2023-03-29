#pragma once

#include<vector>

class Neuron {

private:
    double bias;
    std::vector<double> weights;

public:
    Neuron(int input_size);


    double activate(std::vector<double>& inputs);
    void train(std::vector<double>& inputs, double error, double learning_rate);

    double GetBias() {
        return bias;
    }
    std::vector<double> GetWeights() {
        return weights;
    }


    ~Neuron() = default;
};

