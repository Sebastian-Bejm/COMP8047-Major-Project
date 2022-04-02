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
	// initialize random weight with range (-0.5, 0.5)
	weights = Eigen::MatrixXf::Zero(this->hiddenSize, this->inputSize).unaryExpr(weights_uniform);
	// initialize random bias with range (0, 1)
	bias = Eigen::MatrixXf::Zero(1, this->hiddenSize).unaryExpr(bias_uniform);

}

// Extreme Learning Machine training process
Eigen::MatrixXf ELM::Train(Eigen::MatrixXf X, Eigen::MatrixXf Y) {
	// calculate the hidden layer output matrix
	H = (X * weights.transpose()) + bias;

	H = SigmoidActivation(H);

	// calculate the Moore-Penrose psuedoinverse matrix
	Eigen::MatrixXf moorePenrose = H.transpose() * H;
	moorePenrose = moorePenrose.inverse();
	moorePenrose = moorePenrose * H.transpose();

	beta = moorePenrose * Y;

	return H * beta;
}

// Predict the results of the training process using test data
Eigen::MatrixXf ELM::Predict(Eigen::MatrixXf X) {
	Eigen::MatrixXf prod = (X * weights.transpose()) + bias;
	Eigen::MatrixXf y = SigmoidActivation(prod) * beta;
}

// Sigmoid activation function
Eigen::MatrixXf ELM::SigmoidActivation(Eigen::MatrixXf X) {
	for (size_t i = 0; i < X.rows(); i++) {
		for (size_t j = 0; j < X.cols(); j++) {
			X(i, j) = 1.0 / (1.0 + exp(-1 * X(i, j)));
		}
	}

	return X;
}

void ELM::Score() {

}
