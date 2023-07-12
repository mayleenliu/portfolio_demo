// Mayleen Liu
// Fundamentals of Computing
// arcadefunc.c
// Final project arcade game functions file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "gfx.h"
#include "arcadefunc.h"

int user_preferences(int difficulty) // adjust speed of snake depending on difficulty user selected
{
    int scale;
    switch (difficulty)
    {
    case 1:
        scale = 20;
        break;
    case 2:
        scale = 25;
        break;
    case 3:
        scale = 30;
        break;
    default:
        printf("invalid entry");
        exit(0);
    }
    return scale;
}

void display_choices() // display menu of choices
{
    printf("Welcome to the Arcade!\n");
    printf("Please Select Your Snake Game Desired Difficulty (speed) Level (1-3): ");
    printf("\nPlease Select Your Snake Head Shape (s, t, c): \n");
    printf("Please Select Your Snake Color (w, r, o, y, g, b, p, a): \n");
}

int reset_coordinates_fireworks(Object particle[], Firework dot[], double gravity, int msecond_count) // reset firework coordinates to bottom of display
{
    for (int i = 0; i < 50; i++)
    {
        dot[i].x_vel = 0;
        dot[i].y_vel = -20 + rand() % (-10 - -20 + 1);
        dot[i].x_acc = 0;
        dot[i].y_acc = 0;
        dot[i].x_pos = 50 + rand() % (950 - 50);
        dot[i].y_pos = 780;
    }
    for (int i = 0; i < 100; i++)
    {
        particle[i].ax = 0;
        particle[i].ay = 0;
        particle[i].vx = -10 + rand() % (10 - -10 + 1);
        particle[i].vy = -10 + rand() % (10 - -10 + 1);
        particle[i].random = 20 + rand() % (40 - 20 + 1);
    }

    msecond_count = 0;
    return msecond_count;
}

void update_dot(Firework dot[], int i) // update firework initial upward "dot" velocity
{
    dot[i].x_pos += dot[i].x_vel;
    dot[i].y_pos += dot[i].y_vel;
    dot[i].x_vel += dot[i].x_acc;
    dot[i].y_vel += dot[i].y_acc;
    dot[i].x_acc = 0;
    dot[i].y_acc = 0;
}
void update_particle(Object particle[], int i) // update firework explosion position, velocity
{
    particle[i].x += particle[i].vx * 2;
    particle[i].y += particle[i].vy * 2;
    particle[i].vx += particle[i].ax;
    particle[i].vy += particle[i].ay;
    particle[i].ax = 0;
    particle[i].ay = 0;
}
void forces(Firework dot[], double force, int i) // make initial dots feel "gravity" on way up
{
    dot[i].y_acc += force;
}

void display_dot(Firework dot[], double gravity, Object particle[]) // display rainbow firework dots
{
    gfx_color(rand() % (256), rand() % (256), rand() % (256));

    for (int i = 0; i < 50; i++)
    {
        forces(dot, gravity, i);
        update_dot(dot, i);
        if ((dot[i].y_pos > 550 && dot[i].y_pos < 770) && dot[i].y_vel < 0)
            gfx_circle(dot[i].x_pos, dot[i].y_pos, 1);

        if (dot[i].y_vel == 0)
        {
            for (int j = 0; j < 100; j++) // at the peak of the firework trajectory (velocity = 0), split into 100 particles
            {
                particle[j].x = dot[i].x_pos;
                particle[j].y = dot[i].y_pos;
            }
        }
    }
}
void display_particles(Object particle[], double force) // display firework particles
{
    gfx_color(rand() % (256), rand() % (256), rand() % (256));
    int x = particle[0].x;
    int y = particle[0].y;
    for (int i = 0; i < 100; i++)
    {
        particle[i].ay += 1;
        update_particle(particle, i);
        int d = sqrt(pow((particle[i].x - x), 2) + pow((particle[i].y - y), 2)); // make it a circle firework instead of square

        if (particle[i].y > 500 && particle[i].y < 770 && particle[i].x > 50 && particle[i].x < 950 && d < particle[i].random)
            gfx_circle(particle[i].x, particle[i].y, 1);
    }
}

