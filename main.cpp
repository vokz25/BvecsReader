#define N_COLS 128
#define N_ROWS 512

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <random>
#include <chrono>

#include "BvecsReader.h"

using namespace std;

vector<vector<float>> read_first_bvecs(int nvectors, const string &filename, int expected_dimension = N_COLS) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка при открытии файла: " << filename << endl;
        return {};
    }

    vector<vector<float>> matrix(N_ROWS, vector<float>(N_COLS, 0));

    for(int i=0; i<N_ROWS; ++i){
        file.read(reinterpret_cast<char*>(matrix[i].data()), N_COLS * sizeof(float));
    }

    file.close();
    return matrix;
}

void printMatrix(const Eigen::MatrixXf& matrix) {
    for (int row = 0; row < matrix.rows(); ++row) {
        for (int col = 0; col < matrix.cols(); ++col) {
            // Выводим элемент с двумя знаками после запятой
            std::cout << std::fixed << std::setprecision(5) << matrix(row, col) << "\t";
        }
        std::cout << std::endl; // Перевод строки после каждой строки матрицы
        std::cout << "===============================================" << std::endl;
    }
}

int main() {
    string path = "/home/vladimir/Desktop/ANN/learn.bvecs";

    std::ifstream file = ifstream(path, std::ios::binary);

    unsigned dim;

    file.read(reinterpret_cast<char*>(&dim), sizeof(dim));

    cout<<dim<<endl;

 //   return 0;

    BvecsReader reader(path);

    cout<<"Class created Successfully"<<endl;

    cout<<"Reading 1000 vectors from 10.000.000 to 10.010.000"<<endl;
    auto start = std::chrono::high_resolution_clock::now();
    Eigen::MatrixXf matrix = reader.Read(10000000, 10010000);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout<<"Time consumed: "<<elapsed.count()<<" seconds"<<endl;

    printMatrix(matrix);

//    cout<<"Reading 1000 random vectors"<<endl;
//    start = std::chrono::high_resolution_clock::now();
//    reader.ReadSample(1000);
//    end = std::chrono::high_resolution_clock::now();
//    elapsed = end - start;
//    cout<<"Time consumed: "<<elapsed.count()<<" seconds"<<endl;

    return 0;
}
