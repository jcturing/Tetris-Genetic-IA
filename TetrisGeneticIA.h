#ifndef TETRISGENETIC_H
#define TETRISGENETIC_H

#include "TetrisHedonisticIA.h"
#include <vector>

/* A little bit of terminology explanation:
- Choromosome: Brain of a single bot. Contains the weights and is a potential solution.
- Gen: A single value of the chromosome.
- Allele: A single bit of a gen. NOT NEEDED HERE.

- Genotype: Chromosome coded (normally binary code). NOT NEEDED HERE.
- Phenotype: Chromosome decoded. NOT NEEDED HERE.

*/

const int POPULATION_MAX = 100; //Even number better!
const int GENERATION_NUM = 100;
const float MUTATION_PROB = 0.07;
const float CROSS_PROB = 0.7;
const int CHROMOSOME_SIZE = 4;

struct chromosome {
	float fitness = 0;
	float height_weight = 0;
	float clears_weight = 0;
	float hole_weight = 0;
	float blockade_weight = 0;
};

class TetrisGeneticIA {

public:
	TetrisGeneticIA();

	std::vector<chromosome> generateInitialPopulation();

	void trainPopulation();

	//Setters
	void setPopulation(std::vector<chromosome> new_population) { population = new_population; }

	//Getters
	std::vector<chromosome> getPopulation() { return population; }
	chromosome getBestChromosome();

private:

	void fitnessPopulation();

	int fitnessChromosome(chromosome const& c);

	void selectionTournament(chromosome &c1, chromosome &c2);

	void crossPairOnePoint(chromosome &c1, chromosome &c2);

	void mutateChromosome(chromosome &c);

	std::vector<chromosome> population;

};

#endif