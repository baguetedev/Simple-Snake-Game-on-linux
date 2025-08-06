#include <iostream>

#include <cstdlib>

#include <ctime>

#include <unistd.h>

#include <termios.h>

using namespace std;

// Players name
string Name;

// Boundaries
const int width = 100;
const int height = 40;

// Snake head cords
int snake_x, snake_y;

// Food cords
int food_x, food_y;

// Score
int score;

// Arrays of cords of the snake tail
int snake_tailx[100], snake_taily[100];

// Length of the snake tail
int Tail_Length;

// Storing snake's direction
enum snakeDirection {
  STOP = 0, left, right, up, down
};
snakeDirection sdirection = snakeDirection::STOP; // Initialize to STOP

// Bool to check if game is over
bool isGameOver;

// Function to handle key input (non-blocking on Linux)
char getInput() {
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, & oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
  tcsetattr(STDIN_FILENO, TCSANOW, & newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
  return ch;
}

// Initialize the game
void GameStart() {
  isGameOver = false;
  sdirection = snakeDirection::STOP;
  snake_x = width / 2;
  snake_y = height / 2;
  food_x = rand() % width;
  food_y = rand() % height;
  score = 0;
  Tail_Length = 0;
}

// Render the game board
void Render() {
  system("clear"); // Clears the screen to refresh the game

  // Top borders
  for (int i = 0; i < width; i++) {
    cout << "-";
  }
  cout << endl;

  // Main game area
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // Walls
      if (j == 0 || j == width - 1)
        cout << "|";
      // Snake's head
      else if (i == snake_y && j == snake_x)
        cout << "(0)";
      // Snake's food
      else if (i == food_y && j == food_x)
        cout << "@";
      else {
        bool Tail = false;
        for (int k = 0; k < Tail_Length; k++) {
          if (snake_tailx[k] == j && snake_taily[k] == i) {
            cout << "0";
            Tail = true;
          }
        }
        if (!Tail)
          cout << " ";
      }
    }
    cout << endl;
  }

  // Bottom borders
  for (int i = 0; i < width; i++)
    cout << "-";

  cout << endl;

  // Score
  cout << Name << "'s Score: " << score << endl;
}

// Update the game state
void Update() {
  int previousX = snake_tailx[0];
  int previousY = snake_taily[0];
  int previous2X, previous2Y;
  snake_tailx[0] = snake_x;
  snake_taily[0] = snake_y;

  for (int i = 1; i < Tail_Length; i++) {
    previous2X = snake_tailx[i];
    previous2Y = snake_taily[i];
    snake_tailx[i] = previousX;
    snake_taily[i] = previousY;
  }

  // Move the snake based on the current direction
  switch (sdirection) {
  case snakeDirection::left:
    snake_x--;
    break;
  case snakeDirection::right:
    snake_x++;
    break;
  case snakeDirection::up:
    snake_y--;
    break;
  case snakeDirection::down:
    snake_y++;
    break;
  default:
    break;
  }

  // Check for collisions with boundaries
  if (snake_x >= width || snake_x < 0 || snake_y >= height || snake_y < 0)
    isGameOver = true;

  // Check for collisions with the snake's tail
  for (int i = 0; i < Tail_Length; i++) {
    if (snake_tailx[i] == snake_x && snake_taily[i] == snake_y)
      isGameOver = true;
  }

  // Check if the snake eats food
  if (snake_x == food_x && snake_y == food_y) {
    score += 100;
    food_x = rand() % width;
    food_y = rand() % height;
    Tail_Length++;
  }
}

// Handle user input
void Input() {
  if (isGameOver) return;

  char key = getInput();

  // Update the snake's direction based on the key pressed
  switch (key) {
  case 'a': // Left
    if (sdirection != snakeDirection::right) // Prevent going in the opposite direction
      sdirection = snakeDirection::left;
    break;
  case 'd': // Right
    if (sdirection != snakeDirection::left) // Prevent going in the opposite direction
      sdirection = snakeDirection::right;
    break;
  case 'w': // Up
    if (sdirection != snakeDirection::down) // Prevent going in the opposite direction
      sdirection = snakeDirection::up;
    break;
  case 's': // Down
    if (sdirection != snakeDirection::up) // Prevent going in the opposite direction
      sdirection = snakeDirection::down;
    break;
  case 'x': // Quit
    isGameOver = true;
    break;
  default:
    break;
  }
}

int main() {
  cout << "Enter your name: ";
  cin >> Name;

  srand(time(0)); // Initialize random seed
  GameStart();

  while (!isGameOver) {
    Render();
    Input();
    Update();
    usleep(100000); // Slow down the game loop (100ms)
  }

  cout << "Game Over! Final Score: " << score << endl;
  return 0;
}