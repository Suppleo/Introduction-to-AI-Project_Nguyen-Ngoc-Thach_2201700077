#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Function to calculate distance between two cities
int calculateDistance(pair<int, int> city1, pair<int, int> city2) {
    int dx = city1.first - city2.first;
    int dy = city1.second - city2.second;
    return dx * dx + dy * dy;  // We're using squared distance to avoid floating-point calculations
}

// Greedy algorithm for TSP
vector<int> greedyTSP(const vector<pair<int, int>>& cities) {
    int n = cities.size();
    vector<bool> visited(n, false);
    vector<int> tour;
    tour.push_back(0);  // Start with the first city
    visited[0] = true;

    for (int i = 1; i < n; i++) {
        int lastCity = tour.back();
        int nearestCity = -1;
        int minDistance = numeric_limits<int>::max();

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int distance = calculateDistance(cities[lastCity], cities[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestCity = j;
                }
            }
        }

        tour.push_back(nearestCity);
        visited[nearestCity] = true;
    }

    tour.push_back(0);  // Return to the starting city
    return tour;
}

int main() {
    // Read input from file
    ifstream inputFile("inputGreedyTSP.txt");
    int n;
    inputFile >> n;

    vector<pair<int, int>> cities(n);
    for (int i = 0; i < n; i++) {
        inputFile >> cities[i].first >> cities[i].second;
    }
    inputFile.close();

    // Solve TSP using greedy algorithm
    vector<int> tour = greedyTSP(cities);

    // Calculate total distance
    int totalDistance = 0;
    for (int i = 0; i < n; i++) {
        totalDistance += calculateDistance(cities[tour[i]], cities[tour[i+1]]);
    }

    // Write output to file
    ofstream outputFile("outputGreedyTSP.txt");
    outputFile << "Tour: ";
    for (int city : tour) {
        outputFile << city << " ";
    }
    outputFile << endl;
    outputFile << "Total distance: " << totalDistance << endl;
    outputFile.close();

    cout << "TSP solution has been written to output.txt" << endl;

    return 0;
}