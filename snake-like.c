//Original game developed to prove i can do it
//LEGACY
//The new one is snake-ncurses which is written more sensely

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define MOVE_UP 119    //'w'
#define MOVE_DOWN 115  //'s'
#define MOVE_LEFT 97   //'a'
#define MOVE_RIGHT 100 //'d'
#define SPEED 500      // msq

int random_n(int max)
{
    return rand() % max;
}

int snake(const size_t game_lengght, const size_t matrix_cube_size)
{
    srand(time(NULL));

    // matrix initiation
    int matrix[matrix_cube_size][matrix_cube_size] = {};
    // tresure generation
    int t_pos[2] = {0, 0};
    t_pos[0] = random_n(matrix_cube_size);
    t_pos[1] = random_n(matrix_cube_size);
    matrix[t_pos[0]][t_pos[1]] = 9;
    // start position of snake
    int pos[2] = {0, 0};
    matrix[pos[0]][pos[1]] = 1;
    // start paramaters initiation
    int pos_changes = 0;
    int snake_size = 0;
    int move = MOVE_DOWN;
    int forbidden_move = 1;
    // postion barrier matrix
    const int barrier = 4;
    int matrix_barriers[barrier][2] = {};
    for (size_t i = 0; i < barrier; i++)
    {
        matrix_barriers[i][0] = random_n(matrix_cube_size);
        matrix_barriers[i][1] = random_n(matrix_cube_size);
        matrix[matrix_barriers[i][0]][matrix_barriers[i][1]] = 8;
        switch (random_n(2))
        {
        case 1:
            matrix[matrix_barriers[i][0]+random_n(3)][matrix_barriers[i][1]] = 8;
            break;
        
        case 0:
        matrix[matrix_barriers[i][0]][matrix_barriers[i][1]+random_n(3)] = 8;
            break;
        }

    }

    // position history matrix
    const int max_pos_changes = 4 * 1024;
    int matrix_position_history[max_pos_changes][2] = {};

    // ncourses initiation
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();

    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);

    int start_y = (screen_height - matrix_cube_size) / 2;
    int start_x = (screen_width - matrix_cube_size * 2) / 2;
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);

    // game logic
    while (snake_size < game_lengght)

    {
        mvprintw(start_y - 4, start_x - 2, "Game size: %ld", matrix_cube_size);
        mvprintw(start_y - 3, start_x - 2, "Time: %d", pos_changes * SPEED / 1000);
        mvprintw(start_y - 2, start_x - 2, "Score: %d", snake_size);
        mvprintw(start_y - 1, start_x - 2, "Objective: %ld", game_lengght);

        for (int i = 0; i < matrix_cube_size; i++)
        {
            for (int j = 0; j < matrix_cube_size; j++)
            {
                char print;
                switch (matrix[i][j])
                {
                case 1:
                    print = '*';
                    attron(COLOR_PAIR(3));
                    mvprintw(start_y + i, start_x + j * 2, "%c", print);
                    attroff(COLOR_PAIR(3));
                    break;
                case 9:
                    print = '*';
                    attron(COLOR_PAIR(2));
                    mvprintw(start_y + i, start_x + j * 2, "%c", print);
                    attroff(COLOR_PAIR(2));
                    break;
                case 0:
                    print = '*';
                    attron(COLOR_PAIR(1));
                    mvprintw(start_y + i, start_x + j * 2, "%c", print);
                    attroff(COLOR_PAIR(1));
                    break;
                case 8:
                    print = '*';
                    attron(COLOR_PAIR(4));
                    mvprintw(start_y + i, start_x + j * 2, "%c", print);
                    attroff(COLOR_PAIR(4));
                    break;
                }
            }
        }

        refresh();
        usleep(SPEED * 1000);
        int new_move = getch();
        if (new_move == MOVE_UP || new_move == MOVE_DOWN || new_move == MOVE_LEFT || new_move == MOVE_RIGHT)
        {
            if (new_move != forbidden_move) // prevent reversing into itself
            {
                move = new_move;
            }
        }

        switch (move)
        {
        case MOVE_DOWN:
            matrix_position_history[pos_changes][0] = pos[0];
            matrix_position_history[pos_changes][1] = pos[1];
            matrix[matrix_position_history[pos_changes - snake_size][0]][matrix_position_history[pos_changes - snake_size][1]] = 0;
            pos_changes++;
            pos[0] = pos[0] + 1;
            if (pos[0] > matrix_cube_size - 1)
            {
                pos[0] = pos[0] - matrix_cube_size;
            }
            forbidden_move = MOVE_UP;
            break;

        case MOVE_LEFT:
            matrix_position_history[pos_changes][0] = pos[0];
            matrix_position_history[pos_changes][1] = pos[1];
            matrix[matrix_position_history[pos_changes - snake_size][0]][matrix_position_history[pos_changes - snake_size][1]] = 0;
            pos_changes++;
            pos[1] = pos[1] - 1;
            if (pos[1] > matrix_cube_size - 1)
            {
                pos[1] = pos[1] + matrix_cube_size;
            }
            forbidden_move = MOVE_RIGHT;
            break;

        case MOVE_RIGHT:
            matrix_position_history[pos_changes][0] = pos[0];
            matrix_position_history[pos_changes][1] = pos[1];
            matrix[matrix_position_history[pos_changes - snake_size][0]][matrix_position_history[pos_changes - snake_size][1]] = 0;
            pos_changes++;
            pos[1] = pos[1] + 1;
            if (pos[1] > matrix_cube_size - 1)
            {
                pos[1] = pos[1] - matrix_cube_size;
            }
            forbidden_move = MOVE_LEFT;
            break;

        case MOVE_UP:
            matrix_position_history[pos_changes][0] = pos[0];
            matrix_position_history[pos_changes][1] = pos[1];
            matrix[matrix_position_history[pos_changes - snake_size][0]][matrix_position_history[pos_changes - snake_size][1]] = 0;
            pos_changes++;
            pos[0] = pos[0] - 1;
            if (pos[0] > matrix_cube_size - 1)
            {
                pos[0] = pos[0] + matrix_cube_size;
            }
            forbidden_move = MOVE_DOWN;
            break;

        default:
            mvprintw(screen_height - 2, 0, "Invalid move");
            break;
        }

        move(screen_height - 2, 0);
        clrtoeol();

        if (matrix[pos[0]][pos[1]] == 1)
        {
            mvprintw(screen_height - 2, 0, "Game Over! Final Score: %d", snake_size);
            refresh();
            getch();
            move(screen_height - 2, 0);
            clrtoeol();
            endwin();
            return -1;
        }

        // move snake forward

        // if he eats hits barrier
        if (matrix[pos[0]][pos[1]] == 8)
        {

            goto endgame;

        }
        matrix[pos[0]][pos[1]] = 1;
        // if he eats a reward
        if (matrix[t_pos[0]][t_pos[1]] != 9)
        {
            snake_size++;
            // generate a new reward on free place
            while (matrix[t_pos[0]][t_pos[1]] != 0)
            {
                t_pos[0] = random_n(matrix_cube_size);
                t_pos[1] = random_n(matrix_cube_size);
            }

            matrix[t_pos[0]][t_pos[1]] = 9;
        }
        matrix[t_pos[0]][t_pos[1]] = 9;

        refresh();
    }
    endgame:
    clear();
    refresh();
    endwin();
    return pos_changes * SPEED / 1000;
}

int main()
{

    int result = snake(63, 16);
    if (result != -1)
    {
        printf("You finished the level in %d seconds.\n", result);
    }
    else
    {
        printf("You failed.");
        getchar();
        return -1;
    }

    printf("press any key to continue...\n");
    getchar();

    /*     result = snake(64, 8)
        if (result != -1)
        {
            printf("You finished the level in %d seconds.\n", result);
        }
        else
        {
            printf("You failed.");
            getchar();
            return -1;
        } */

    getchar();
    return 0;
}
