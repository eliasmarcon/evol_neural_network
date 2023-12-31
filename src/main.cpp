#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <random>
#include "PythonCaller.h"

using namespace std;

const int POPULATION_SIZE = 1;
const int MAX_GENERATIONS = 3;
const float MEAN_WEIGHTS = 0.0;
const float MEAN_BIAS = 0.1;
const float STDDEV = 0.05;

const int EMPTY_PLACEHOLDER = 0;
/*
const std::vector<int> IN_WEIGHTS = {13, 64, 32};
const std::vector<int> OUT_WEIGHTS = §{64, 32, 1};
*/

// Testing values
const std::vector<int> IN_WEIGHTS = {2, 4, 3};
const std::vector<int> OUT_WEIGHTS = {4, 3, 1};


// Objective function
float objective(GAGenome &g)
{
    PythonCaller pythonCaller;

    GA2DArrayGenome<float>& genome = (GA2DArrayGenome<float>&)g;

    // shape of the layers
    std::vector<int> input_python = OUT_WEIGHTS;
    
    input_python.insert(input_python.begin(), IN_WEIGHTS[0]);
    
    // get the values of the genome and put them in a 2 dimensional vector
    std::vector<std::vector<float>> weights;
    
    // put the values of each genome row into a vector without 0
    for (int i = 0; i < genome.width(); i++)
    {
        std::vector<float> row;
        for (int j = 0; j < genome.height(); j++)
        {
            if (genome.gene(i, j) != EMPTY_PLACEHOLDER)
            {
                row.push_back(genome.gene(i, j));
            }
        }
        weights.push_back(row);
    }


    // Display the 2D vector
    std::cout << std::endl;
    std::cout << "Weights Vector" << std::endl;
    for (const auto& row : weights) {
        for (float value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Ensure Python interpreter is initialized
    PyObject* pWeights = PythonCaller::Vector2DToPyList(weights);
    PyObject* pInput = PythonCaller::VectorToPyList(input_python);

    PyObject* pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pWeights);
    PyTuple_SetItem(pArgs, 1, pInput);

    PythonCaller::CallPythonFunction("neural_network", "main", pArgs);

}

// Initializer
void initializer(GAGenome &g)
{
    GA2DArrayGenome<float>& genome = (GA2DArrayGenome<float>&)g;

    // Seed for the random number generator
    std::default_random_engine rng(std::random_device{}());

    std::normal_distribution<float> distribution_weights(MEAN_WEIGHTS, STDDEV);
    std::normal_distribution<float> distribution_bias(MEAN_BIAS, STDDEV);
    
    int in_offset = 0;
    
    for (int i = 0; i < IN_WEIGHTS.size(); ++i)
    {
        int in_val_weights = IN_WEIGHTS[i];
        int out_val_weights = OUT_WEIGHTS[i];
        //weights
        //std::cout << "Weights" << std::endl;
        for (int i = 0; i < in_val_weights; i++)
        {
            for (int j = 0; j < out_val_weights; j++)
            {
                // random float between -1 and 1
                //genome.gene(i, j, GARandomFloat(-1, 1));
                //std::cout << "i: " << in_offset+i << " j: " << j << std::endl;
                genome.gene(in_offset+i, j, distribution_weights(rng));
            }
        }
        in_offset += in_val_weights ;

        //biases
        //std::cout << "Bias" << std::endl;
        for (int i = 0; i < out_val_weights; i++)
        {
            //std::cout << "i: " << in_offset << " j: " << i << std::endl;
            genome.gene(in_offset, i, distribution_bias(rng));
        }

        ++in_offset;
    }

    //print genome
    for (int i = 0; i < genome.width(); i++)
    {
        for (int j = 0; j < genome.height(); j++)
        {
        
            cout << genome.gene(i, j) << " ";
        
        }
        cout << endl;
    }
}

// Mutator
int mutator(GAGenome &g, float p)
{
    
}

// Crossover
int crossover(const GAGenome &p1, const GAGenome &p2, GAGenome *c1, GAGenome *c2)
{
    
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Calculate the sum of elements in IN_WEIGHTS
    int sumIN = 0;
    for (int weight : IN_WEIGHTS) {
        sumIN += weight;
    }

    // Calculate the sum of elements in OUT_WEIGHTS
    int max = IN_WEIGHTS[0];
    for (int weight : OUT_WEIGHTS) {
        if(weight > max)
            max = weight;
    }

    GA2DArrayGenome<float> genome(sumIN + IN_WEIGHTS.size(), max, objective);
    genome.initializer(initializer);
    genome.mutator(mutator);
    genome.crossover(crossover);

    GASimpleGA ga(genome);
    ga.populationSize(POPULATION_SIZE);
    ga.nGenerations(MAX_GENERATIONS);
    ga.pMutation(0.05);
    ga.pCrossover(0.9);
    ga.evolve();

    const GA2DArrayGenome<float> &bestGenome = (GA2DArrayGenome<float> &)ga.statistics().bestIndividual();
    
    // cout << "Best solution found: ";
    // for (int i = 0; i < CHESSBOARD_SIZE; i++)
    // {
    //     cout << bestGenome.gene(i, j) + 1;
    // }
    cout << endl;

    return 0;
}


//     b0, b1, b2, b3, b4
//     w20,21,22,23,24,25  
//     w10,11,12,13,14,15 
// L1  w00,01,02,03,04,05  L2 w06,07,08,09, L3 w010,011,012,013