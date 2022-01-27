#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>

using namespace std;

const int GENE = 10;
const int WEIGHT_CAPACITY = 7000;
const int POP_SIZE = 30;
const int WEIGHT[GENE] = { 500,150,20,400,300,350,200,250,700,10 };
const int PRICE[GENE] = { 7,4,3,8,8,8,4,10,12,2 };
const float CO_PROB = 0.9;
const float MUT_PROB = 0.9;
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

void initializePopulation() {

	int randNum;
	srand(time(NULL));
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < GENE; g++) {
			randNum = rand() % 21;
			chromosome[c][g] = randNum;
		}
	}
}

void printChromosome() {
	
	cout << "\n\t=> View Chromosome Population\n" << endl;

	cout << "\t\t";
	for (int g = 0; g < GENE; g++) {
		cout << setw(5) << g+1 << setw(5);
	}
	cout << "\n\t\t-------------------------------------------------------";
	cout << endl;

	for (int c = 0; c < POP_SIZE; c++) {
		int count = c;
		cout << "\tC" << count + 1 << "\t";
		for (int g = 0; g < GENE; g++) {

			cout << setw(5) <<chromosome[c][g] << setw(5);
		}
		cout << endl;
	}

	cout << endl << endl;
}

void evaluateChromosomeVersion1() {

	/* Default Variable */
	double accumulatedWeight = 0;
	double accumulatedPrice = 0;
	double equation_1 = 0;
	double equation_2 = 0;

	for (int c = 0; c < POP_SIZE; c++) {

		accumulatedWeight = 0;
		accumulatedPrice = 0;
		equation_1 = 0;
		equation_2 = 0;
		// Sum Up All Weight 
		for (int g = 0; g < GENE; g++) {

			if (chromosome[c][g] != 0) {

				accumulatedWeight = accumulatedWeight + (chromosome[c][g] * WEIGHT[g]);
				accumulatedPrice = accumulatedPrice + (chromosome[c][g] * PRICE[g]);

			}
		}

		equation_1 = 1 -(abs(WEIGHT_CAPACITY - 5000) / float(WEIGHT_CAPACITY));
		equation_2 = 1 / accumulatedPrice;
		fitness[c] = (equation_1*0.5) + (equation_2*0.5);

		/*
		cout << "Analysis Equation 1" << endl;
		cout << "abs(WEIGHT_CAPACITY - accumulatedWeight) \t:" << abs(WEIGHT_CAPACITY - accumulatedWeight) << endl;
		cout << "float(WEIGHT_CAPACITY) \t" << float(WEIGHT_CAPACITY) << endl;
		cout << "abs(WEIGHT_CAPACITY - accumulatedWeight) / float(WEIGHT_CAPACITY) \t" << abs(WEIGHT_CAPACITY - accumulatedWeight) / float(WEIGHT_CAPACITY) << endl << endl;

		cout << "(equation_1*0.5)" << (equation_1 * 0.5) << endl;
		cout << "(equation_2*0.5)" << (equation_2 * 0.5) << endl;
		*/


		cout << "Weight    \t: " << accumulatedWeight << endl;
		cout << "Price     \t: " << accumulatedPrice << endl;
		cout << "Equation 1\t: " << equation_1 << endl;
		cout << "Equation 2\t: " << equation_2 << endl;
		cout << "Fitness   \t: " << fitness[c] << endl << endl;
		cout << "---------------------------------------------------------" << endl;
	}



}

