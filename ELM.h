#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <Eigen/Dense>

// Misc stuff:
// https://www.delftstack.com/howto/cpp/cpp-dot-product/
// https://medium.datadriveninvestor.com/extreme-learning-machine-for-simple-classification-e776ad797a3c
// https://www.geeksforgeeks.org/ml-neural-network-implementation-in-c-from-scratch/
// https://github.com/5663015/elm

class ELM
{
public:

	ELM(int inputSize, int hiddenSize, int outputSize);

	void Train(Eigen::MatrixXf features, Eigen::MatrixXf labels);
	void Predict(Eigen::MatrixXf X);
	void Score();

private:

	int inputSize, hiddenSize, outputSize;
	Eigen::MatrixXf weights, bias;
	Eigen::MatrixXf H, beta;

	Eigen::MatrixXf SigmoidActivation(Eigen::MatrixXf X);
	Eigen::MatrixXf RELUActivation(Eigen::MatrixXf X);

};