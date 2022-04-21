#include "ELM.h"

// Initialize ELM with input and hidden size
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

	// initialize random weight with range (-0.5, 0.5)
	weights = Eigen::MatrixXf::Zero(this->hiddenSize, this->inputSize).unaryExpr(weights_uniform);

	// initialize random bias with range (0, 1)
	bias = Eigen::MatrixXf::Zero(1, this->hiddenSize).unaryExpr(bias_uniform);
}

// Extreme Learning Machine training process
Eigen::MatrixXf ELM::Train(Eigen::MatrixXf X, Eigen::MatrixXf Y) {

	// calculate the hidden layer output matrix
	H = (X * weights.transpose()) + bias.replicate(X.rows(), 1);
	H = ReLuActivation(H);

	// calculate the Moore-Penrose psuedoinverse matrix using SVD method
	Eigen::JacobiSVD<Eigen::MatrixXf> svd(H, Eigen::ComputeThinU | Eigen::ComputeThinV);

	constexpr double epsilon = std::numeric_limits<double>::epsilon();
	double tolerance = epsilon * std::max(H.cols(), H.rows()) * svd.singularValues().array().abs()(0);

	Eigen::MatrixXf moorePenrose = svd.matrixV() * 
		(svd.singularValues().array().abs() > tolerance)
		.select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal()
		* svd.matrixU().adjoint();

	//Eigen::MatrixXf moorePenrose = H.completeOrthogonalDecomposition().pseudoInverse();

	// calculate the output weights
	beta = moorePenrose * Y;

	return H * beta;
}

// Predict the results of the training process using test data
Eigen::MatrixXf ELM::Predict(Eigen::MatrixXf X) {
	Eigen::MatrixXf pred = ReLuActivation((X * weights.transpose()) + bias.replicate(X.rows(), 1)) * beta;

	return pred;
}

// Sigmoid activation function
Eigen::MatrixXf ELM::SigmoidActivation(Eigen::MatrixXf X) {
	return (1 + (-X.array()).exp()).cwiseInverse();
}

// Rectified linear activation function
Eigen::MatrixXf ELM::ReLuActivation(Eigen::MatrixXf X) {
	Eigen::MatrixXf relu = Eigen::MatrixXf(X.rows(), X.cols());
	for (size_t i = 0; i < X.rows(); i++) {
		for (size_t j = 0; j < X.cols(); j++) {
			if (X(i, j) <= 0.0) {
				relu(i, j) = 0.0;
			}
			else {
				relu(i, j) = X(i, j);
			}
		}
	}

	return relu;
}

// Calculate the accuracy and loss based on predictions
// This is for initial test classification set
void ELM::Score(Eigen::MatrixXf YTest, Eigen::MatrixXf YPred) {
	int correct = 0;
	int total = YTest.rows();
	int onesCount = 0, zerosCount = 0;

	for (int i = 0; i < total; i++) {
		YPred(i, 0) = YPred(i, 0) >= 0.5 ? 1 : 0;
	}

	for (int i = 0; i < total; i++) {
		if (YTest(i, 0) == YPred(i, 0)) {
			correct++;
		}
	}

	float accuracy = correct / total;
	std::cout << "Accuracy for " << hiddenSize << " hidden nodes " << accuracy << std::endl;
}
