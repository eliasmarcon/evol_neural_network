#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "PythonCaller.h"
#include <ga/GASelector.h>

/*========================================================================================================================================*/
/*============================================================= CONSTANTS ===============================================================*/
/*========================================================================================================================================*/

const int POPULATION_SIZE = 10;
const int MAX_GENERATIONS = 4;

const float MIN_ACCURACY = 0.85;
const float MAX_ACCURACY = 1.0;
const float MIN_LOSS = 0.1;

const float MEAN_WEIGHTS = 0.0;
const float MEAN_BIAS = 0.1;
const float STDDEV = 0.05;

const int EMPTY_PLACEHOLDER = 0;

const std::vector<int> IN_WEIGHTS = {13, 64, 32};
const std::vector<int> OUT_WEIGHTS = {64, 32, 1};

/*========================================================================================================================================*/
/*============================================================= GA FUNCTIONS =============================================================*/
/*========================================================================================================================================*/

// Objective function
float objective(GAGenome &g)
{
    float fitness = 2.0;

    GA2DArrayGenome<float> &genome = (GA2DArrayGenome<float> &)g;

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

    // get the shape of the vector and print it
    std::vector<int> shape;
    for (std::vector<float>::size_type i = 0; i < weights.size(); i++)
    {
        shape.push_back(weights[i].size());
    }

    // Ensure Python interpreter is initialized
    PyObject *pWeights = PythonCaller::Vector2DToPyList(weights);
    PyObject *pInput = PythonCaller::VectorToPyList(input_python);

    PyObject *pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pWeights);
    PyTuple_SetItem(pArgs, 1, pInput);

    float loss, acc = 0.0;

    PythonCaller::CallPythonFunction("neural_network", "main", pArgs, loss, acc);

    float lossPenalty = 0.0;
    float accPenalty = 0.0;

    if (loss > MIN_LOSS)
    {
        lossPenalty = loss - MIN_LOSS;
    }

    if (acc < MAX_ACCURACY)
    {
        accPenalty = MAX_ACCURACY - acc;
    }

    float fitnessPenalty = lossPenalty + accPenalty;

    // std::cout << "Fitness penalty: " << fitnessPenalty << std::endl;

    // Adjust fitness proportionally
    fitness -= fitnessPenalty;

    return fitness;
}

// Initializer
void initializer(GAGenome &g)
{
    GA2DArrayGenome<float> &genome = (GA2DArrayGenome<float> &)g;

    // Seed for the random number generator
    std::default_random_engine rng(std::random_device{}());

    std::normal_distribution<float> distribution_weights(MEAN_WEIGHTS, STDDEV);
    std::normal_distribution<float> distribution_bias(MEAN_BIAS, STDDEV);

    int in_offset = 0;

    for (int i = 0; i < static_cast<int>(IN_WEIGHTS.size()); ++i)
    {
        int in_val_weights = IN_WEIGHTS[i];
        int out_val_weights = OUT_WEIGHTS[i];
        // weights
        // std::cout << "Weights" << std::endl;
        for (int i = 0; i < in_val_weights; i++)
        {
            for (int j = 0; j < out_val_weights; j++)
            {
                // random float between -1 and 1
                genome.gene(in_offset + i, j, distribution_weights(rng));
            }
        }
        in_offset += in_val_weights;

        // biases
        for (int i = 0; i < out_val_weights; i++)
        {
            genome.gene(in_offset, i, distribution_bias(rng));
        }

        ++in_offset;
    }
}

// Mutator
int mutator(GAGenome &g, float p)
{
    GA2DArrayGenome<float> &genome = (GA2DArrayGenome<float> &)g;
    int nMutations = 0;

    std::default_random_engine rng(std::random_device{}());
    std::normal_distribution<float> dist(0, 0.1); // Small mutation step

    for (int i = 0; i < genome.width(); i++)
    {
        for (int j = 0; j < genome.height(); j++)
        {
            if (GAFlipCoin(p) && genome.gene(i, j) != EMPTY_PLACEHOLDER)
            {
                // Add a small random value to the weight
                float mutationValue = dist(rng);
                genome.gene(i, j, genome.gene(i, j) + mutationValue);
                ++nMutations;
            }
        }
    }

    return nMutations;
}