void death(Object snake[], int tail[][2], int wid, int ht) // check if the snake crashes into itself
{
    double d = 0;
    for (int i = 0; i < snake[0].total_length; i++)
    {
        d = sqrt(pow((snake[0].x - tail[i][0]), 2) + pow((snake[0].y - tail[i][1]), 2));
        if (d < 1)
        {
            gfx_text(400, 300, "You crashed into yourself!");
            gfx_flush();
            usleep(1000000);
            printf("Wow, you scored %d points.\n", snake[0].total_length);
            snake[0].vx = 0;
            snake[0].vy = 0;
            for (int i = 0; i < 757; i++)
            {
                for (int j = 0; j < 2; j++)
                    tail[i][j] = 0;
            }
            snake[0].total_length = 0;
            snake[0].x = 500;
            snake[0].y = 275;
        }
    }
}

bool eat(Object food, Object snake[], int r) // check if snake touches the food or edges of the food
{
    double d = sqrt(pow((snake[0].x - food.x), 2) + pow((snake[0].y - food.y), 2));
    if (d < 2 * r)
    {
        snake[0].total_length++;
        return true;
    }
    else
        return false;
}

void check_boundaries(Object snake[], int wid, int ht, int tail[][2], int scale, int board_min, int board_max, int margin) // check if snake crashes into game boundary
{
    if (((snake[0].x) >= board_max - scale / 2) || (snake[0].x <= board_min + scale / 2) || ((snake[0].y) <= margin + scale / 2) || ((snake[0].y) >= board_max - board_min - scale / 2))
    {
        gfx_color(255, 255, 255);
        gfx_text(400, 300, "You crashed!");
        gfx_flush();
        usleep(1000000);
        printf("Wow, you scored %d points.\n", snake[0].total_length);
        snake[0].vx = 0;
        snake[0].vy = 0;

        for (int i = 0; i < 757; i++)
        {
            for (int j = 0; j < 2; j++)
                tail[i][j] = 0;
        }
        snake[0].total_length = 0;
        snake[0].x = 500;
        snake[0].y = 275;
    }
}

void draw_food(Object food, int r) // draw a rainbow food
{
    gfx_color(rand() % (256), rand() % (256), rand() % (256));
    gfx_circle(food.x, food.y, r);
}

void keyPressed(Object snake[], char c, int x, int y) // user controls snake direction
{
    if (c == 'q') // quit
        exit(0);
    if (c == 'Q' || c == 'a')
    { // left arrow
        snake[0].vx = -1;
        snake[0].vy = 0;
    }
    if (c == 'T' || c == 's')
    { // up arrow
        snake[0].vy = 1;
        snake[0].vx = 0;
    }
    if (c == 'S' || c == 'd')
    { // right arrow
        snake[0].vx = 1;
        snake[0].vy = 0;
    }
    if (c == 'R' || c == 'w')
    { // down arrow
        snake[0].vy = -1;
        snake[0].vx = 0;
    }
}

