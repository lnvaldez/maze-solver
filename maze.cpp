#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <conio.h>

const char* RED = "\033[31m";
const char* NORMAL = "\033[0m";
const char* HIDE_CURSOR = "\033[?25l";
const char* SHOW_CURSOR = "\033[?25h";
const char* CLEAR_SCREEN = "\033[2J\033[H";

int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

void shuffleDirections(std::vector<int>& directions)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(directions.begin(), directions.end(), gen);
}

bool isValid(int x, int y, int rows, int cols, const std::vector<std::vector<char>>& maze)
{
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == '#');
}

void printMaze(const std::vector<std::vector<char>>& maze)
{
    printf("\033[H");
    for (const auto& row : maze){
        for (char cell : row) {
            if (cell == '*') {
                printf("%s%c%s", RED, cell, NORMAL);
            } else {
                printf("%c", cell);
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void carveMaze(std::vector<std::vector<char>>& maze, int x, int y, int rows, int cols)
{
    maze[x][y] = ' ';

    std::vector<int> directions = {0, 1, 2, 3};
    shuffleDirections(directions);

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (isValid(nx, ny, rows, cols, maze)) {
            maze[x + dx[directions[i]]][y + dy[directions[i]]] = ' ';
            carveMaze(maze, nx, ny, rows, cols);
        }
    }
}

void randomCarve(std::vector<std::vector<char>>& maze, int x, int y)
{
    if (maze[x][y] == '#') {
        maze[x][y] = ' ';
    }
}

bool solveMaze(std::vector<std::vector<char>>& maze, int x, int y, int rows, int cols)
{
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

void playMaze(std::vector<std::vector<char>>& maze, int rows, int cols)
{
    int x = 1, y = 0;
    maze[x][y] = '*';
    printMaze(maze);

    while (x != rows - 2 || y != cols - 1) {
        int nx = x, ny = y;
        char move = _getch();
        switch (move) {
            case 'w': nx = x + dy[0]; ny = y + dx[0]; break;
            case 'd': nx = x + dy[1]; ny = y + dx[1]; break;
            case 's': nx = x + dy[2]; ny = y + dx[2]; break;
            case 'a': nx = x + dy[3]; ny = y + dx[3]; break;
        }

        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] == ' ') {
            maze[x][y] = ' ';
            x = nx;
            y = ny;
            maze[x][y] = '*';
            printMaze(maze);
        }
    }

    std::cout << "Congratulations! You have solved the maze!" << std::endl;
}

int main()
{
    int rows, cols, choice;

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

    if (rows % 2 == 0) rows++;
    if (cols % 2 == 0) cols++;

    std::cout << "Choose an option:\n";
    std::cout << "1. Simulate\n";
    std::cout << "2. Play\n";
    std::cin >> choice;

    printf("%s", CLEAR_SCREEN);

    std::vector<std::vector<char>> maze(rows, std::vector<char>(cols, '#'));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disRow(1, rows - 2);
    std::uniform_int_distribution<> disCol(1, cols - 2);

    std::srand(std::time(0));

    carveMaze(maze, 1, 1, rows, cols);

    for (int i = 0; i < rows; ++i) {
        int random_row = disRow(gen);
        int random_col = disCol(gen);
        if (maze[random_row][random_col] == '#') {
            randomCarve(maze, random_row, random_col);
        }
    }

    maze[1][0] = ' ';
    maze[rows - 2][cols - 1] = ' ';

    printf("%s", HIDE_CURSOR);

    if (choice == 1) {
        if (!solveMaze(maze, 1, 0, rows, cols)) {
            std::cout << "No solution found!" << std::endl;
        }
    } else if (choice == 2) {
        playMaze(maze, rows, cols);
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }

    printf("%s", SHOW_CURSOR);

    return 0;
}
