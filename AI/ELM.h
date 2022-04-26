#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <Eigen/Dense>

class ELM
{
public:

	ELM(int inputSize, int hiddenSize, int outputSize, bool verbose=false);

	Eigen::MatrixXf Train(Eigen::MatrixXf X, Eigen::MatrixXf Y, std::string type="clf");
	Eigen::MatrixXf Predict(Eigen::MatrixXf X, std::string type="clf");
	void Score(Eigen::MatrixXf YTest, Eigen::MatrixXf YPred, bool regression=false);

private:

	int inputSize, hiddenSize, outputSize;
	Eigen::MatrixXf weights, bias;
	Eigen::MatrixXf H, beta;

	Eigen::MatrixXf Sigmoid(Eigen::MatrixXf X);
	Eigen::MatrixXf ReLu(Eigen::MatrixXf X);
};