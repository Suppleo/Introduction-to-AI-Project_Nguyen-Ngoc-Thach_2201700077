#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm> 

using namespace std;

// Structure to represent a node in the grid
struct Node {
    int x, y;
    int f, g, h;
    Node* parent;

    Node(int x, int y) : x(x), y(y), f(0), g(0), h(0), parent(nullptr) {}
};

// Compare function for priority queue
struct CompareNode {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->f > rhs->f;
    }
};

// Function to calculate heuristic (Manhattan distance)
int heuristic(const Node* a, const Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

// A* algorithm implementation
vector<Node*> astar(vector<vector<int>>& grid, Node* start, Node* goal) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Create open and closed lists
    priority_queue<Node*, vector<Node*>, CompareNode> openList;
    vector<vector<bool>> closedList(rows, vector<bool>(cols, false));

    // Add start node to open list
    start->f = start->g + heuristic(start, goal);
    openList.push(start);

    // Main loop
    while (!openList.empty()) {
        // Get the node with the lowest f score
        Node* current = openList.top();
        openList.pop();

        // Check if we've reached the goal
        if (current->x == goal->x && current->y == goal->y) {
            // Reconstruct path
            vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Mark current node as closed
        closedList[current->y][current->x] = true;

        // Check neighbors
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                int newX = current->x + dx;
                int newY = current->y + dy;

                // Check if the neighbor is valid
                if (newX >= 0 && newX < cols && newY >= 0 && newY < rows && grid[newY][newX] == 0 && !closedList[newY][newX]) {
                    Node* neighbor = new Node(newX, newY);
                    neighbor->g = current->g + 1;
                    neighbor->h = heuristic(neighbor, goal);
                    neighbor->f = neighbor->g + neighbor->h;
                    neighbor->parent = current;

                    openList.push(neighbor);
                }
            }
        }
    }

    // No path found
    return {};
}

int main() {
    // Read input file
    ifstream inputFile("inputAstar.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    int rows, cols;
    inputFile >> rows >> cols;

    vector<vector<int>> grid(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            inputFile >> grid[i][j];
        }
    }

    int startX, startY, goalX, goalY;
    inputFile >> startX >> startY >> goalX >> goalY;

    inputFile.close();

    // Create start and goal nodes
    Node* start = new Node(startX, startY);
    Node* goal = new Node(goalX, goalY);

    // Run A* algorithm
    vector<Node*> path = astar(grid, start, goal);

    // Write output file
    ofstream outputFile("outputAstar.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    if (path.empty()) {
        outputFile << "No path found" << endl;
    } else {
        outputFile << "Path found:" << endl;
        for (const auto& node : path) {
            outputFile << node->x << " " << node->y << endl;
        }
    }

    outputFile.close();

    // Clean up memory
    for (const auto& node : path) {
        delete node;
    }
    delete start;
    delete goal;

    cout << "A* algorithm completed. Check output.txt for results." << endl;

    return 0;
}