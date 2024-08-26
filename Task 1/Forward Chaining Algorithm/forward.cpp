#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string> 

using namespace std;

// Structure to represent a rule
struct Rule {
    vector<string> conditions;
    string conclusion;
};

// Function to perform forward chaining
void forwardChaining(const vector<string>& facts, const vector<Rule>& rules, vector<string>& newFacts) {
    unordered_set<string> knownFacts(facts.begin(), facts.end());
    bool newFactAdded;

    do {
        newFactAdded = false;
        for (const Rule& rule : rules) {
            // Check if all conditions of the rule are satisfied
            if (all_of(rule.conditions.begin(), rule.conditions.end(),
                       [&knownFacts](const string& condition) {
                           return knownFacts.find(condition) != knownFacts.end();
                       })) {
                // If the conclusion is not already known, add it to the known facts
                if (knownFacts.find(rule.conclusion) == knownFacts.end()) {
                    knownFacts.insert(rule.conclusion);
                    newFacts.push_back(rule.conclusion);
                    newFactAdded = true;
                }
            }
        }
    } while (newFactAdded);
}

int main() {
    // Read input from file
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    vector<string> initialFacts;
    vector<Rule> rules;

    // Read initial facts
    string line;
    getline(inputFile, line);
    while (line != "RULES") {
        initialFacts.push_back(line);
        getline(inputFile, line);
    }

    // Read rules
    while (getline(inputFile, line)) {
        Rule rule;
        size_t arrowPos = line.find("->");
        string conditions = line.substr(0, arrowPos);
        rule.conclusion = line.substr(arrowPos + 2);

        size_t pos = 0;
        while ((pos = conditions.find(',')) != string::npos) {
            rule.conditions.push_back(conditions.substr(0, pos));
            conditions.erase(0, pos + 1);
        }
        rule.conditions.push_back(conditions);

        rules.push_back(rule);
    }

    inputFile.close();

    // Perform forward chaining
    vector<string> newFacts;
    forwardChaining(initialFacts, rules, newFacts);

    // Write output to file
    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    outputFile << "Initial facts:" << endl;
    for (const string& fact : initialFacts) {
        outputFile << fact << endl;
    }

    outputFile << "\nNew facts derived:" << endl;
    for (const string& fact : newFacts) {
        outputFile << fact << endl;
    }

    outputFile.close();

    cout << "Forward chaining completed. Results written to output.txt" << endl;

    return 0;
}