#pragma once

#include "Layer.hpp"

class Perceptron {

private:
	Layer input_layer;
	Layer hidden_layer;
	Layer output_layer;

public:
	Perceptron() = default;

	Perceptron(int input_size, int hidden_size, int output_size);


	std::vector<double> activate(std::vector<double>& inputs);
	void train(std::vector<double>& inputs, std::vector<double>& target_outputs, double learning_rate);


	~Perceptron() = default;
};
