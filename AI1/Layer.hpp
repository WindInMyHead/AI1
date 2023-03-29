#pragma once

#include"Neuron.hpp"

class Layer {

private:
    std::vector<Neuron> neurons;

public:
    Layer() = default;
    Layer(int input_size, int num_neurons);


    std::vector<double> activate(std::vector<double>& inputs);
    void train(std::vector<double>& inputs, std::vector<double>& errors, double learning_rate);

    std::vector<Neuron> GetNeurons() {
        return neurons;
    }


    ~Layer() = default;
};

