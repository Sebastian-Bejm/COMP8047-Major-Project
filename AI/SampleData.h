#pragma once

#include <Eigen/Dense>
#include <iostream>

#include "ELM.h"

const float Iris_setosa = 0;
const float Iris_versicolor = 1;
const float Iris_virginica = 2;
const float Iris_unknown = 3;

std::vector<std::vector<float>> ReadIrisDataset()
{
	std::ifstream myfile("Data/iris.data");
	std::string line;
	std::vector<std::vector<float>> Iris_Dataset;
	std::vector<float> temp_sepal_len;
	std::vector<float> temp_sepal_wid;
	std::vector<float> temp_petal_len;
	std::vector<float> temp_petal_wid;
	std::vector<float> temp_iris_class;

	float sepal_len_f, sepal_wid_f, petal_len_f, petal_wid_f;
	float iris_class_f;

	std::string temp_string;
	int count = 0;
	if (myfile.is_open())
	{
		std::cout << "file opened successfully" << std::endl;
		while (std::getline(myfile, line)) {
			std::replace(line.begin(), line.end(), '-', '_');
			std::replace(line.begin(), line.end(), ',', ' ');

			std::istringstream iss(line);
			count++;

			iss >> sepal_len_f >> sepal_wid_f >> petal_len_f >> petal_wid_f >> temp_string;
			temp_sepal_len.push_back(sepal_len_f);
			temp_sepal_wid.push_back(sepal_wid_f);
			temp_petal_len.push_back(petal_len_f);
			temp_petal_wid.push_back(petal_wid_f);
			if (temp_string.compare("Iris_setosa") == 0)
			{
				iris_class_f = Iris_setosa;
			}
			else if (temp_string.compare("Iris_versicolor") == 0)
			{
				iris_class_f = Iris_versicolor;
			}
			else if (temp_string.compare("Iris_virginica") == 0)
			{
				iris_class_f = Iris_virginica;
			}
			else
			{
				iris_class_f = Iris_unknown;
			}
			temp_iris_class.push_back(iris_class_f);
		}
		Iris_Dataset.push_back(temp_sepal_len);
		Iris_Dataset.push_back(temp_sepal_wid);
		Iris_Dataset.push_back(temp_petal_len);
		Iris_Dataset.push_back(temp_petal_wid);
		Iris_Dataset.push_back(temp_iris_class);
	}
	else
	{
		std::cout << "Unable to open file";
	}
	return Iris_Dataset;
}


template <typename T>
std::vector<std::vector<T>> vect_Transpose(std::vector<std::vector<T>>& input_Vector)
{
	if (input_Vector.size() > 0)
	{
		std::vector< std::vector<T> > out_Vector(input_Vector[0].size(), std::vector<T>(input_Vector.size()));
		for (int i = 0; i < input_Vector.size(); i++)
		{
			for (int j = 0; j < input_Vector[i].size(); j++)
			{
				out_Vector[j][i] = input_Vector[i][j];
			}
		}
		return out_Vector;

	}
	return input_Vector;
}

std::vector<std::vector<float>> vector_Test_Split(const std::vector<std::vector<float>>& vect, float percent)
{
	std::vector< std::vector<float>> temp_out;
	std::vector<std::vector<float>> input = vect;
	int sindex = vect[0].size() * (100 - percent) / 100;

	// dealing with data as vectors is easier than separating features in vectors
	input = vect_Transpose(input);
	for (auto temp = input.begin() + sindex; temp != input.end(); temp++)
	{
		temp_out.push_back(*temp);
	}
	return vect_Transpose(temp_out);
}

std::vector<std::vector<float>> vector_Train_Split(const std::vector<std::vector<float>>& vect, float percent)
{
	std::vector<std::vector<float>> temp_out;
	std::vector<std::vector<float>> input = vect;
	int size = vect[0].size() * percent / 100;
	// dealing with data as vectors is easier than separating features in vectors
	input = vect_Transpose(input);
	for (auto temp = input.begin(); temp != input.begin() + size; temp++)
	{
		temp_out.push_back(*temp);
	}
	return vect_Transpose(temp_out);
}

void PrintData(std::vector <std::vector<float>> dataset) {
	for (size_t i = 0; i < dataset.size(); i++) {
		for (size_t j = 0; j < dataset[i].size(); j++) {
			std::cout << dataset[i][j] << ",";
		}
		std::cout << std::endl;
	}
}

std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> MakeFeaturesLabelsMatrices(std::vector<std::vector<float>> dataset) {
	Eigen::MatrixXf data_features = Eigen::MatrixXf(dataset.size(), dataset[0].size()-1);
	Eigen::MatrixXf data_labels = Eigen::MatrixXf(dataset.size(), 1);
	for (size_t i = 0; i < dataset.size(); i++) {
		for (size_t j = 0; j < dataset[i].size()-1; j++) {
			data_features(i, j) = dataset[i][j];
		}
		data_labels(i, 0) = dataset[i][dataset[0].size() - 1];
	}
	return std::make_tuple(data_features, data_labels);
}

void LoadIrisDataset() {
	std::vector<std::vector<float>> dataset = ReadIrisDataset();

	std::srand(unsigned(std::time(0)));
	dataset = vect_Transpose(dataset);
	std::random_shuffle(dataset.begin(), dataset.end());
	dataset = vect_Transpose(dataset);

	// train test split and transpose so we get the data in proper format
	std::vector<std::vector<float>> training_data = vector_Train_Split(dataset, 80);
	training_data = vect_Transpose(training_data);
	std::cout << "IRIS training Data Size is ( " << training_data.size() << " , " << training_data[0].size() << " )" << std::endl;

	std::vector<std::vector<float>> testing_data = vector_Test_Split(dataset, 20);
	testing_data = vect_Transpose(testing_data);
	std::cout << "IRIS testing Data Size is ( " << testing_data.size() << " , " << testing_data[0].size() << " )" << std::endl;
	
	std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> train = MakeFeaturesLabelsMatrices(training_data);
	Eigen::MatrixXf train_x = std::get<0>(train), train_y = std::get<1>(train);

	std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> test = MakeFeaturesLabelsMatrices(testing_data);
	Eigen::MatrixXf test_x = std::get<0>(test), test_y = std::get<1>(test);

	// multi class classification
	ELM elm = ELM(train_x.cols(), 10, 1);

	elm.Train(train_x.normalized(), train_y);

	Eigen::MatrixXf pred_y = elm.Predict(test_x.normalized());
	std::cout << pred_y << std::endl;

	//elm.Score(test_y, pred_y, 2, 3, false);
}

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