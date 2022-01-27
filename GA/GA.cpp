/*
	author : Paveetheran
*/

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <algorithm>

using namespace std;

const int GENE = 10;
const int WEIGHT_CAPACITY = 7000;
const int PRICE_CUTOFF = 30;
const int POP_SIZE = 30;
const int WEIGHT[GENE] = { 500,150,20,400,300,350,200,250,700,10 };
const double PRICE[GENE] = { 2.00,0.80,0.30,1.50,1.00,1.30,0.50,0.60,2.00,0.30 };
const float CO_PROB = 0.9;
const float MUT_PROB = 0.2;
const int MAX_GENERATION = 20;

int chromosome[POP_SIZE][GENE];
double fitness[POP_SIZE];
int parents[2][GENE];
int children[2][GENE];
int new_children[2][GENE];
int new_chromosome[POP_SIZE][GENE];
int newChromoCounter = 0;

double bestFitness = 0;
double avgFitness = 0.0;
int bestChromosome[GENE];

ofstream bestFitnessFile, avgFitnessFile, bestChromosomeFile;

// FUNCTIONS

void initializePopulation() {

	int randNum;
	srand(time(NULL));
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < GENE; g++) {
			randNum = rand() % 6;
			chromosome[c][g] = randNum;
		}
	}
}

void evaluateChromosome() {

	double accumulatedWeight = 0.0;
	double accumulatedPrice = 0.0;
	double equation_1 = 0.0;
	double equation_2 = 0.0;

	for (int c = 0; c < POP_SIZE; c++) {
		
		accumulatedWeight = 0.0;
		accumulatedPrice = 0.0;
		equation_1 = 0.0;
		equation_2 = 0.0;
		
		for (int g = 0; g < GENE; g++) {

			if (chromosome[c][g] != 0) {

				accumulatedWeight = accumulatedWeight + (chromosome[c][g] * WEIGHT[g]);
				accumulatedPrice = accumulatedPrice + (chromosome[c][g] * PRICE[g]);

			}
		}

		if (accumulatedWeight <= 7000) {

			equation_1 = 1 - (abs(WEIGHT_CAPACITY - accumulatedWeight) / float(WEIGHT_CAPACITY));
		}
		else {
			equation_1 = 0;
		}

		//if (accumulatedPrice <= PRICE_CUTOFF) {

		//equation_2 = 1 / accumulatedPrice;
			equation_2 = 1 - (abs(PRICE_CUTOFF - accumulatedPrice) / float(PRICE_CUTOFF));
		//}
		//else {
		//	equation_2 = 0;
		//}


		fitness[c] = (equation_1 * 0.5) + (equation_2 * 0.5);

	}
}

void parentTournamentSelection() {

	int player1, player2;
	int indexParents[2];

	do
	{
		for (int p = 0; p < 2; p++) {

			player1 = rand() % POP_SIZE;
			do
			{
				player2 = rand() % POP_SIZE;
			} while (player1 == player2);

			if (fitness[player1] >= fitness[player2]) {

				indexParents[p] = player1;
			}
			else {
				indexParents[p] = player2;
			}
		}
	} while (indexParents[0] == indexParents[1]);

	for (int p = 0; p < 2; p++) {
		for (int g = 0; g < GENE; g++) {
			parents[p][g] = chromosome[indexParents[p]][g];
		}
	}
} 

void parentRankSelectionProgress(int noGen){
	
	// 1. Set l =1, j =1 , i = nogen;
	int l = 0;
	int j = 0;
	int i = noGen;
	double rSum = 0.0;
	double prank = 0.0;
	int random = 0;

	// while 1 <= mpool
	while (l <= POP_SIZE) {

		// while j <= N
		while (j <= POP_SIZE) {

			rSum = rSum + fitness[j];
		}

		// Set j = 1
		j = 0;

		// while j <= N
		while (j <= POP_SIZE) {

			prank = fitness[j] / rSum;
		}

		random = rand() % (int)rSum;

		j = 0;
		int S = 0;
		while (j <= POP_SIZE) {

		}



	}

}

void parentRankSelectionProgress() {

	//	Higher FV is the best in our problem
	//	So, I'm going to ranking it from high to low fitness.
	//	Finally pick two highest FV as parents 

	int indexParent[2];
	double sortFitness[POP_SIZE];

	for (int assign = 0; assign < POP_SIZE; assign++) {

		sortFitness[assign] = fitness[assign];
	}

	sort(sortFitness, sortFitness + POP_SIZE, greater<double>());

	for (int f = 0; f < POP_SIZE; f++) {

		if (sortFitness[0] == fitness[f]) {
			indexParent[0] = f;
		}

		if (sortFitness[1] == fitness[f]) {
			indexParent[1] = f;
		}
	}

	for (int p = 0; p < 2; p++) {
		for (int g = 0; g < GENE; g++) {

			parents[p][g] = chromosome[indexParent[p]][g];
		}
	}

}

void crossoverOnePoint() {

	float prob = 0.0;
	int co_point;

	for (int p = 0; p < 2; p++) {
		for (int g = 0; g < GENE; g++) {
			children[p][g] = parents[p][g];
		}
	}

	prob = ((rand() % 10) + 1) / 10.0;

	if (prob < CO_PROB) {

		co_point = rand() % GENE;
		for (int g = co_point; g < GENE; g++) {

			children[0][g] = parents[1][g];
			children[1][g] = parents[0][g];
		}
	}
	else {

	}
}


