#include "Perceptron.hpp"


// Конструктор для инициализации слоев перцептрона
Perceptron::Perceptron(int input_size, int hidden_size, int output_size) {

	input_layer = Layer(input_size, hidden_size);
	hidden_layer = Layer(hidden_size, output_size);
	output_layer = Layer(output_size, 1);
}

// Функция активации перцептрона
std::vector<double> Perceptron::activate(std::vector<double>& inputs) {

	std::vector<double> hidden_outputs = input_layer.activate(inputs);
	std::vector<double> final_outputs = hidden_layer.activate(hidden_outputs);

	final_outputs = output_layer.activate(final_outputs);

	return final_outputs;
}

// Обучение перцептрона
void Perceptron::train(std::vector<double>& inputs, std::vector<double>& target_outputs, double learning_rate) {

	// Прямой проход
	std::vector<double> hidden_outputs = input_layer.activate(inputs);
	std::vector<double> final_outputs = hidden_layer.activate(hidden_outputs);
	final_outputs = output_layer.activate(final_outputs);

	// Вычисление ошибки на выходе
	double error = target_outputs[0] - final_outputs[0];

	// Обратный проход для корректировки весов выходного слоя
	std::vector<double> output_errors = { error };
	output_layer.train(hidden_outputs, output_errors, learning_rate);

	// Вычисление ошибки на скрытом слое
	std::vector<double> hidden_errors(hidden_layer.GetNeurons().size());

	for (int i = 0; i < hidden_layer.GetNeurons().size(); i++) {
		double error = output_errors[0] * output_layer.GetNeurons()[0].GetWeights()[i];
		hidden_errors[i] = error;
	}

	// Обратный проход для корректировки весов скрытого слоя
	hidden_layer.train(hidden_outputs, output_errors, learning_rate);
	input_layer.train(inputs, hidden_errors, learning_rate);
}
