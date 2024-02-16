#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

//This is the graphic library we have decided to use

// Define constants for the game
#define MAP_SIZE_X 10
#define MAP_SIZE_Y 10
#define SNAKE_INITIAL_LENGTH 3

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Define directions
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Define structures for the snake and food

//Coordinates for characters
struct Point {
    int x;
    int y;
};

struct Snake {
    struct Point body[MAP_SIZE_X * MAP_SIZE_Y]; //Array 
    int length;
    enum Direction direction;
};

struct Food {
    struct Point position;
};

// Function prototypes
void initializeGame(struct Snake *snake, struct Food *food);
void drawGame(struct Snake *snake, struct Food *food, SDL_Renderer *renderer, SDL_Texture *foodTexture);
void moveSnake(struct Snake *snake);
void checkCollision(struct Snake *snake, struct Food *food);
void generateFood(struct Food *food);
enum Direction getButtonInput();

int main() {
    struct Snake snake;
    struct Food food;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Initialize the game and SDL
    initialize_SDL(&window, &renderer);
    initializeGame(&snake, &food);

    SDL_Texture *foodTexture = IMG_LoadTexture(renderer, "starberry.png");
    if (!foodTexture) {
        printf("Failed to load food texture: %s\n", IMG_GetError());
    }

    // Game loop
    while (true) {
        // Draw the game
        drawGame(&snake, &food, renderer, foodTexture);

        // Get button input to control the snake's direction
        snake.direction = getButtonInput();

        // Move the snake
        moveSnake(&snake);

        // Check for collisions
        checkCollision(&snake, &food);

        // Generate new food if needed
        if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
            snake.length++;
            generateFood(&food);
        }

         SDL_Delay(100);
    }

    SDL_DestroyTexture(foodTexture);
    return 0;
}

void initializeGame(struct Snake *snake, struct Food *food) {
    // Initialize the snake
    snake->length = SNAKE_INITIAL_LENGTH;
    //Snake is intialized to start from far left and to go right
    snake->direction = RIGHT;

    // Initialize the snake's body from the bottom left corner
    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = i;
        snake->body[i].y = 0;
    }

    // Initialize the food
    generateFood(food);
}

void drawGame(struct Snake *snake, struct Food *food, SDL_Renderer *renderer, SDL_Texture *foodTexture) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for the snake
    for (int i = 0; i < snake->length; i++) {
        SDL_Rect rect = {snake->body[i].x * 40, snake->body[i].y * 40, 40, 40};  // Adjust the size as needed
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw the food using a texture
    SDL_Rect foodRect = {food->position.x * 40, food->position.y * 40, 40, 40};  // Adjust the size as needed
    SDL_RenderCopy(renderer, foodTexture, NULL, &foodRect);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

void moveSnake(struct Snake *snake) {
    // Move the body of the snake
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Move the head of the snake based on the direction
    switch (snake->direction) {
        case UP:
            snake->body[0].y--;
            break;
        case DOWN:
            snake->body[0].y++;
            break;
        case LEFT:
            snake->body[0].x--;
            break;
        case RIGHT:
            snake->body[0].x++;
            break;
    }
}

void checkCollision(struct Snake *snake, struct Food *food) {
    // Check if the snake collides with itself or the walls
    // Implement collision logic based on your hardware configuration
}

void generateFood(struct Food *food) {
    // Generate a new position for the food
    // You may need to ensure that the new position is not occupied by the snake
    food->position.x = rand() % MAP_SIZE_X;
    food->position.y = rand() % MAP_SIZE_Y;
}

enum Direction getButtonInput() {
    // Implement code to read the state of your FPGA push buttons
    // and return the corresponding direction
    // You might need to debounce the button inputs for better performance
}

void initialize_SDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        exit(1);  // Exit the program on initialization failure
    }

    // Create a window
    *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!*window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);  // Exit the program on window creation failure
    }

    // Create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);  // Exit the program on renderer creation failure
    }
}