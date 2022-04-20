#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <Eigen/Dense>

// Misc stuff:
// https://medium.datadriveninvestor.com/extreme-learning-machine-for-simple-classification-e776ad797a3c
// https://github.com/5663015/elm

class ELM
{
public:

	ELM(int inputSize, int hiddenSize, int outputSize);

	Eigen::MatrixXf Train(Eigen::MatrixXf X, Eigen::MatrixXf Y);
	Eigen::MatrixXf Predict(Eigen::MatrixXf X);
	void Score(Eigen::MatrixXf YTest, Eigen::MatrixXf YPred);

private:

	int inputSize, hiddenSize, outputSize;
	Eigen::MatrixXf weights, bias;
	Eigen::MatrixXf H, beta;

	Eigen::MatrixXf SigmoidActivation(Eigen::MatrixXf X);

};