// Crossover
int crossover(const GAGenome &p1, const GAGenome &p2, GAGenome *c1, GAGenome *c2)
{
    GA2DArrayGenome<float> &parent1 = (GA2DArrayGenome<float> &)p1;
    GA2DArrayGenome<float> &parent2 = (GA2DArrayGenome<float> &)p2;

    // Generate two random crossover points
    int point1 = GARandomInt(0, parent1.width() - 1);
    int point2 = GARandomInt(0, parent1.height() - 1);

    // Ensure point2 >= point1
    if (point1 > point2)
    {
        std::swap(point1, point2);
    }

    // Generate a random weight for averaging
    std::default_random_engine rng(std::random_device{}());
    float weight = std::uniform_real_distribution<float>(0, 1)(rng);

    // Perform hybrid crossover
    if (c1 && c2)
    {
        GA2DArrayGenome<float> &child1 = (GA2DArrayGenome<float> &)*c1;
        GA2DArrayGenome<float> &child2 = (GA2DArrayGenome<float> &)*c2;

        for (int i = 0; i < parent1.width(); i++)
        {
            for (int j = 0; j < parent1.height(); j++)
            {
                if (i >= point1 && i <= point2 && j >= point1 && j <= point2)
                {
                    // Weighted average for the genes within the crossover points
                    child1.gene(i, j, parent1.gene(i, j) * weight + parent2.gene(i, j) * (1 - weight));
                    child2.gene(i, j, parent1.gene(i, j) * (1 - weight) + parent2.gene(i, j) * weight);
                }
                else
                {
                    // Direct inheritance outside the crossover points
                    child1.gene(i, j, parent1.gene(i, j));
                    child2.gene(i, j, parent2.gene(i, j));
                }
            }
        }
        return 2;
    }
    else if (c1)
    {
        // If only one child, do a simple mix
        GA2DArrayGenome<float> &child = (GA2DArrayGenome<float> &)*c1;
        for (int i = 0; i < parent1.width(); i++)
        {
            for (int j = 0; j < parent1.height(); j++)
            {
                child.gene(i, j, (parent1.gene(i, j) + parent2.gene(i, j)) / 2.0);
            }
        }
        return 1;
    }
    return 0;
}

class BestSelector : public GASelectionScheme
{
    public:
        GADefineIdentity("BestSelector", 0);

        BestSelector() : GASelectionScheme() {}
        virtual ~BestSelector() {}

        virtual GASelectionScheme *clone() const
        {
            return new BestSelector;
        }

        virtual void assign(GAPopulation &p)
        {
            GASelectionScheme::assign(p);
        }

        virtual void update()
        {
            GASelectionScheme::update();
        }

        virtual GAGenome &select() const
        {
            const int numCandidates = POPULATION_SIZE * 0.5; // Change the number of candidates
            const int numSelected = POPULATION_SIZE * 0.2;   // Change the number of selected individuals

            int idx[numCandidates];
            GAGenome *candidates[numCandidates];

            for (int i = 0; i < numCandidates; ++i)
            {
                idx[i] = GARandomInt(0, pop->size() - 1);
                candidates[i] = &(pop->individual(idx[i]));
            }

            // Sort candidates based on their scores in descending order
            std::sort(candidates, candidates + numCandidates,
                    [](const GAGenome *a, const GAGenome *b)
                    {
                        return a->score() > b->score();
                    });

            // Return a reference to the 10th best individual
            return *candidates[numSelected - 1];
        }
};

/*========================================================================================================================================*/
/*============================================================= HELPER FUNCTIONS =========================================================*/
/*========================================================================================================================================*/

