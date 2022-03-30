#pragma once

#include <vector>

// Misc stuff:
// https://www.delftstack.com/howto/cpp/cpp-dot-product/
// https://medium.datadriveninvestor.com/extreme-learning-machine-for-simple-classification-e776ad797a3c
// https://www.geeksforgeeks.org/ml-neural-network-implementation-in-c-from-scratch/
// https://github.com/5663015/elm

class ELM
{
public:

	ELM();
	ELM(int inputSize, int hiddenSize, int outputSize);

	void SigmoidTrain();
	void RELUTrain();

	void Predict();

private:
	int inputSize, hiddenSize, outputSize;
};