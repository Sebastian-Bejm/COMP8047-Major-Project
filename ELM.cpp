#include "ELM.h"

ELM::ELM(int inputSize, int hiddenSize, int outputSize) {
	this->inputSize = inputSize;
	this->hiddenSize = hiddenSize;
	this->outputSize = outputSize;

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_real_distribution<float> weights_distr(-0.5, 0.5);
	auto weights_uniform = [&](float) {return weights_distr(gen); };

	std::uniform_real_distribution<float> bias_distr(0, 1);
	auto bias_uniform = [&](float) {return bias_distr(gen); };

	// may have to change this, doesnt seem uniform
	weights = Eigen::MatrixXf::Zero(this->hiddenSize, this->inputSize).unaryExpr(weights_uniform);
	//std::cout << weights << std::endl;

	bias = Eigen::MatrixXf::Zero(1, this->hiddenSize).unaryExpr(bias_uniform);

	SigmoidActivation(bias);

	//std::cout << bias << std::endl;
}

void ELM::Train(Eigen::MatrixXf features, Eigen::MatrixXf labels) {
	
}

void ELM::Predict(Eigen::MatrixXf X) {

}

Eigen::MatrixXf ELM::SigmoidActivation(Eigen::MatrixXf X) {
	for (size_t i = 0; i < X.rows(); i++) {
		for (size_t j = 0; j < X.cols(); j++) {
			X(i, j) = 1.0 / (1.0 + exp(-1 * X(i, j)));
		}
	}

	return X;
}

Eigen::MatrixXf ELM::RELUActivation(Eigen::MatrixXf X) {

}

void ELM::Score() {

}