void displayWeightsAndBiases(const std::vector<std::vector<float>>& weights) {
    int layerCounter = 0;
    std::cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (auto size : IN_WEIGHTS) {
        layerCounter++;

        std::cout << "Layer " << layerCounter << " weights:" << std::endl;

        for (int i = 0; i < size; i++) {
            std::cout << "[";
            for (int j = 0; j < OUT_WEIGHTS[layerCounter - 1]; j++) {
                if (j == OUT_WEIGHTS[layerCounter - 1] - 1) {
                    std::cout << weights[i][j];
                } else {
                    std::cout << weights[i][j] << " ";
                }
            }
            std::cout << "]" << std::endl;
        }

        std::cout << "\nLayer " << layerCounter << " biases:" << std::endl;

        std::cout << "[";
        for (int i = 0; i < OUT_WEIGHTS[layerCounter - 1]; i++) {
            if (i == OUT_WEIGHTS[layerCounter - 1] - 1) {
                std::cout << weights[size][i];
            } else {
                std::cout << weights[size][i] << " ";
            }
        }
        std::cout << "]" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

}

void printElapsedTime(const std::chrono::time_point<std::chrono::system_clock>& start_time,
                      const std::chrono::time_point<std::chrono::system_clock>& end_time) {
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Calculate hours, minutes, seconds, and milliseconds
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    // Print elapsed time in hours, minutes, seconds, and milliseconds
    std::cout << "Time taken for evolution: " << hours.count() << " hours, "
              << minutes.count() << " minutes, "
              << seconds.count() << " seconds, and "
              << milliseconds.count() << " milliseconds\n"
              << std::endl;
}

void printNeuralNetworkDetails(){

    std::cout << "Neural Network Details:" << std::endl;
    std::cout << "   - Population size: " << POPULATION_SIZE << std::endl;
    std::cout << "   - Max generations: " << MAX_GENERATIONS << std::endl;

    for (int i = 0; i < 3; ++i) {
        std::cout << "   - Layer " << i + 1 << " input size:  " << IN_WEIGHTS[i] << std::endl;
        std::cout << "   - Layer " << i + 1 << " output size: " << OUT_WEIGHTS[i] << std::endl;
    }

    std::cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

}

/*========================================================================================================================================*/
/*============================================================= MAIN =====================================================================*/
/*========================================================================================================================================*/

int main()
{
    PythonCaller pythonCaller;
    srand(static_cast<unsigned int>(time(nullptr)));

    // Calculate the sum of elements in IN_WEIGHTS
    int sumIN = 0;
    for (int weight : IN_WEIGHTS)
    {
        sumIN += weight;
    }

    // Calculate the sum of elements in OUT_WEIGHTS
    int max = IN_WEIGHTS[0];
    for (int weight : OUT_WEIGHTS)
    {
        if (weight > max)
            max = weight;
    }

    // Print the neural network details
    printNeuralNetworkDetails();

    // Start measuring time
    auto start_time = std::chrono::high_resolution_clock::now();

    GA2DArrayGenome<float> genome(sumIN + static_cast<int>(IN_WEIGHTS.size()), max, objective);
    genome.initializer(initializer);
    genome.mutator(mutator);
    genome.crossover(crossover);

    GASimpleGA ga(genome);
    ga.populationSize(POPULATION_SIZE);
    ga.pMutation(0.1);
    ga.pCrossover(0.9);
    ga.minimaxi(GAGeneticAlgorithm::MAXIMIZE);
    ga.set(gaNnGenerations, MAX_GENERATIONS);

    BestSelector selector;
    ga.selector(selector);

    // Evolve and output information for each generation
    std::cout << "\nStarting evolution...\n\n";
    ga.evolve();
    GA2DArrayGenome<float> bestGenome = (GA2DArrayGenome<float> &)ga.statistics().bestIndividual();

    // shape of the layers
    std::vector<int> input_python = OUT_WEIGHTS;

    input_python.insert(input_python.begin(), IN_WEIGHTS[0]);

    // get the values of the genome and put them in a 2 dimensional vector
    std::vector<std::vector<float>> weights;

    // put the values of each genome row into a vector without 0
    for (int i = 0; i < bestGenome.width(); i++)
    {
        std::vector<float> row;
        for (int j = 0; j < bestGenome.height(); j++)
        {
            if (bestGenome.gene(i, j) != EMPTY_PLACEHOLDER)
            {
                row.push_back(bestGenome.gene(i, j));
            }
        }
        weights.push_back(row);
    }

    // Stop measuring time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Print the best genome
    std::cout << "Best Genome:" << std::endl;
    displayWeightsAndBiases(weights);

    // Print elapsed time
    printElapsedTime(start_time, end_time);

    // Ensure Python interpreter is initialized
    PyObject *pWeights = PythonCaller::Vector2DToPyList(weights);
    PyObject *pInput = PythonCaller::VectorToPyList(input_python);

    PyObject *pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pWeights);
    PyTuple_SetItem(pArgs, 1, pInput);

    float loss, acc = 0.0;
    PythonCaller::CallPythonFunction("neural_network", "main", pArgs, loss, acc);

    std::cout << "Performance of the best weigths and biases:" << std::endl;
    std::cout << "Best Genome loss: " << loss << std::endl;
    std::cout << "Best Genome accuracy: " << std::fixed << std::setprecision(2) << acc * 100 << "%" << std::endl
              << std::endl;
  
    return 0;
}