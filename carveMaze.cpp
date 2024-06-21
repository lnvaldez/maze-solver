#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

// Directions arrays for moving up, down, left, right
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

// Function to shuffle the directions randomly
void shuffleDirections(std::vector<int>& directions) {
    std::random_shuffle(directions.begin(), directions.end());
}

// Check if the current position is within the bounds of the maze and is a wall
bool isSafe(int x, int y, int rows, int cols, const std::vector<std::vector<char>>& maze) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == '#');
}

// Function to print the maze
void printMaze(const std::vector<std::vector<char>>& maze) {
    // Move cursor to the top-left corner of the console
    printf("\033[H");
    for (const auto& row : maze) {
        for (char cell : row) {
            printf("%c", cell);
        }
        printf("\n");
    }
    fflush(stdout);  // Ensure the output is flushed to the console
}

// Carve the maze using DFS
void carveMaze(std::vector<std::vector<char>>& maze, int x, int y, int rows, int cols) {
    maze[x][y] = ' ';
    printMaze(maze);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Delay for visualization

    std::vector<int> directions = {0, 1, 2, 3};
    shuffleDirections(directions);

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (isSafe(nx, ny, rows, cols, maze)) {
            maze[x + dx[directions[i]]][y + dy[directions[i]]] = ' ';
            carveMaze(maze, nx, ny, rows, cols);
        }
    }
}

int main() {
    int rows, cols;

    std::cout << "Enter amount of rows: ";
    std::cin >> rows;
    std::cout << "Enter amount of columns: ";
    std::cin >> cols;

    // Ensure rows and cols are odd numbers for the maze to work properly
    if (rows % 2 == 0) rows++;
    if (cols % 2 == 0) cols++;

    std::vector<std::vector<char>> maze(rows, std::vector<char>(cols, '#'));

    std::srand(std::time(0)); // Seed for randomness

    // Start carving from (1, 1)
    carveMaze(maze, 1, 1, rows, cols);

    maze[1][0] = ' ';
    maze[rows - 2][cols - 1] = 'E';

    printMaze(maze);

    return 0;
}
