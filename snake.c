#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <ncurses.h>
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
};

size_t random_n(size_t max)
{
    return rand() % max;
}
size_t request_move(struct snake *snake)
{
    printf("your move:...\n");
    size_t move;
    scanf("%ld", &move);
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
    for (size_t i = 0; i < snake->matrix_size; i++)
    {
        for (size_t j = 0; j < snake->matrix_size; j++)
        {
            printf("%ld", snake->arr[i][j]);
        }
        printf("\n");
    }
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

    printf("Snake Lenght:%ld, Target:%ld\n", snake->size,snake->game_lenght);

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
    render_array(snake);
    printf("\n");
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
    }
    deinitiate_game(snake);
    printf("You finished game in %ld moves. \n",snake->pos_history);
}

int main()
{
    struct snake snake;
    snake_game(&snake);

    return 0;
}
