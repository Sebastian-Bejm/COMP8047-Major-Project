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

	constexpr double epsilon = std::numeric_limits<double>::epsilon();

	// calculate the Moore-Penrose psuedoinverse matrix (NEW)
	Eigen::JacobiSVD<Eigen::MatrixXf> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);
	// for a non square matrix
	//Eigen::JacobiSVD<Eigen::MatrixXf> svd(H, Eigen::ComputeThinU | Eigen::ComputeThinV);
	double tolerance = epsilon * std::max(H.cols(), H.rows()) * svd.singularValues().array().abs()(0);
	Eigen::MatrixXf moorePenrose = svd.matrixV() * 
		(svd.singularValues().array().abs() > tolerance)
		.select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal()
		* svd.matrixU().adjoint();

	// calculate the Moore-Penrose psuedoinverse matrix (OLD)
	//Eigen::MatrixXf moorePenrose = H.transpose() * H;
	//moorePenrose = moorePenrose.inverse();
	//moorePenrose = moorePenrose * H.transpose();

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

// Calculate the accuracy and loss based on predictions
void ELM::Score(Eigen::MatrixXf X, Eigen::MatrixXf Y) {
	/*def score(self, x, y) :
        self.prediction = self.predict(x)
        if self.elm_type == 'clf':
            self.correct = 0
            for i in range(y.shape[0]):
                if self.prediction[i] == y[i]:
                    self.correct += 1
            self.test_score = self.correct/y.shape[0]
        if self.elm_type == 'reg':
            self.test_score = np.sqrt(np.sum((self.result - self.y) * (self.result - self.y))/self.y.shape[0])

        return self.test_score*/
}
