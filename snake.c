#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define COLS 25
#define ROWS 25
#define SNAKE_MAX_LEN 256

char board[COLS * ROWS];
int isGameOver = 0;
int paused = 0;

struct SnakePart {
    int x, y;
};

struct Snake {
    int length;
    struct SnakePart part[SNAKE_MAX_LEN];
};
struct Snake snake;

struct Food {
    int x, y;
    int consumed;
};
struct Food food;

void clear_screen() {
    system("cls");
}

void fill_board() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (x == 0 || y == 0 || x == COLS - 1 || y == ROWS - 1)
                board[y * COLS + x] = '#';
            else
                board[y * COLS + x] = ' ';
        }
    }
}

void print_board() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            putch(board[y * COLS + x]);
        }
        putch('\n');
    }
}

void setup_snake() {
    snake.length = 1;
    snake.part[0].x = COLS / 2;
    snake.part[0].y = ROWS / 2;
}

void spawn_food() {
    food.x = 1 + rand() % (COLS - 2);
    food.y = 1 + rand() % (ROWS - 2);
    food.consumed = 0;
}

void draw_food() {
    if (!food.consumed) {
        board[food.y * COLS + food.x] = '+';
    }
}

void draw_snake() {
    for (int i = snake.length - 1; i > 0; i--) {
        board[snake.part[i].y * COLS + snake.part[i].x] = '*';
    }
    board[snake.part[0].y * COLS + snake.part[0].x] = '@';
}

void move_snake(int dx, int dy) {
    for (int i = snake.length - 1; i > 0; i--) {
        snake.part[i] = snake.part[i - 1];
    }
    snake.part[0].x += dx;
    snake.part[0].y += dy;
}

void check_rules() {
    if (snake.part[0].x == 0 || snake.part[0].x == COLS - 1 ||
        snake.part[0].y == 0 || snake.part[0].y == ROWS - 1) {
        isGameOver = 1;
    }

    for (int i = 1; i < snake.length; i++) {
        if (snake.part[0].x == snake.part[i].x &&
            snake.part[0].y == snake.part[i].y) {
            isGameOver = 1;
        }
    }

    if (!food.consumed && 
        snake.part[0].x == food.x && 
        snake.part[0].y == food.y) {
        food.consumed = 1;
        snake.length++;
        if (snake.length < SNAKE_MAX_LEN)
            spawn_food(); // Spawn new food
    }
}

int get_speed() {
    int base = 200;
    int speed = base - (snake.length - 1) * 5;
    if (speed < 50) speed = 50;
    return speed;
}

void read_keyboard(int *dx, int *dy) {
    if (_kbhit()) {
        int ch = _getch();
        switch (ch) {
            case 'w': if (*dy == 0 && !paused) { *dx = 0; *dy = -1; } break;
            case 's': if (*dy == 0 && !paused) { *dx = 0; *dy = 1; } break;
            case 'a': if (*dx == 0 && !paused) { *dx = -1; *dy = 0; } break;
            case 'd': if (*dx == 0 && !paused) { *dx = 1; *dy = 0; } break;
            case 'p': paused = 1; break;
            case 'c': paused = 0; break;
        }
    }
}

int main() {
    srand(time(0));
    setup_snake();
    spawn_food();

    int dx = 1, dy = 0;

    while (!isGameOver) {
        fill_board();
        draw_food();
        draw_snake();
        check_rules();
        clear_screen();
        printf("SNAKE GAME | Score: %d\n", (snake.length - 1) * 100);
        print_board();

        if (paused) {
            printf("\n[PAUSED] Press 'c' to continue...\n");
            Sleep(100);
        } else {
            read_keyboard(&dx, &dy);
            move_snake(dx, dy);
            Sleep(get_speed());
        }
        read_keyboard(&dx, &dy);
    }

    printf("\nGAME OVER! Final Score: %d\n", (snake.length - 1) * 100);
    while (!_kbhit()); // wait for user to press any key
    return 0;
}
