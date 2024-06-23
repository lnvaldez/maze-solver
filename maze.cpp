#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <windows.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

const char* RED = "\033[31m";
const char* NORMAL = "\033[0m";
const char* HIDE_CURSOR = "\033[?25l";
const char* SHOW_CURSOR = "\033[?25h";
const char* CLEAR_SCREEN = "\033[2J\033[H";

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
            if (cell == '*') {
                printf("%s%c%s", RED, cell, NORMAL);
            } else {
                printf("%c", cell);
            }
        }
        printf("\n");
    }
    fflush(stdout);  // Ensure the output is flushed to the console
}

// Carve the maze using DFS
void carveMaze(std::vector<std::vector<char>>& maze, int x, int y, int rows, int cols) {
    maze[x][y] = ' ';

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

// Solve the maze using backtracking
bool solveMaze(std::vector<std::vector<char>>& maze, int x, int y, int rows, int cols) {
    printMaze(maze);
    if (x == rows - 2 && y == cols - 1) {
        maze[x][y] = '*';
        printMaze(maze);
        return true;
    }

    if (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == ' ') {
        maze[x][y] = '*';

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (solveMaze(maze, nx, ny, rows, cols)) {
                return true;
            }
        }

        maze[x][y] = ' ';
    }

    return false;
}

int main() {
    int rows, cols;

    std::cout << R"(
 #     #    #    ####### #######
 ##   ##   # #        #  #
 # # # #  #   #      #   #
 #  #  # #     #    #    #####
 #     # #######   #     #
 #     # #     #  #      #
 #     # #     # ####### #######
    )" << std::endl;

    std::cout << "Max dimensions = 29x119 (rows x columns)\n";
    std::cout << "*Enter amount of rows* \n";
    std::cin >> rows;
    std::cout << "*Enter amount of columns* \n";
    std::cin >> cols;

    // Ensure rows and cols are odd numbers for the maze to work properly
    if (rows % 2 == 0) rows++;
    if (cols % 2 == 0) cols++;

#ifdef _WIN32
    system("cls");
#else
    printf("%s", CLEAR_SCREEN);
#endif

    std::vector<std::vector<char>> maze(rows, std::vector<char>(cols, '#'));

    std::srand(std::time(0)); // Seed for randomness

    // Start carving from (1, 1)
    carveMaze(maze, 1, 1, rows, cols);

    // Set start point
    maze[1][0] = ' ';
    maze[rows - 2][cols - 1] = ' ';

    // Hide the cursor
    printf("%s", HIDE_CURSOR);

    // Solve the maze
    if (!solveMaze(maze, 1, 0, rows, cols)) {
        std::cout << "No solution found!" << std::endl;
    }

    // Show the cursor
    printf("%s", SHOW_CURSOR);

    return 0;
}
