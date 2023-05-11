#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <random>

using namespace std;

int c = 2;
int b = 6;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(c, b);
int a = dis(gen);

bool gameOver;
int width = 20;
int height = 20;
int x, y, score;
int tailX[100], tailY[100];
int nTail;
int maxFruits = a; // Maximum number of fruits on the field
int maxObstacles = a + 3; // Maximum number of obstacles on the field
vector<int> fruitX(maxFruits);
vector<int> fruitY(maxFruits);
vector<pair<int, int>> obstacles;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
eDirection prevDir;

void player()
{
    cout << "if height >= 20 end width >= 20 Obstacles will be added to the field." << endl;

    cout << "Enter the height of the game board: " << endl;
    cin >> height;
    cout << "Enter the width of the game board: " << endl;
    cin >> width;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    nTail = 0;

    for (int i = 0; i < maxFruits; i++) {
        fruitX[i] = rand() % width;
        fruitY[i] = rand() % height;
    }
    if (height >= 20 && width >= 20) {
        for (int i = 0; i < maxObstacles; i++) {
            int obstacleX = rand() % width;
            int obstacleY = rand() % height;
            obstacles.push_back(make_pair(obstacleX, obstacleY));
        }
    }
}

void Draw() {
    system("cls"); // Clear the console

    // display the upper border of the field
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Print the side borders of the field and game elements
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "O";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                bool isFruit = false;
                for (int k = 0; k < maxFruits; k++) {
                    if (fruitX[k] == j && fruitY[k] == i) {
                        cout << "A";
                        isFruit = true;
                    }
                }
                if (height >= 20 && width >= 20) {
                    bool isObstacle = false;
                    for (auto obstacle : obstacles) {
                        if (obstacle.first == j && obstacle.second == i) {
                            cout << "#";
                            isObstacle = true;
                        }
                    }
                    if (!isObstacle && !print && !isFruit)
                        cout << " ";
                }
                else {
                    if (!print && !isFruit)
                        cout << " ";
                }
            }
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    // Print the bottom border of the field and the score
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (prevDir != RIGHT) // Disallow reversing direction instantly
                dir = LEFT;
            break;
        case 'd':
            if (prevDir != LEFT) // Disallow reversing direction instantly
                dir = RIGHT;
            break;
        case 'w':
            if (prevDir != DOWN) // Disallow reversing direction instantly
                dir = UP;
            break;
        case 's':
            if (prevDir != UP) // Disallow reversing direction instantly
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic() {
    // Save the previous position of the tail
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    prevDir = dir;
    // Move the snake according to the direction
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Check for collision with the boundaries of the field
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    // Check for collision with the tail
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Check for fruit consumption and add a new tail
    for (int i = 0; i < maxFruits; i++) {
        if (x == fruitX[i] && y == fruitY[i]) {
            score += 100;
            fruitX[i] = rand() % width;
            fruitY[i] = rand() % height;
            nTail++;
        }
    }

    // Check for collision with obstacles
    if (height >= 20 && width >= 20) {
        for (auto obstacle : obstacles) {
            if (x == obstacle.first && y == obstacle.second)
                gameOver = true;
        }
    }
}

void Restart() {
    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        Setup();
        player();
        gameOver = false;
    }
    else {
        gameOver = true;
    }
}

int main() {
    srand(time(NULL));
    Setup();
    player();
    do {
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(50); // Slow down the movement of the snake
        }
        Restart();
    } while (!gameOver);

    return 0;
}