void evaluateChromosomeVersion2() {

	/* Default Variable */
	double accumulatedWeight = 0;
	double accumulatedPrice = 0;
	double equation_1 = 0;
	double equation_2 = 0;

	for (int c = 0; c < POP_SIZE; c++) {

		accumulatedWeight = 0;
		accumulatedPrice = 0;
		equation_1 = 0;
		equation_2 = 0;
		// Sum Up All Weight and Price 
		for (int g = 0; g < GENE; g++) {

			if (chromosome[c][g] != 0) {

				accumulatedWeight = accumulatedWeight + (chromosome[c][g] * WEIGHT[g]);
				accumulatedPrice = accumulatedPrice + (chromosome[c][g] * PRICE[g]);

			}
		}

		// Start : Part 1
		if (accumulatedWeight <= 7000) {

			equation_1 = 1 - (abs(WEIGHT_CAPACITY - accumulatedWeight)/float(WEIGHT_CAPACITY));
		}
		else {
			equation_1 = 0;
		}
		// End : Part 1

		// Start : Part 2
		equation_2 = 1 / accumulatedPrice;
		// End : Part 2

		//  Generate Fitness Value
		fitness[c] = (equation_1*0.5) + (equation_2*0.5);

		/*
		cout << "Weight    \t: " << accumulatedWeight << endl;
		cout << "Price     \t: " << accumulatedPrice << endl;
		cout << "Equation 1\t: " << equation_1 << endl;
		cout << "Equation 2\t: " << equation_2 << endl;
		cout << "Fitness   \t: " << fitness[c] << endl << endl;
		cout << "---------------------------------------------------------" << endl;
		*/ 

		//cout << "Fitness   \t: " << fitness[c] << endl << endl;

	}
}

void printEvaluate() {

	cout << "\t=> Fitness Evaluate\n" << endl;
	
	for (int c = 0; c < POP_SIZE; c++) {

		cout << "\tFitness " << c << "\t: " << fitness[c] << endl;
	}

	cout << endl;
}

void parentSelection() {

	cout << "\n\t=> Parent Selection\n" << endl;
	int player1, player2;
	int indexParents[2];
	do
	{
		for (int p = 0; p < 2; p++)
		{
			player1 = rand() % POP_SIZE;
			do
			{
				player2 = rand() % POP_SIZE;
			} while (player1 == player2);

			if (fitness[player1] >= fitness[player2])
			{
				indexParents[p] = player1;
			}
			else {
				indexParents[p] = player2;
			}

			cout << "\tTournament " << p + 1 << endl;
			cout << "\t---------------------------------------------" << endl;
			cout << "\tPlayers: " << player1 << " vs " << player2 << endl;
			cout << "\tFitness: " << fitness[player1] << " vs " << fitness[player2] << endl;
			cout << "\tWinner : " << indexParents[p] << endl << endl;
		}

	} while (indexParents[0] == indexParents[1]);

	cout << "\n\tParent Selected " << endl;
	cout << "\t------------------------------------------";

	cout << "\n\t\t\t  ";
	for (int g = 0; g < GENE; g++) {
		cout << setw(5) << g + 1 << setw(5);
	}
	
	cout << endl;

	for (int p = 0; p < 2; p++) {
		cout << "\n\tParents " << p + 1 << "\t: ";
		for (int g = 0; g < GENE; g++) {
			parents[p][g] = chromosome[indexParents[p]][g];
			cout << setw(5) <<parents[p][g] << setw(5);
		}
	}

	cout << endl << endl;
}

void crossover() {

	cout << "\n\t=> Crossover\n" << endl;

	float prob = 0.0;
	int co_point;

	// Copy Parents to Children
	for (int p = 0; p < 2; p++) {
		for (int g = 0; g < GENE; g++) {
			children[p][g] = parents[p][g];
		}
	}

	// Generate 0 - 20
	prob =  ((rand() % 10) + 1) / 10.0;

	//cout << "\nPROB : "<< CO_PROB << endl;
	//cout << "\nPROB : "<< prob << endl;
	if (prob < CO_PROB) {

		co_point = rand() % GENE;
		cout << "\n\tChlidren Crossover at " << co_point + 1 << endl;
		cout << "\t-------------------------------------" << endl;
		for (int g = co_point; g < GENE; g++) {

			children[0][g] = parents[1][g];
			children[1][g] = parents[0][g];
		}
	}
	else {
		cout << "\n\tCrossover did not happen" << endl;
	}

	//output

	cout << "\n\t\t\t  ";
	for (int g = 0; g < GENE; g++) {
		cout << setw(5) << g + 1 << setw(5);
	}

	cout << endl;

	for (int c = 0; c < 2; c++) {
		cout << "\n\tChildren " << c + 1 << "\t: ";
		for (int g = 0; g < GENE; g++) {
			cout << setw(5) << children[c][g] << setw(5);
		}
	}

	cout << endl << endl;
}