void draw_snake(Object snake[], int tail[][2], int scale, char ch, char color) // draws the snake, different shapes and colors
{
    switch (color)
    {
    case 'w':
        gfx_color(255, 255, 255);
        break;
    case 'r':
        gfx_color(255, 0, 0);
        break;
    case 'o':
        gfx_color(255, 100, 0);
        break;
    case 'y':
        gfx_color(255, 225, 0);
        break;
    case 'g':
        gfx_color(0, 255, 50);
        break;
    case 'b':
        gfx_color(0, 0, 255);
        break;
    case 'p':
        gfx_color(150, 0, 255);
        break;
    case 'a':
        gfx_color(rand() % (256), rand() % (256), rand() % (256));
        break;
    default:
        printf("invalid entry\n");
        exit(0);
    }

    switch (ch)
    {
    case 's':
        for (int i = 0; i < snake[0].total_length + 1; i++) // draws tail
        {
            draw_square(tail[i][0], tail[i][1], scale);
        }
        draw_square(snake[0].x, snake[0].y, scale); // draws snake head
        break;
    case 'c':
        for (int i = 0; i < snake[0].total_length + 1; i++) // draws tail
        {
            gfx_circle(tail[i][0], tail[i][1], scale / 2);
        }
        gfx_circle(snake[0].x, snake[0].y, scale / 2);
        break;
    case 't':
        for (int i = 0; i < snake[0].total_length + 1; i++) // draws tail
        {
            triangle(tail[i][0], tail[i][1], scale);
        }
        triangle(snake[0].x, snake[0].y, scale);
        break;
    default:
        printf("invalid entry\n");
        exit(0);
    }
}
void update(Object snake[], int tail[][2], int scale) // update so all elements of snake tail follow same path as the head
{

    for (int i = 0; i < snake[0].total_length; i++)
    {
        tail[i][0] = tail[i + 1][0]; // x coordinates of tail
        tail[i][1] = tail[i + 1][1]; // y coordinates of tail
    }

    if (snake[0].total_length >= 1)
    {
        tail[snake[0].total_length - 1][0] = snake[0].x; // add tail by making past element shifted from what's before it
        tail[snake[0].total_length - 1][1] = snake[0].y;
    }

    snake[0].x += snake[0].vx * scale; // scale makes the snake move by a certain frame rate
    snake[0].y += snake[0].vy * scale;
}

void draw_square(int xc, int yc, int side) // draw a square
{
    gfx_line(xc - side / 2, yc - side / 2, xc + side / 2, yc - side / 2); // bottom line
    gfx_line(xc - side / 2, yc - side / 2, xc - side / 2, yc + side / 2); // left line
    gfx_line(xc - side / 2, yc + side / 2, xc + side / 2, yc + side / 2); // top line
    gfx_line(xc + side / 2, yc + side / 2, xc + side / 2, yc - side / 2); // right line
}

