#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const int POPULATION_SIZE = 50;
const int MAX_GENERATIONS = 1000;
const double MUTATION_RATE = 0.01;

// Using shuffle
random_device rd;
mt19937 g(rd());

struct City {
    int id;
    double x, y;
};

// Function to calculate distance between two cities
double distance(const City& city1, const City& city2) {
    double dx = city1.x - city2.x;
    double dy = city1.y - city2.y;
    return sqrt(dx*dx + dy*dy);
}

// Function to calculate total distance of a tour
double calculateTotalDistance(const vector<City>& cities, const vector<int>& tour) {
    double total = 0;
    for (int i = 0; i < tour.size() - 1; i++) {
        total += distance(cities[tour[i]], cities[tour[i+1]]);
    }
    total += distance(cities[tour.back()], cities[tour.front()]); // Return to start
    return total;
}

// Function to generate a random tour
vector<int> generateRandomTour(int numCities) {
    vector<int> tour(numCities);
    for (int i = 0; i < numCities; i++) {
        tour[i] = i;
    }
    shuffle(tour.begin(), tour.end(), g);
    return tour;
}

// Function to perform tournament selection
vector<int> tournamentSelection(const vector<vector<int>>& population, const vector<double>& fitness) {
    int tournamentSize = 5;
    int bestIndex = rand() % population.size();
    for (int i = 1; i < tournamentSize; i++) {
        int index = rand() % population.size();
        if (fitness[index] < fitness[bestIndex]) {
            bestIndex = index;
        }
    }
    return population[bestIndex];
}

// Function to perform order crossover (OX)
vector<int> orderCrossover(const vector<int>& parent1, const vector<int>& parent2) {
    int size = parent1.size();
    vector<int> child(size, -1);
    int start = rand() % size;
    int end = rand() % size;
    if (start > end) swap(start, end);

    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
    }

    int index = (end + 1) % size;
    for (int i = 0; i < size; i++) {
        int current = parent2[(end + 1 + i) % size];
        if (find(child.begin(), child.end(), current) == child.end()) {
            child[index] = current;
            index = (index + 1) % size;
        }
    }

    return child;
}

// Function to perform mutation
void mutate(vector<int>& tour) {
    for (int i = 0; i < tour.size(); i++) {
        if ((double)rand() / RAND_MAX < MUTATION_RATE) {
            int j = rand() % tour.size();
            swap(tour[i], tour[j]);
        }
    }
}

int main() {
    srand(time(0));

    // Read input file
    ifstream input("input.txt");
    int numCities;
    input >> numCities;
    vector<City> cities(numCities);
    for (int i = 0; i < numCities; i++) {
        input >> cities[i].id >> cities[i].x >> cities[i].y;
    }
    input.close();

    // Initialize population
    vector<vector<int>> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = generateRandomTour(numCities);
    }

    // Main GA loop
    vector<int> bestTour;
    double bestDistance = numeric_limits<double>::max();

    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        // Calculate fitness
        vector<double> fitness(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness[i] = calculateTotalDistance(cities, population[i]);
            if (fitness[i] < bestDistance) {
                bestDistance = fitness[i];
                bestTour = population[i];
            }
        }

        // Create new generation
        vector<vector<int>> newPopulation(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; i++) {
            vector<int> parent1 = tournamentSelection(population, fitness);
            vector<int> parent2 = tournamentSelection(population, fitness);
            vector<int> child = orderCrossover(parent1, parent2);
            mutate(child);
            newPopulation[i] = child;
        }

        population = newPopulation;
    }

    // Write output file
    ofstream output("output.txt");
    output << "Best tour: ";
    for (int city : bestTour) {
        output << city << " ";
    }
    output << endl;
    output << "Total distance: " << bestDistance << endl;
    output.close();

    cout << "Genetic Algorithm completed. Results written to output.txt" << endl;

    return 0;
}