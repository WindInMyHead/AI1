#include "Neuron.hpp"


Neuron::Neuron(int input_size) {
    bias = ((double)rand() / RAND_MAX) * 2 - 1;

    for (int i = 0; i < input_size; i++)
        weights.push_back(((double)rand() / RAND_MAX) * 2 - 1);
}

// Функция активации (sigmoid)
double Neuron::activate(std::vector<double>& inputs) {
    double sum = bias;

    for (int i = 0; i < inputs.size(); i++)
        sum += inputs[i] * weights[i];

    return 1.0 / (1.0 + exp(-sum));
}

// Обновление весов нейрона
void Neuron::train(std::vector<double>& inputs, double error, double learning_rate) {

    double output = activate(inputs);
    double delta = error * output * (1 - output);
    bias += delta * learning_rate;

    for (int i = 0; i < inputs.size(); i++)
        weights[i] += delta * inputs[i] * learning_rate;
}