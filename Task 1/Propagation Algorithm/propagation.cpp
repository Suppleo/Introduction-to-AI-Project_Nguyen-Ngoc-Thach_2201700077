#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Function to perform the propagation
std::vector<double> propagate(const std::vector<double>& input, double decay_factor) {
    std::vector<double> output(input.size());
    
    // Forward propagation
    output[0] = input[0];
    for (size_t i = 1; i < input.size(); ++i) {
        output[i] = input[i] + decay_factor * output[i-1];
    }
    
    // Backward propagation
    for (size_t i = input.size() - 2; i > 0; --i) {
        output[i] += decay_factor * output[i+1];
    }
    
    return output;
}

int main() {
    // Open input file
    std::ifstream inFile("inputPropagation.txt");
    if (!inFile) {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }

    // Read input data
    std::vector<double> input;
    double value;
    while (inFile >> value) {
        input.push_back(value);
    }
    inFile.close();

    // Perform propagation
    double decay_factor = 0.5; // You can adjust this value
    std::vector<double> result = propagate(input, decay_factor);

    // Open output file
    std::ofstream outFile("outputPropagation.txt");
    if (!outFile) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    // Write results to output file
    for (const auto& val : result) {
        outFile << val << std::endl;
    }
    outFile.close();

    std::cout << "Propagation complete. Results written to output.txt" << std::endl;

    return 0;
}