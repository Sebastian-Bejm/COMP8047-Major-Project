#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <Eigen/Dense>

class ELM
{
public:

	ELM(int inputSize, int hiddenSize, int outputSize, std::string type="clf", bool verbose = false);

	Eigen::MatrixXf Train(Eigen::MatrixXf X, Eigen::MatrixXf Y);
	Eigen::MatrixXf Predict(Eigen::MatrixXf X);
	void Score(Eigen::MatrixXf YTest, Eigen::MatrixXf YPred);

private:

	std::string type;

	int inputSize, hiddenSize, outputSize;
	Eigen::MatrixXf weights, bias;
	Eigen::MatrixXf H, beta;

	Eigen::MatrixXf Sigmoid(Eigen::MatrixXf X);
	Eigen::MatrixXf ReLu(Eigen::MatrixXf X);

};