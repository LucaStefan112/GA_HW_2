#include <iostream>
#include <math.h>
#include <string>
#include "Functions.h"

#define POPULATION_SIZE 100
#define MUTATION_RATE 0.05
#define MAX_SAME_VALUE_POPULATIONS 5000
#define MAX_VALUE 999999999
#define RAND_PRECISION 1000000

int dimension, functionType, sameValuePopulations;
Functions thisFunction;

std::vector<std::vector<int> > population;
std::vector<float> individualScore;

float populationMin, globalMin;

double time_taken;

std::vector<int> nullVector;

struct Children{
    std::vector<int> first, second;
};

std::string getOsName()
{
    #ifdef _WIN32
    return "Windows";
    #elif _WIN64
    return "Windows";
    #elif __APPLE__ || __MACH__
    return "Mac";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
} 

void clearConsole(){
    if(getOsName() == "Windows")
        system("cls");
    else
        system("clear");
}

void readData(){
    clearConsole();

    std::cout << "Introduceti dimensiunea: ";
    std::cin >> dimension;
    clearConsole();

    std::cout << "0 -> De Jong's Function 1;\n1 -> Schwefel's Function 7;\n2 -> Rastrigin's Function 6;\n3 -> Michalewicz's Function 12;\n\n";
    std::cout << "Introduceti functia: ";
    std::cin >> functionType;
    clearConsole();

    thisFunction.selectFunction(static_cast<Functions::FunctionTypes>(functionType), dimension);
    for(int i = 0; i < POPULATION_SIZE; i++){
        population.push_back(nullVector);
        individualScore.push_back(0);
    }
}

// Cross-over mechanism:
Children getChildren(std::vector<int> parent1, std::vector<int> parent2){
    const int randomValue = rand() % (dimension * thisFunction.bitSpace) + 1;
    Children pairOfChildren;

    for(int i = 0; i < randomValue; i++){
        pairOfChildren.first.push_back(parent1[i]);
        pairOfChildren.second.push_back(parent2[i]);
    }

    for(int i = randomValue; i < dimension * thisFunction.bitSpace; i++){
        pairOfChildren.first.push_back(parent2[i]);
        pairOfChildren.second.push_back(parent1[i]);
    }

    return pairOfChildren;
}

float evaluatePopulation(){
    float minValue = MAX_VALUE;
    for(int i = 0; i < POPULATION_SIZE; i++)
        minValue = std::min(minValue, thisFunction.getResult(population[i]));
    
    return minValue;
}

void setIndividualScore(){
    float localMin = MAX_VALUE, valueSum = 0;

    for(int i = 0; i < POPULATION_SIZE; i++){
        individualScore[i] = -thisFunction.getResult(population[i]);
        localMin = std::min(localMin, individualScore[i]);
        valueSum += individualScore[i];
    }

    localMin = abs(localMin) + 1;
    valueSum += (localMin * POPULATION_SIZE);

    for(int i = 0; i < POPULATION_SIZE; i++){
        individualScore[i] = (individualScore[i] + localMin) / valueSum;
        if(i)
            individualScore[i] += individualScore[i - 1];
    }
}

void ShowPopulation(){
    std::cout << "\n";
    for(int i = 0; i < POPULATION_SIZE; i++){
        for(int j = 0; j < dimension * thisFunction.bitSpace; j++)
            std::cout << population[i][j];
        std::cout << '\n';
    }

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    std::cout << '\n';
}

void computeAlgorithm(){
    globalMin = MAX_VALUE;

    // Generate starting population:
    for(int i = 0; i < POPULATION_SIZE; i++)
        population[i] = thisFunction.generateRandom();
    
    // ShowPopulation();
    setIndividualScore();

    // Continue while the populations evolve:
    while (sameValuePopulations < MAX_SAME_VALUE_POPULATIONS){
        std::vector<std::vector<int> > newPopulation;

        // Select the new population:
        for(int i = 0; i < POPULATION_SIZE; i++){
            float randomValue = float(rand() % RAND_PRECISION) / RAND_PRECISION;
            int index = 0;

            while(randomValue > individualScore[index])
                index++;

            newPopulation.push_back(population[index]);
        }

        // Mutation:
        for(int i = 0; i < POPULATION_SIZE; i++){
            if(float(rand() % RAND_PRECISION) / RAND_PRECISION <= MUTATION_RATE){
                const int 
                    randIndividual = rand() % POPULATION_SIZE,
                    randBit = rand() % (thisFunction.bitSpace * dimension);
                newPopulation[randIndividual][randBit] = 1 - newPopulation[randIndividual][randBit];
            }
        }

        // Sorting population:
        for(int i = 0; i < POPULATION_SIZE - 1; i++)
            for(int j = i + 1; j < POPULATION_SIZE; j++)
                if(individualScore[j] < individualScore[i]){
                    std::swap(individualScore[i], individualScore[j]);
                    std::swap(newPopulation[i], newPopulation[j]);
                }

        // Cross-over:
        for(int i = 0; i < POPULATION_SIZE; i += 2){
            Children pairOfChildren = getChildren(newPopulation[i], newPopulation[i + 1]);
            newPopulation[i] = pairOfChildren.first;
            newPopulation[i + 1] = pairOfChildren.second;
        }

        population = newPopulation;

        setIndividualScore();

        populationMin = evaluatePopulation();
        if(globalMin <= populationMin)
            sameValuePopulations++;
        else{
            sameValuePopulations = 0;
            globalMin = populationMin;
        }
    }
}

void algorithmData(){
    std::cout << "Result: " << globalMin << '\n';
    std::cout << "Time: " << time_taken << " seconds\n";
    std::cout << "Function Type: ";
    switch (functionType){
        case 0:
        std::cout << "De Jong's Function 1";
        break;
        case 1:
        std::cout << "Schwefel's Function 7";
        break;
        case 2:
        std::cout << "Rastrigin's Function 6";
        break;
        case 3:
        std::cout << "Michalewicz's Function 12";
        break;
    }
    std::cout << '\n';
    std::cout << "Dimension: " << dimension << '\n';
    std::cout << '\n';
}

int main(){
    clock_t start, end;
    start = clock();
    srand(time(0));
    
    readData();

    computeAlgorithm();

    end = clock();

    time_taken = (double(end - start) / double(CLOCKS_PER_SEC));

    algorithmData();

    return 0;
}