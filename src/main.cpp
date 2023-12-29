#include <ga/GA1DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

const int CHESSBOARD_SIZE = 8;
const int POPULATION_SIZE = 500;
const int MAX_GENERATIONS = 2000;

// Objective function
float objective(GAGenome &g)
{
    
}

// Initializer
void initializer(GAGenome &g)
{
    
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

    GA1DArrayGenome<int> genome(CHESSBOARD_SIZE, objective);
    genome.initializer(initializer);
    genome.mutator(mutator);
    genome.crossover(crossover);

    GASimpleGA ga(genome);
    ga.populationSize(POPULATION_SIZE);
    ga.nGenerations(MAX_GENERATIONS);
    ga.pMutation(0.05);
    ga.pCrossover(0.9);
    ga.evolve();

    const GA1DArrayGenome<int> &bestGenome = (GA1DArrayGenome<int> &)ga.statistics().bestIndividual();
    cout << "Best solution found: ";
    for (int i = 0; i < CHESSBOARD_SIZE; i++)
    {
        cout << bestGenome.gene(i) + 1;
    }
    cout << endl;

    return 0;
}