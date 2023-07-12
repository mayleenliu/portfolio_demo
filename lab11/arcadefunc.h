// Mayleen Liu
// Fundamentals of Computing
// arcadefunc.c
// Final project arcade game header file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "gfx.h"

typedef struct
{
    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
    int total_length;
    int random;
} Object;

typedef struct
{
    int x_pos;
    int y_pos;
    int x_vel;
    int y_vel;
    int x_acc;
    int y_acc;
    int p_x;
    int p_y;
} Firework;

void display_choices();
int user_preferences(int difficulty);

// Snake
void initialize(int, int, int, int, int, int);
void update(Object snake[], int tail[][2], int scale);
void draw_snake(Object snake[], int tail[][2], int scale, char ch, char color);
void keyPressed(Object snake[], char c, int x, int y);
void check_boundaries(Object snake[], int wid, int ht, int tail[][2], int scale, int board_min, int board_max, int margin);
void draw_food(Object food, int r);
bool eat(Object food, Object snake[], int r);
void draw_square(int xc, int yc, int side);
void triangle(int x, int y, int size);
void death(Object snake[], int tail[][2], int wid, int ht);
void initialize_snake(Object snake[]);

// Fireworks
void display_dot(Firework dot[], double gravity, Object particle[]);
void update_dot(Firework dot[], int i);
void forces(Firework dot[], double force, int i);
void display_particles(Object particle[], double force);
void update_particle(Object particle[], int i);
int reset_coordinates_fireworks(Object particle[], Firework dot[], double gravity, int msecond_count);

// Decoration
void draw_decor_remote();
void draw_button();
void draw_slot(int r, int g, int b);