void crossoverNPoint() {

	float prob = 0.0;
	int co_point_1 = 0;
	int co_point_2 = 0;

	for (int p = 0; p < 2; p++) {
		for (int g = 0; g < GENE; g++) {
			children[p][g] = parents[p][g];
		}
	}

	prob = ((rand() % 10) + 1) / 10.0;

	if (prob < CO_PROB) {

		co_point_1 = rand() % 5;
		for (int g = co_point_1; g < GENE; g++) {

			children[0][g] = parents[1][g];
			children[1][g] = parents[0][g];
		}		
		
		co_point_2 = rand() % 5 + 5;
		for (int g = co_point_2; g < GENE; g++) {

			children[0][g] = parents[0][g];
			children[1][g] = parents[1][g];
		}

	}
	else {

	}
	/*
	// Output
	cout << "Children " << " Crossover at " << co_point_1 << " & " << co_point_2 << endl;
	for (int c = 0; c < 2; c++) {

		cout << endl;
		cout << "Parent \t\t";
		for (int g = 0; g < GENE; g++) {

			cout << parents[c][g] << " ";
		}

		cout << endl;
		cout << endl;
	}	
	
	for (int c = 0; c < 2; c++) {

		cout << "Children " << c + 1 << "\t";
		for (int g = 0; g < GENE; g++) {
			cout << children[c][g] << " ";
		}
		cout << endl;

		cout << endl;
	}*/
}


void mutationSwap() {

	float prob = 0.0;
	int mut_point1;
	int mut_point2;
	int ori_value1, ori_value2;

	for (int c = 0; c < 2; c++) {

		prob = (rand() % 11) / 10.0;
		if (prob < MUT_PROB) {

			mut_point1 = rand() % GENE;
			mut_point2 = rand() % GENE;

			ori_value1 = children[c][mut_point1];
			ori_value2 = children[c][mut_point2];

			children[c][mut_point1] = ori_value2;
			children[c][mut_point2] = ori_value1;

		}
		else {
		
		}
	}
}

void mutationBitFilp() {

	float prob;
	int mut_point;
	int gene_value;

	for (int c = 0; c < 2; c++) {
		prob = (rand() % 11) / 10.0;
		if (prob < MUT_PROB) {
			mut_point = rand() % GENE;
			gene_value = rand() % 6;
			//cout << "\n\t Children mutated at " << mut_point;
			//if (children[c][mut_point] == 0)
			//	children[c][mut_point] = 1;
			//else
			//	children[c][mut_point] = 0;
			children[c][mut_point] = gene_value;
		}
		else {
			//cout << "\n\t Mutation did not happen ";
		}
	}
	//output
	//for (int c = 0; c < 2; c++) {
	//	cout << "\n\t Children " << c + 1 << " after mutation : ";
	//	for (int g = 0; g < GENE; g++) {
	//		cout << children[c][g] << " ";
	//	}
	//}
}
void survivalSelection() {

	for (int c = 0; c < 2; c++) {
		for (int g = 0; g < GENE; g++) {
			new_chromosome[newChromoCounter][g] = children[c][g];
		}
		newChromoCounter++;
	}
}

void copy_Chromosome() {

	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < GENE; g++)
		{
			chromosome[c][g] = new_chromosome[c][g];
		}
	}
}

void recordBestFitness(int i) {

	for (int c = 0; c < POP_SIZE; c++) {
		if (bestFitness < fitness[c]) {
			bestFitness = fitness[c];
			for (int g = 0; g < GENE; g++) {
				bestChromosome[g] = chromosome[c][g];
			}
		}
	}

	//bestFitnessFile << "Generation " << i + 1 << " \t: ";
	bestFitnessFile << bestFitness << endl;
	//bestChromosomeFile << "Generation " << i + 1 << " \t: ";
	for (int g = 0; g < GENE; g++) {
		bestChromosomeFile << bestChromosome[g] << " ";
	}
	bestChromosomeFile << endl;

}

void calcAvgFitness(int i) {

	double sum = 0.0;
	for (int c = 0; c < POP_SIZE; c++) {
		sum += fitness[c];
	}
	avgFitness = sum / POP_SIZE;

	avgFitnessFile << avgFitness << endl;
}

void report() {

	double totalWeight = 0.0;
	double totalPrice = 0.0;

	cout << endl << endl;
	
	cout << "\tBest Chromosome\t: ";
	for (int g = 0; g < GENE; g++) {

		cout << bestChromosome[g] << setw(5);
		totalWeight = totalWeight + (bestChromosome[g] * WEIGHT[g]);
		totalPrice = totalPrice + (bestChromosome[g] * PRICE[g]);
	}
	
	cout << endl;
	cout << "\tTotal Weight\t: " << totalWeight << " g " << endl;
	cout << "\tTotal Price\t: RM " << totalPrice << endl;

}

int main() {
	
	cout << "\n\tGenetic Algorithm" << endl;
	cout << "\n\tInitialzation... \n";

	bestFitnessFile.open("bestFitness.txt");
	avgFitnessFile.open("avgFitness.txt");
	bestChromosomeFile.open("bestChromosome.txt");

	initializePopulation();
	getchar();

	for (int g = 0; g < MAX_GENERATION; g++) {

		cout << "\tGENERATION " << g + 1 << endl;
		evaluateChromosome();
		recordBestFitness(g);
		calcAvgFitness(g);
		newChromoCounter = 0;

		for (int i = 0; i < POP_SIZE/2; i++) {

			parentTournamentSelection();
			//parentRankSelectionProgress();
			crossoverOnePoint();
			//mutationSwap();
			mutationBitFilp();
			survivalSelection();
		}

		copy_Chromosome();
		//Sleep(1000);
	}

	report();
	bestFitnessFile.close();
	avgFitnessFile.close();
	bestChromosomeFile.close();
	


	return 0;
}