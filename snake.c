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
    size_t pos[2];
    size_t treasure_pos[2];
    size_t forbidden_move;
    size_t size;
    size_t lenght;
    size_t delete[2];
};

size_t random_n(size_t max)
{
    return rand() % max;
}

size_t **create_cubic_matrix(size_t **arr, size_t matrix_size)
{
    arr = (size_t **)malloc(sizeof(size_t *) * matrix_size);
    for (size_t i = 0; i < matrix_size; i++)
    {
        arr[i] = (size_t *)malloc(sizeof(size_t) * matrix_size);
    }

    return arr;
}

void set_pos(struct snake snake)
{

    snake.arr[snake.pos[0]][snake.pos[1]] = 1;
}

void set_treasue_pos(size_t **arr, size_t treasure_pos[2])
{
    arr[treasure_pos[0]][treasure_pos[1]] = 9;
}

void render_array(struct snake snake)
{
    for (size_t i = 0; i < snake.matrix_size; i++)
    {
        for (size_t j = 0; j < snake.matrix_size; j++)
        {
            printf("%ld", snake.arr[i][j]);
        }
        printf("\n");
    }
}
void move_snake(struct snake *snake, size_t move)
{

        if (snake->size > snake->lenght)
        {
        snake->delete[0]=snake->pos[0] ;  
        snake->delete[1]=snake->pos[1] ;
        snake->size--;
        }
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

    snake->size++;
    printf("[%d][%d]\n", snake->pos[0], snake->pos[1]);
        snake->arr[snake->delete[0]][snake->delete[1]] = 0;

    

    set_pos(*snake);
    render_array(*snake);
}

size_t request_move(struct snake *snake)
{
    printf("your move:...\n");
        size_t move;
    scanf("%ld", &move);
  //  getchar();
    printf("{%ld}", move);
    return move;
}

void snake_game(struct snake *snake)
{

    render_array(*snake);
    printf("\n");
    while (1)
    {

        move_snake(snake, request_move(snake));
    }
}

int main()
{
    struct snake s1;

    s1.matrix_size = 8;
    s1.arr = create_cubic_matrix(s1.arr, s1.matrix_size);
    s1.pos[0] = 0;
    s1.pos[1] = 2;
    s1.lenght = 3;
    set_pos(s1);
    snake_game(&s1);

    return 0;
}
