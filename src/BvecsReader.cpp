#include "BvecsReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>

using namespace std;

BvecsReader::BvecsReader(const std::string &_path) : path(_path){
    int nstrings;

    this->d = 0;

    this->file = ifstream(path, std::ios::binary);

    if (!file) {
        std::cerr << "Error occured while opening file: " << path << std::endl;
        return; // Print error message and exit
    }
    if (!this->file.is_open()) // если файл не открыт
        cout << "Cannot open file " << _path << " !\n"; // сообщить об этом
    this->file.read((char*)&this->d, 4);
    printf("NCols=");
    printf("%i", this->d);
    printf("=NCols\n");
    this->file.seekg(0, ios::end);
    streampos pos = this->file.tellg();
    int ndatacols = this->d;
    this->width = 4 + 1 * ndatacols;
    this->nlines = pos / width;

    printf("NStr=");
    printf("%i", this->nlines);
    printf("=NStr\n");
    this->file.seekg(0, ios::beg);

    this->position = 0;
    this->buffer_vector = new vector<float>(this->d);
}

int BvecsReader::get_dim() {
    return this->d;
}

int BvecsReader::get_nlines() {
    return this->nlines;
}


std::vector<float>* BvecsReader::ReadOne(bool skip){
    this->position++;
    if(skip) {cout<<"skip\n"; this->file.seekg(this->position*(this->width));}
    else {
        file.read((char*)&this->d, 4);
        for(int i=0;i<this->d;++i){
            file.read((char*)&this->val, 1);
            this->buffer_vector->at(i) = static_cast<float>(this->val);
        }
    }

    return this->buffer_vector;
}

void BvecsReader::Reset(){
    if (!this->file.is_open()) {
        std::cerr << "Error occured: file is closed" << std::endl;
        std::exit(1);
    }
    this->position = 0;
    this->file.clear();
    this->file.seekg(0);
}

void BvecsReader::MoveToPosition(int new_position){
    //if(new_position<this->position) Reset();
    //while(this->position < new_position) {
    //    this->position++;
    //    ReadOne(true);
    //}
    this->position = new_position;
    this->file.seekg(new_position*(this->width));
}

Eigen::MatrixXf BvecsReader::Read(int from, int to){
    if(from>to || from<0 || to<0 ||to>this->nlines){
        std::cerr << "Error occured: invalid range of indexes" << std::endl;
        std::exit(1);
    }
    MoveToPosition(from);
    int n_rows = to-from;
    Eigen::MatrixXf matrix(n_rows, this->d);
    for (int i=0; i<n_rows; ++i){
        this->buffer_vector = this->ReadOne(false);
        matrix.row(i) = Eigen::Map<Eigen::VectorXf>(this->buffer_vector->data(), this->buffer_vector->size());
    }
    return matrix;
}

Eigen::MatrixXf BvecsReader::ReadSample(int n){
    std::random_device rd;  // Инициализация случайным числом
    std::mt19937 gen(rd()); // Генератор Мерсенна

    // Генератор для чисел с плавающей запятой в диапазоне [0, 1]
    std::uniform_real_distribution<float> dist(0.0, 1.0);

//    float random_number = dist(gen);

    Eigen::MatrixXf matrix(n, this->d);

    float skipped = 0, chosen = 0;
    float left_in_dataset = (float) MAX_ROWS, left_to_choose = n;
    int position = 0;

    Reset();

    while(chosen<n){

        float p = left_to_choose/left_in_dataset;
        if(dist(gen)<p){
            MoveToPosition(position);
            this->buffer_vector = ReadOne(false);
            matrix.row(chosen) = Eigen::Map<Eigen::VectorXf>(this->buffer_vector->data(), this->buffer_vector->size());
            chosen++;
            left_to_choose--;
            left_in_dataset--;
//            std::cout<<i<<" "<<left_to_choose<<" "<<left_in_dataset<<std::endl;
        } else left_in_dataset--;
        position++;
    }
    return matrix;
}

BvecsReader::~BvecsReader()
{
    this->file.close();
}
