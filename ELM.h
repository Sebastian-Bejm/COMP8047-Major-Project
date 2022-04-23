#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <Eigen/Dense>

class ELM
{
public:

	ELM(int inputSize, int hiddenSize, int outputSize);

	Eigen::MatrixXf Train(Eigen::MatrixXf X, Eigen::MatrixXf Y);
	Eigen::MatrixXf Predict(Eigen::MatrixXf X);
	void Score(Eigen::MatrixXf YTest, Eigen::MatrixXf YPred, bool regression=false);

private:

	int inputSize, hiddenSize, outputSize;
	Eigen::MatrixXf weights, bias;
	Eigen::MatrixXf H, beta;

	Eigen::MatrixXf SigmoidActivation(Eigen::MatrixXf X);
	Eigen::MatrixXf ReLuActivation(Eigen::MatrixXf X);

};