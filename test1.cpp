#include <iostream>
#include <stdio.h>

// Generate a rows x columns character
char** generateMaze(int row, int col)
{
    char** maze;
    int r, c;

    //Dynamically allocating row space in heap
    maze = new char*[row];
    //Dynamically allocating column space in heap
    for(r=0; r<row; r++){
        maze[r] = new char[col];
    }

    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            maze[r][c] = '@';
        }
    }
    return maze;
}

void displayMaze(char** a, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            std::cout << a[i][j];
        }
        std::cout << std::endl;
    }
}

// "Carve" a path through the maze by recursion
void generateMazePath()
{

}

// Main Loop
int main()
{
    int mazeRows;
    int mazeColumns;
    char** maze;

    std::cin >> mazeRows;
    std::cin >> mazeColumns;

    maze = generateMaze(mazeRows, mazeColumns);
    displayMaze(maze, mazeRows, mazeColumns);

    return 0;
}
