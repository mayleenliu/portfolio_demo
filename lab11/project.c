// Mayleen Liu
// Fundamentals of Computing
// project.c
// Final project arcade game main driver with snake and fireworks

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "gfx.h"
#include "arcadefunc.h"

int main()
{
    int difficulty, scale, wid = 1000, ht = 900, tail[757][2], board_max = 750, board_min = 250, margin = 50, msecond_count = 0;
    double gravity = 1;
    char ch, color, c;
    srand(time(0));

    display_choices();

    scanf("%d %c %c", &difficulty, &ch, &color);

    scale = user_preferences(difficulty);

    int r = scale / 2;

    Object snake[100], food, particle[100];
    Firework dot[100];

    snake[0].x = 400;
    snake[0].y = 300;
    snake[0].vx = 0;
    snake[0].vy = 0;
    snake[0].total_length = 0;

    reset_coordinates_fireworks(particle, dot, gravity, msecond_count);

    gfx_open(wid, ht, "Arcade");

    food.x = (board_min + margin) + rand() % (board_max - margin + 1 - board_min - margin);
    food.y = (board_min + margin) + rand() % (board_max - board_min - margin + 1 - board_min - margin);
    int count = 1;
    while (1)
    {
        initialize(board_min, board_max, scale, wid, ht, margin);
        display_dot(dot, gravity, particle);
        display_particles(particle, gravity);
        msecond_count++;

        if (eat(food, snake, r))
        {
            food.x = (board_min + margin) + rand() % (board_max - margin + 1 - board_min - margin);
            food.y = (board_min + margin) + rand() % (board_max - board_min - margin + 1 - board_min - margin);
            count++;
        }

        if (msecond_count == 30)
        {
            msecond_count = reset_coordinates_fireworks(particle, dot, gravity, msecond_count);
            display_dot(dot, gravity, particle);
            display_particles(particle, gravity);
        }

        death(snake, tail, wid, ht);
        update(snake, tail, scale);
        draw_snake(snake, tail, scale, ch, color);
        draw_food(food, r);
        gfx_flush();

        check_boundaries(snake, wid, ht, tail, scale, board_min, board_max, margin);

        if (gfx_event_waiting())
        {
            c = gfx_wait();
            int x = gfx_xpos();
            int y = gfx_ypos();
            keyPressed(snake, c, x, y);
        }
        usleep(100000);
        gfx_clear();
    }
    return 0;
}