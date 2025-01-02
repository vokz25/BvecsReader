#define MAX_ROWS 100000000
#define SAMPLE_SIZE 10000

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Eigen/Dense"

class BvecsReader
{
    public:
        BvecsReader(const std::string &path);
        Eigen::MatrixXf Read(int from=0, int to=SAMPLE_SIZE);
        Eigen::MatrixXf ReadSample(int n=SAMPLE_SIZE);
        int get_dim();
        int get_nlines();
        int nvec();
        virtual ~BvecsReader();

    protected:

    private:
        const std::string &path; // Path to .bvec file
        std::ifstream file; // .bvec file
        int8_t val; // One value
        int d; // Dimensionality
        int position; // Where we are in the file
        int nlines; //number of lines in the file

        int width;

        std::vector<float>* buffer_vector; // Reading it here
        std::vector<int> array_of_indexes; // Used for sampling

        std::vector<float>* ReadOne(bool skip=false); // Reading ove vector
        bool CheckIfReachedEOF(); // Check, if we reached the end of file
        void Reset(); // Reseting the filestream (moove to the start)
        void MoveToPosition(int new_position);
};
