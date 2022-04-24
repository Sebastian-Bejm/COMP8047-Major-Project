#pragma once

#include <Eigen/Dense>
#include <iostream>

#include "ELM.h"

// This is to test that the ELM is functioning correctly.
void TestGenData(bool regression=false) {
	// Generate random data
	Eigen::MatrixXf data_x, data_y, test_x, test_y;
	int samples_to_gen = 1000;

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());

	// classification
	std::uniform_real_distribution<float> rand_distr(-5, 5);
	std::uniform_int_distribution<int> q_distr(0, 1);

	// regression
	std::uniform_real_distribution<float> randreg_distr(-5, 5);
	std::uniform_real_distribution<float> qr_distr(-200, 200);

	// initial sizes
	int train_size = (int) (samples_to_gen * 0.8);
	int test_size = (int) (samples_to_gen * 0.2);
	data_x = Eigen::MatrixXf::Zero(train_size, 20);
	data_y = Eigen::MatrixXf::Zero(train_size, 1);

	test_x = Eigen::MatrixXf::Zero(test_size, 20);
	test_y = Eigen::MatrixXf::Zero(test_size, 1);

	if (regression) {
		// regression data
		// gen training data
		for (int i = 0; i < data_x.rows(); i++) {
			for (int j = 0; j < data_x.cols(); j++) {
				data_x(i, j) = randreg_distr(gen);
			}
			data_y(i, 0) = qr_distr(gen);
		}

		// gen testing data
		for (int i = 0; i < test_x.rows(); i++) {
			for (int j = 0; j < test_x.cols(); j++) {
				test_x(i, j) = randreg_distr(gen);
			}
			test_y(i, 0) = qr_distr(gen);
		}
	}
	else {
		// classification data
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
	}

	// Testing ELM
	ELM elm = ELM(data_x.cols(), 20, 1);

	elm.Train(data_x, data_y);

	Eigen::MatrixXf y_pred = elm.Predict(test_x);
	
	elm.Score(test_y, y_pred, regression);
}