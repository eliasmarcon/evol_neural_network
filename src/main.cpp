#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <random>
#include "PythonCaller.h"

const int POPULATION_SIZE = 2;
const int MAX_GENERATIONS = 3;
const float MIN_ACCURACY = 0.85;
const float MIN_LOSS = 0.15;
const float MEAN_WEIGHTS = 0.0;
const float MEAN_BIAS = 0.1;
const float STDDEV = 0.05;

const int EMPTY_PLACEHOLDER = 0;

const std::vector<int> IN_WEIGHTS = {13, 64, 32};
const std::vector<int> OUT_WEIGHTS = {64, 32, 1};


/*
// Testing values
const std::vector<int> IN_WEIGHTS = {2, 4, 3};
const std::vector<int> OUT_WEIGHTS = {4, 3, 1};
*/

// Objective function
float objective(GAGenome &g)
{
    int fitness = 2;

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


    // // Display the 2D vector
    // std::cout << std::endl;
    // std::cout << "Weights Vector" << std::endl;
    // for (const auto& row : weights) {
    //     for (float value : row) {
    //         std::cout << value << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // Ensure Python interpreter is initialized
    PyObject* pWeights = PythonCaller::Vector2DToPyList(weights);
    PyObject* pInput = PythonCaller::VectorToPyList(input_python);

    PyObject* pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pWeights);
    PyTuple_SetItem(pArgs, 1, pInput);

    float testLoss, testAcc;
    PythonCaller::CallPythonFunction("neural_network", "main", pArgs, testLoss, testAcc);

    std::cout << "C++ Output" << std::endl;
    std::cout << "Test loss: " << testLoss << std::endl;
    std::cout << "Test accuracy: " << testAcc << std::endl << std::endl;

    if (testLoss < MIN_LOSS){
        --fitness;
    }
    
    if (testAcc < MIN_ACCURACY){
        --fitness;
    }

    return (float)fitness;

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
    
    for (int i = 0; i < static_cast<int>(IN_WEIGHTS.size()); ++i)
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

    // //print genome
    // for (int i = 0; i < genome.width(); i++)
    // {
    //     for (int j = 0; j < genome.height(); j++)
    //     {
    //         std::cout << genome.gene(i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
}

// Mutator
int mutator(GAGenome &g, float p)
{
    GA2DArrayGenome<float>& genome = (GA2DArrayGenome<float>&)g;
    int nMutations = 0;

    if (GAFlipCoin(p))
    {
        // change the sign of a random number in the genome
        
        
        ++nMutations;
    }

    return nMutations;
}

// Crossover
int crossover(const GAGenome &p1, const GAGenome &p2, GAGenome *c1, GAGenome *c2)
{
    GA2DArrayGenome<float>& parent1 = (GA2DArrayGenome<float>&)p1;
    GA2DArrayGenome<float>& parent2 = (GA2DArrayGenome<float>&)p2;

    // Add values of parents togehter and take the mean and same them in a 2d vector
    std::vector<std::vector<float>> saveVectorParent1(parent1.width(), std::vector<float>(parent1.height(), 0));
    std::vector<std::vector<float>> saveVectorParent2(parent1.width(), std::vector<float>(parent1.height(), 0));
    std::vector<std::vector<float>> saveVectorMean(parent1.width(), std::vector<float>(parent1.height(), 0));

    std::default_random_engine rng(std::random_device{}());
    // one value between 0 and 1
    float weight = std::round(std::uniform_real_distribution<float>(0, 1)(rng) * 100) / 100;

    //print value
    std::cout << "distribution: " << weight << std::endl;

    for (int i = 0; i < parent1.width(); i++){
        for (int j = 0; j < parent1.height(); j++){
            saveVectorParent1[i][j] = ((parent1.gene(i, j) * (1-weight)) + (parent2.gene(i, j) * weight));        
            saveVectorParent2[i][j] = ((parent1.gene(i, j) * weight) + (parent2.gene(i, j) * (1-weight)));
            saveVectorMean[i][j] = (parent1.gene(i, j) + parent2.gene(i, j))*0.5;   
        }
    }
    // create two children
    if (c1 && c2){

        GA2DArrayGenome<float>& child1 = (GA2DArrayGenome<float>&)*c1;
        GA2DArrayGenome<float>& child2 = (GA2DArrayGenome<float>&)*c2;

        for (int i = 0; i < parent1.width(); i++){
            for (int j = 0; j < parent1.height(); j++){
                child1.gene(i, j, saveVectorParent1[i][j]);
                child2.gene(i, j, saveVectorParent2[i][j]);
            }
        }

        // // print saveVector
        // std::cout << "child1" << std::endl;
        // for (int i = 0; i < child1.width(); i++){
        //     for (int j = 0; j < child1.height(); j++){
        //         std::cout << child1.gene(i, j) << " ";
        //     }
        //     std::cout << std::endl;
        // }
        
        return 2;
    }
    else if(c1){
        
        GA2DArrayGenome<float>& child = (GA2DArrayGenome<float>&)*c1;

        for (int i = 0; i < parent1.width(); i++){
            for (int j = 0; j < parent1.height(); j++){
                child.gene(i, j, saveVectorMean[i][j]);
            }
        }

        return 1;
    }else{
        return 0;
    }
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

    GA2DArrayGenome<float> genome(sumIN + static_cast<int>(IN_WEIGHTS.size()), max, objective);
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
    std::cout << std::endl;

    return 0;
}


//     b0, b1, b2, b3, b4
//     w20,21,22,23,24,25  
//     w10,11,12,13,14,15 
// L1  w00,01,02,03,04,05  L2 w06,07,08,09, L3 w010,011,012,013