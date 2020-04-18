#include "TetrisGeneticIA.h"


TetrisGeneticIA::TetrisGeneticIA() {

}

std::vector<chromosome> TetrisGeneticIA::generateInitialPopulation() {
	std::vector<chromosome> random_population;

	for (int i = 0; i < POPULATION_MAX; i++) {
		chromosome aux;
		aux.height_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		aux.clears_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		aux.hole_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		aux.blockade_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		random_population.push_back(aux);
	}

	return random_population;
}

void TetrisGeneticIA::trainPopulation() {
	for (int i = 0; i < GENERATION_NUM; i++) {
		std::cout << "GENERACION: " << i << std::endl;
		fitnessPopulation();
		//Selection -> Cross -> Mutate
		std::vector<chromosome> next_population;
		for (int j = 0; j < POPULATION_MAX / 2; j++) {
			chromosome c1, c2;
			//Select a pair
			selectionTournament(c1, c2);
			
			//Cross a pair
			crossPairOnePoint(c1, c2);
			
			//Mutate both chromosomes if needed
			mutateChromosome(c1);
			mutateChromosome(c2);
			
			//Push them into the next generation population
			next_population.push_back(c1);
			next_population.push_back(c2);
		}
		population = next_population;
	}
	std::cout << "FIN" << std::endl;
}

void TetrisGeneticIA::fitnessPopulation() {
	for (int i = 0; i < population.size(); i++) {
		population[i].fitness = fitnessChromosome(population[i]);
	}
}

int TetrisGeneticIA::fitnessChromosome(chromosome const& c) {
	TetrisHedonisticIA trainer_bot(c.height_weight, c.clears_weight, c.hole_weight, c.blockade_weight);
	TetrisGame trainer_game;
	std::vector<bool> trainer_keys(ALLEGRO_KEY_MAX);

	//Playing
	trainer_game.setState(RUNNING);
	trainer_game.generateNewPiece();
	trainer_bot.calculateDestination(trainer_game.getTable(), trainer_game.getPiece());
	
	while (trainer_game.getState() != GAME_OVER) {
		GameDirection bot_next_movement = trainer_bot.getNextMovement(trainer_game.getTable(), trainer_game.getPiece());
		switch (bot_next_movement) {
			case UP: trainer_keys[KEY_UP] = true; break;
			case RIGHT: trainer_keys[KEY_RIGHT] = true; break;
			case DOWN: trainer_keys[KEY_DOWN] = true; break;
			case LEFT: trainer_keys[KEY_LEFT] = true; break;
		}

		trainer_game.execute(trainer_keys, false, true);

		if (trainer_game.getPiecePlaced()) { //Calculate next destination of Bot if the piece has been placed
			trainer_bot.calculateDestination(trainer_game.getTable(), trainer_game.getPiece());
			trainer_game.setPiecePlaced(false);
		}
	}

	return trainer_game.getScore();
}

void TetrisGeneticIA::selectionTournament(chromosome &c1, chromosome &c2) {
	//Torneo c1
	int rand_sel1 = rand() % POPULATION_MAX;
	int rand_sel2 = rand() % POPULATION_MAX;

	if (population[rand_sel1].fitness > population[rand_sel2].fitness) c1 = population[rand_sel1];
	else c1 = population[rand_sel2];

	//Torneo c2
	rand_sel1 = rand() % POPULATION_MAX;
	rand_sel2 = rand() % POPULATION_MAX;

	if (population[rand_sel1].fitness > population[rand_sel2].fitness) c2 = population[rand_sel1];
	else c2 = population[rand_sel2];
}

void TetrisGeneticIA::crossPairOnePoint(chromosome &c1, chromosome &c2) {
	int prob_cross = rand() % 101;
	if (prob_cross < CROSS_PROB * 100) {
		chromosome cross_c1 = c1, cross_c2 = c2;
		int cross_point = 1 + rand() % (CHROMOSOME_SIZE - 1); //Possible values: 1, 2, 3
		switch (cross_point) {
		case 1:
			cross_c1.clears_weight = c2.clears_weight;
			cross_c1.hole_weight = c2.hole_weight;
			cross_c1.blockade_weight = c2.blockade_weight;

			cross_c2.clears_weight = c1.clears_weight;
			cross_c2.hole_weight = c1.hole_weight;
			cross_c2.blockade_weight = c1.blockade_weight;
			break;
		case 2:
			cross_c1.hole_weight = c2.hole_weight;
			cross_c1.blockade_weight = c2.blockade_weight;

			cross_c2.hole_weight = c1.hole_weight;
			cross_c2.blockade_weight = c1.blockade_weight;
			break;
		case 3:
			cross_c1.blockade_weight = c2.blockade_weight;

			cross_c2.blockade_weight = c1.blockade_weight;
			break;
		}
		c1 = cross_c1;
		c2 = cross_c2;
	}
}

void TetrisGeneticIA::mutateChromosome(chromosome &c) {
	int prob_mut = rand() % 101;
	if (prob_mut < MUTATION_PROB * 100) {
		int pos_mutar = 1 + rand() % CHROMOSOME_SIZE;
		switch (pos_mutar) {
			case 1: c.height_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); break;
			case 2: c.clears_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); break;
			case 3: c.hole_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); break;
			case 4: c.blockade_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); break;
		}
	}
}

chromosome TetrisGeneticIA::getBestChromosome() {
	chromosome c;
	c.fitness = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].fitness > c.fitness) c = population[i];
	}
	return c;
}