#include "Layer.hpp"


// Конструктор для инициализации нейронов слоя
Layer::Layer(int input_size, int num_neurons) {

    for (int i = 0; i < num_neurons; i++) {
        Neuron neuron(input_size);
        neurons.push_back(neuron);
    }
}

// Функция активации слоя (ReLU)
std::vector<double> Layer::activate(std::vector<double>& inputs) {

    std::vector<double> outputs;

    for (int i = 0; i < neurons.size(); i++) {
        double output = neurons[i].activate(inputs);
        outputs.push_back(std::max(0.0, output));
    }
    return outputs;
}

// Обновление весов слоя
void Layer::train(std::vector<double>& inputs, std::vector<double>& errors, double learning_rate) {

    for (int i = 0; i < neurons.size(); i++) {

        double error = errors[i];

        std::vector<double> neuron_inputs(inputs);

        neuron_inputs.push_back(neurons[i].GetBias());
        neurons[i].train(neuron_inputs, error, learning_rate);
    }
}