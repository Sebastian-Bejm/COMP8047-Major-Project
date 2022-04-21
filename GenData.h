#pragma once

#include <Eigen/Dense>
#include <iostream>

#include "ELM.h"

void TestGenData() {
	// Generate random data
	Eigen::MatrixXf data_x, data_y, test_x, test_y;
	int samples_to_gen = 1000;

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());

	std::uniform_real_distribution<float> rand_distr(0, 4);
	std::uniform_int_distribution<int> q_distr(0, 1);
	std::uniform_int_distribution<int> t_distr(0, 1);

	// initial sizes
	data_x = Eigen::MatrixXf::Zero(800, 20);
	data_y = Eigen::MatrixXf::Zero(800, 1);

	test_x = Eigen::MatrixXf::Zero(200, 20);
	test_y = Eigen::MatrixXf::Zero(200, 1);

	// gen training data
	for (int i = 0; i < data_x.rows(); i++) {
		for (int j = 0; j < data_x.cols(); j++) {
			data_x(i, j) = rand_distr(gen);
		}
		data_y(i, 0) = q_distr(gen);
	}

	// gen testing data
	for (int i = 0; i < test_x.rows(); i++) {
		for (int j = 0; j < test_x.cols(); j++) {
			test_x(i, j) = rand_distr(gen);
		}
		test_y(i, 0) = q_distr(gen);
	}

	// Testing ELM
	ELM elm = ELM(data_x.cols(), 20, 1);

	elm.Train(data_x, data_y);

	Eigen::MatrixXf y_pred = elm.Predict(test_x);
	
	std::cout << y_pred << std::endl;

	elm.Score(test_y, y_pred);
}