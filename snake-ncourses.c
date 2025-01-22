#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define MOVE_UP 8    //'w'
#define MOVE_DOWN 2  //'s'
#define MOVE_LEFT 4  //'a'
#define MOVE_RIGHT 6 //'d'
#define SPEED 500    // ms

struct snake
{
    size_t matrix_size;
    size_t **arr;
    size_t **pos_history_matrix;
    size_t pos_history;
    size_t pos[2];
    size_t treasure_pos[2];
    size_t forbidden_move;
    size_t size;
    size_t game_lenght;
    size_t start_x;
    size_t start_y;
};

size_t random_n(size_t max)
{
    return rand() % max;
}
size_t request_move()
{
    usleep(SPEED * 1000);
    size_t new_move = getch();
    size_t move;
    if (new_move == MOVE_UP || new_move == MOVE_DOWN || new_move == MOVE_LEFT || new_move == MOVE_RIGHT)
    {
        if (new_move != 0) // prevent reversing into itself
        {
            move = new_move;
        }
    }
    return move;
}

void create_cubic_matrix(struct snake *snake)
{
    snake->arr = (size_t **)malloc(sizeof(size_t *) * snake->matrix_size);
    for (size_t i = 0; i < snake->matrix_size; i++)
    {
        snake->arr[i] = (size_t *)malloc(sizeof(size_t) * snake->matrix_size);
    }
}

void create_pos_history_matrix(struct snake *snake)
{
    snake->pos_history_matrix = (size_t **)malloc(sizeof(size_t *) * 4096);
    for (size_t i = 0; i < 4096; i++)
    {
        snake->pos_history_matrix[i] = (size_t *)malloc(sizeof(size_t) * 2);
    }
}

void set_pos(struct snake *snake)
{
    snake->arr[snake->pos_history_matrix[snake->pos_history - snake->size][0]][snake->pos_history_matrix[snake->pos_history - snake->size][1]] = 0;
    snake->arr[snake->pos[0]][snake->pos[1]] = 1;
}

void set_treasue_pos(struct snake *snake)
{
    size_t *new_treasure_pos = malloc(sizeof(size_t) * 2);
    new_treasure_pos[0] = random_n(snake->matrix_size);
    new_treasure_pos[1] = random_n(snake->matrix_size);
    snake->arr[new_treasure_pos[0]][new_treasure_pos[1]] = 9;
    free(new_treasure_pos);
    new_treasure_pos = NULL;
}

void render_array(struct snake *snake)
{
    mvprintw(snake->start_y - 4, snake->start_y - 2, "Game size: %ld", snake->matrix_size);
    mvprintw(snake->start_y - 3, snake->start_y - 2, "Time: %ld", snake->pos_history * SPEED / 1000);
    mvprintw(snake->start_y - 2, snake->start_y - 2, "Score: %ld", snake->size);
    mvprintw(snake->start_y - 1, snake->start_y - 2, "Objective: %ld", snake->game_lenght);

    for (int i = 0; i < snake->matrix_size; i++)
    {
        for (int j = 0; j < snake->matrix_size; j++)
        {
            char print;
            switch (snake->arr[i][j])
            {
            case 1:
                print = '*';
                attron(COLOR_PAIR(3));
                mvprintw(snake->start_y + i, snake->start_y + j * 2, "%c", print);
                attroff(COLOR_PAIR(3));
                break;
            case 9:
                print = '*';
                attron(COLOR_PAIR(2));
                mvprintw(snake->start_y + i, snake->start_y + j * 2, "%c", print);
                attroff(COLOR_PAIR(2));
                break;
            case 0:
                print = '*';
                attron(COLOR_PAIR(1));
                mvprintw(snake->start_y + i, snake->start_y + j * 2, "%c", print);
                attroff(COLOR_PAIR(1));
                break;
            case 8:
                print = '*';
                attron(COLOR_PAIR(4));
                mvprintw(snake->start_y + i, snake->start_y + j * 2, "%c", print);
                attroff(COLOR_PAIR(4));
                break;
            }
        }
    }

    refresh();
}

void gui_init(struct snake *snake)
{
    // ncourses initiation
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();

    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);

    snake->start_y = (screen_height - snake->matrix_size) / 2;
    snake->start_x = (screen_width - snake->matrix_size * 2) / 2;
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
}

void move_snake(struct snake *snake, size_t move)
{

    switch (move)
    {
    case MOVE_DOWN:

        snake->pos[0] = snake->pos[0] + 1;
        if (snake->pos[0] > snake->matrix_size - 1)
        {
            snake->pos[0] = snake->pos[0] - snake->matrix_size;
        }
        snake->forbidden_move = MOVE_UP;
        break;

    case MOVE_LEFT:
        snake->pos[1] = snake->pos[1] - 1;
        if (snake->pos[1] > snake->matrix_size - 1)
        {
            snake->pos[1] = snake->pos[1] + snake->matrix_size;
        }
        snake->forbidden_move = MOVE_RIGHT;
        break;

    case MOVE_RIGHT:
        snake->pos[1] = snake->pos[1] + 1;
        if (snake->pos[1] > snake->matrix_size - 1)
        {
            snake->pos[1] = snake->pos[1] - snake->matrix_size;
        }
        snake->forbidden_move = MOVE_LEFT;
        break;

    case MOVE_UP:
        snake->pos[0] = snake->pos[0] - 1;
        if (snake->pos[0] > snake->matrix_size - 1)
        {
            snake->pos[0] = snake->pos[0] + snake->matrix_size;
        }
        snake->forbidden_move = MOVE_DOWN;

        break;
    }

    snake->pos_history_matrix[snake->pos_history][0] = snake->pos[0];
    snake->pos_history_matrix[snake->pos_history][1] = snake->pos[1];
    if (snake->arr[snake->pos[0]][snake->pos[1]] == 9)
    {
        snake->size++;
        set_treasue_pos(snake);
    }

    //   printf("Snake Lenght:%ld, Target:%ld\n", snake->size, snake->game_lenght);

    set_pos(snake);
    render_array(snake);
    snake->pos_history++;
}

void initiate_game(struct snake *snake)
{

    snake->matrix_size = 8;
    create_cubic_matrix(snake);
    create_pos_history_matrix(snake);
    snake->pos[0] = 0;
    snake->pos[1] = 0;
    snake->pos_history = 1;
    snake->size = 1;
    snake->game_lenght = 4;
    set_pos(snake);
    set_treasue_pos(snake);
    //  printf("\n");
    gui_init(snake);
}

void deinitiate_game(struct snake *snake)
{
    free(snake->arr);
    snake->arr = NULL;
    free(snake->pos_history_matrix);
    snake->pos_history_matrix = NULL;
}

void snake_game(struct snake *snake)
{
    initiate_game(snake);
    while (snake->size < snake->game_lenght)
    {

        move_snake(snake, request_move(snake));
        render_array(snake);
    }
    deinitiate_game(snake);
    // f  printf("You finished game in %ld moves. \n", snake->pos_history);
}

int main()
{
    struct snake snake;
    snake_game(&snake);

    return 0;
}