void initialize(int board_min, int board_max, int scale, int wid, int ht, int margin) // draws game board borders and text
{
    gfx_color(255, 255, 255);
    gfx_text(450, 25, "Welcome to the Arcade!");
    gfx_text(450, 790, "Firework Display");
    gfx_text(250, 830, "Ramzi: \"The snake game is what Samuel L'Jackson says\"");
    gfx_text(300, 860, "Jackson during Snakes on a Plane:");
    gfx_text(300, 880, "\"I have had it with these motherf-- snakes on this motherf-- plane.\"");

    gfx_text(70, 70, "Customize Your Game");
    gfx_text(35, 100, "Difficulty Level:");
    gfx_text(35, 120, "  - 1");
    gfx_text(35, 140, "  - 2");
    gfx_text(35, 160, "  - 3");
    gfx_text(35, 190, "Head Shape:");
    gfx_text(35, 210, "  - square");
    gfx_text(35, 230, "  - circle");
    gfx_text(35, 250, "  - triangle");
    gfx_text(35, 280, "Snake Color:");
    gfx_text(35, 300, "  - white");
    gfx_text(35, 320, "  - red");
    gfx_text(35, 340, "  - orange");
    gfx_text(35, 360, "  - yellow");
    gfx_text(35, 380, "  - green");
    gfx_text(35, 400, "  - blue");
    gfx_text(35, 420, "  - purple");
    gfx_text(35, 440, "  - rainbow");

    gfx_color(255, 255, 255);
    gfx_text(790, 70, "Directions:");
    gfx_text(790, 100, "Press any arrow key");
    gfx_text(790, 120, "to begin the game.");
    gfx_text(790, 140, "Alternatively, use WASD ");
    gfx_text(790, 160, "keys to navigate.");
    gfx_text(790, 210, "Exit and rerun executable");
    gfx_text(790, 230, "to change preferences");
    gfx_text(790, 250, "on color and shape.");
    gfx_text(790, 290, "Collect fruit and grow");
    gfx_text(790, 310, "the snake as long as");
    gfx_text(790, 330, "possible without hitting");
    gfx_text(790, 350, "game walls or your tail.");
    gfx_text(790, 390, "When you're done playing");
    gfx_text(790, 410, "the game, enjoy the ");
    gfx_text(790, 430, "continuous fireworks show!");
    // gfx_text(790, 440, "  - rainbow");

    int display_len = 250;
    // draw game borders
    gfx_line(board_min, margin, board_max, margin);
    gfx_line(board_max, margin, board_max, board_max - board_min);
    gfx_line(board_max, board_max - board_min, board_min, board_max - board_min);
    gfx_line(board_min, board_max - board_min, board_min, margin);

    // draw window lines
    gfx_line(0, 0, wid, 0); // window lines
    gfx_line(wid - 1, 0, wid - 1, ht - 1);
    gfx_line(wid - 1, ht - 1, 0, ht - 1);
    gfx_line(0, ht, 0, 0);

    // draw window squares
    int side_square = 30;
    while (side_square > 1)
    {
        draw_square(wid, ht, side_square);
        draw_square(wid, 0, side_square);
        draw_square(0, ht, side_square);
        draw_square(0, 0, side_square);
        side_square = side_square * 0.95;
    }

    // draw left side display
    gfx_line(scale, margin, display_len - scale, margin);
    gfx_line(display_len - scale, margin, display_len - scale, board_max - board_min);
    gfx_line(display_len - scale, board_max - board_min, scale, board_max - board_min);
    gfx_line(scale, board_max - board_min, scale, margin);

    // draw right side display
    gfx_line(board_max + scale, margin, board_max + display_len - scale, margin);
    gfx_line(board_max + display_len - scale, margin, board_max + display_len - scale, board_max - board_min);
    gfx_line(board_max + display_len - scale, board_max - board_min, board_max + scale, board_max - board_min);
    gfx_line(board_max + scale, board_max - board_min, board_max + scale, margin);

    // draw bottom display
    gfx_line(scale, board_max - board_min + scale, board_max + display_len - scale, board_max - board_min + scale);
    gfx_line(board_max + display_len - scale, board_max - board_min + scale, board_max + display_len - scale, board_max - board_min + scale + display_len);
    gfx_line(board_max + display_len - scale, board_max - board_min + scale + display_len, scale, board_max - board_min + scale + display_len);
    gfx_line(scale, board_max - board_min + scale + display_len, scale, board_max - board_min + scale);

    draw_decor_remote();
    draw_button();
    int r = 0, g = 100, b = 30;
    draw_slot(r, g, b);
}

void draw_decor_remote() // draw joystick at bottom
{
    gfx_line(770, 900, 750, 900);
    gfx_line(750, 900, 750, 850);
    gfx_line(750, 850, 770, 850);
    gfx_line(770, 850, 770, 900);
    gfx_color(255, 0, 0);
    int r = 25;
    while (r > 1)
    {
        gfx_circle(760, 825, r);
        r = r * 0.95;
    }
}

void draw_button() // draw decorative yellow button
{
    gfx_color(222, 214, 58);
    int s = 50;
    while (s > 1)
    {
        draw_square(825, 875, s);
        s = s * 0.95;
    }
}

void draw_slot(int r, int g, int b) // draw decorative coin slot
{
    gfx_text(50, 870, "Thank you for Your Support!");
    gfx_text(145, 840, "$1");
    gfx_color(r, g, b);
    int x1 = 100, x2 = 200, y1 = 820, y2 = 850;
    gfx_line(x1, y1, x2, y1);
    gfx_line(x2, y1, x2, y2);
    gfx_line(x2, y2, x1, y2);
    gfx_line(x1, y2, x1, y1);
}

void triangle(int x, int y, int size) // draw a triangle
{
    gfx_line(x, y + sqrt(3) * size / 4, x + size / 2, y - sqrt(3) * size / 4);
    gfx_line(x + size / 2, y - sqrt(3) * size / 4, x - size / 2, y - sqrt(3) * size / 4);
    gfx_line(x - size / 2, y - sqrt(3) * size / 4, x, y + sqrt(3) * size / 4);
}