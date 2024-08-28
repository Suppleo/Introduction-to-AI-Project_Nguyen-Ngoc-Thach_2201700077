#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

// Peak function
double peakFunction(double x, double y) {
    return pow(1 - x, 2) * exp(-pow(x, 2) - pow(y + 1, 2)) - (x - pow(x, 3) - pow(y, 3)) * exp(-pow(x, 2) - pow(y, 2));
}

// Individual class
class Individual {
public:
    double x, y, fitness;

    Individual(double x = 0, double y = 0) : x(x), y(y) {
        fitness = peakFunction(x, y);
    }

    void mutate(double mutationRate, std::mt19937& gen) {
        std::uniform_real_distribution<> dis(-mutationRate, mutationRate);
        x += dis(gen);
        y += dis(gen);
        fitness = peakFunction(x, y);
    }
};

// Genetic Algorithm class
class GeneticAlgorithm {
private:
    std::vector<Individual> population;
    int populationSize;
    double mutationRate;
    int generations;
    std::mt19937 gen;

public:
    GeneticAlgorithm(int popSize, double mutRate, int gens) 
        : populationSize(popSize), mutationRate(mutRate), generations(gens) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        gen.seed(seed);
    }

    void initialize() {
        std::uniform_real_distribution<> dis(-5, 5);
        for (int i = 0; i < populationSize; ++i) {
            population.emplace_back(dis(gen), dis(gen));
        }
    }

    Individual selectParent() {
        std::uniform_int_distribution<> dis(0, populationSize - 1);
        Individual parent1 = population[dis(gen)];
        Individual parent2 = population[dis(gen)];
        return (parent1.fitness > parent2.fitness) ? parent1 : parent2;
    }

    Individual crossover(const Individual& parent1, const Individual& parent2) {
        std::uniform_real_distribution<> dis(0, 1);
        double alpha = dis(gen);
        double x = alpha * parent1.x + (1 - alpha) * parent2.x;
        double y = alpha * parent1.y + (1 - alpha) * parent2.y;
        return Individual(x, y);
    }

    void evolve() {
        std::vector<Individual> newPopulation;
        for (int i = 0; i < populationSize; ++i) {
            Individual parent1 = selectParent();
            Individual parent2 = selectParent();
            Individual child = crossover(parent1, parent2);
            child.mutate(mutationRate, gen);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }

    Individual getBest() {
        return *std::max_element(population.begin(), population.end(),
            [](const Individual& a, const Individual& b) { return a.fitness < b.fitness; });
    }

    void run(std::ofstream& outFile) {
        initialize();
        
        for (int gen = 0; gen < generations; ++gen) {
            evolve();
            Individual best = getBest();
            
            // Write to output file every 100 generations
            if (gen % 100 == 0) {
                outFile << "Generation " << gen << ": Best fitness = " << best.fitness
                        << " at (x, y) = (" << best.x << ", " << best.y << ")\n";
            }
        }
        
        Individual finalBest = getBest();
        outFile << "\nFinal result:\n";
        outFile << "Best fitness = " << finalBest.fitness << " at (x, y) = (" 
                << finalBest.x << ", " << finalBest.y << ")\n";
    }
};

int main() {
    std::ofstream outFile("mathsolve_results.txt");
    if (!outFile) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    // GA parameters
    int populationSize = 100;
    double mutationRate = 0.1;
    int generations = 1000;

    GeneticAlgorithm ga(populationSize, mutationRate, generations);
    ga.run(outFile);

    outFile.close();
    std::cout << "Results written to ga_results.txt\n";
    return 0;
}