void mutationVersion1() {

	cout << "\n\t=> Mutation\n" << endl;
	float prob = 0.0;
	int mut_point1;
	int mut_point2;
	int ori_value1, ori_value2;

	for (int c = 0; c < 2; c++) {

	prob = (rand() % 11) / 10.0;
	if (prob < MUT_PROB) {

		mut_point1 = rand() % GENE;
		mut_point2 = rand() % GENE;

		cout << "\n\tSELECTED MUTATION GENE " << mut_point1 + 1 << " & " << mut_point2 + 1 << "  FOR CHILDREN " << c + 1 << endl;
		cout << "\t----------------------------------------------" << endl;
		ori_value1 = children[c][mut_point1];
		ori_value2 = children[c][mut_point2];

		children[c][mut_point1] = ori_value2;
		children[c][mut_point2] = ori_value1;

	}
	else {

		cout << "\tMutation doesn't happen" << endl;
	}

	}

	cout << "\n\t\t\t\t  ";
	for (int g = 0; g < GENE; g++) {
		cout << setw(5) << g + 1 << setw(5);
	}

	for (int c=0; c < 2; c++){
	cout << "\n\tMutated Children " << c + 1 << " \t: ";
	for (int g = 0; g < GENE; g++) {

		cout << setw(5) << children[c][g] << setw(5);
	}
	}
	cout << endl;
}

void survivalSelection() {

	for (int c = 0; c < 2; c++) {
		for (int g = 0; g < GENE; g++) {
			new_chromosome[newChromoCounter][g] = children[c][g];
		}
		newChromoCounter++;
	}

	for (int c = 0; c < newChromoCounter; c++) {

		cout << "\n\tNew Chromosome " << c + 1 << "\t: ";
		for (int g = 0; g < GENE; g++)
		{
			cout << setw(5) << new_chromosome[c][g] << setw(5);
		}
	}

	cout << endl;
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

	cout << "\nBest Fitness = " << bestFitness;
	cout << "\nBest Chromosome = ";
	for (int g = 0; g < GENE; g++) {
		cout << bestChromosome[g] << " ";
	}
	cout << endl;

	bestFitnessFile << "Generation " << i + 1 << " \t: ";
	bestFitnessFile << bestFitness << endl;
	bestChromosomeFile << "Generation " << i + 1 << " \t: ";
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

	// Output to Monitor
	cout << "\n Average Fitness = " << avgFitness;

	// Output to File
	avgFitnessFile << avgFitness << endl;
}

int main() {

	cout << "\nGenetic Algorithm\n" << endl;

	//initializePopulation();
	//printChromosome();
	//evaluateChromosomeVersion2();
	//printEvaluate();
	//parentSelection();
	//crossover();
	//mutationVersion1();
	//survivalSelection();

	cout << "\n\tInitialzation... \n";

	bestFitnessFile.open("bestFitness.txt");
	avgFitnessFile.open("avgFitness.txt");
	bestChromosomeFile.open("bestChromosome.txt");

	initializePopulation();
	getchar();

	for (int generation = 0; generation < MAX_GENERATION; generation++) {

		cout << "\n\tGeneration " << generation + 1 << endl;
		cout << "\nPrint Population\n";
		printChromosome();

		cout << "\n\tEvaluate Chromosome \n";
		evaluateChromosomeVersion2();
		printEvaluate();
		recordBestFitness(generation);
		calcAvgFitness(generation);
		newChromoCounter = 0;
		for (int i = 0; i < POP_SIZE/2; i++)
		{
			cout << "\nPARENT SELECTION \n";
			parentSelection();
			cout << "\nCROSSOVER \n";
			crossover();
			cout << "\nMUTATION \n";
			mutationVersion1();
			cout << "\nSURVIVAL SELECTION \n";
			survivalSelection();
		}

		cout << "\nNEW CHROMOSOMES COPIED TO CHROMOSOME \n";
		copy_Chromosome();
	}

	bestFitnessFile.close();
	avgFitnessFile.close();
	bestChromosomeFile.close();
	return 0;
}

