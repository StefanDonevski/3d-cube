#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#undef main

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define POINT_SIZE 5
#define COORDINATE_SYSTEM_OFFSET_X SCREEN_WIDTH/2
#define COORDINATE_SYSTEM_OFFSET_Y SCREEN_HEIGHT/2
#define NUMBER_OF_POINTS 1200

struct Point {
	double x;
	double y;
	double z;
};

int draw_point(SDL_Surface* surface, int x, int y)
{
	SDL_Rect rect = (SDL_Rect){ x, y, POINT_SIZE, POINT_SIZE };
	SDL_FillRect(surface, &rect, COLOR_WHITE);
}

int draw_point_3d(SDL_Surface* surface, struct Point point)
{
	int x_2d = point.x + COORDINATE_SYSTEM_OFFSET_X;
	int y_2d = point.y + COORDINATE_SYSTEM_OFFSET_Y;
	draw_point(surface, x_2d, y_2d);
}

int draw_points_3d(SDL_Surface* surface, struct Point points[], int number_of_points)
{
	for (int i = 0; i < number_of_points; i++)
	{
		int x_2d = points[i].x + COORDINATE_SYSTEM_OFFSET_X;
		int y_2d = points[i].y + COORDINATE_SYSTEM_OFFSET_Y;
		draw_point(surface, x_2d, y_2d);
	}
}

void apply_rotating(struct Point* point, double alpha, double beta, double gamma)
{
	double rotation_matrix[3][3] = { {cos(beta) * cos(gamma), sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma), cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma)},{cos(beta) * sin(gamma), sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma), sin(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma)}, {-sin(beta), sin(alpha) * cos(beta), cos(alpha) * cos(beta)} };
	double point_vector[3] = { point->x, point->y, point->z };
	double result_point[3];
	for (int i = 0; i < 3; i++)
	{
		double dot_product = 0;
		for (int j = 0; j < 3; j++)
		{
			dot_product += rotation_matrix[i][j] * point_vector[j];
		}
		result_point[i] = dot_product;
	}
	point->x = result_point[0];
	point->y = result_point[1];
	point->z = result_point[2];
}

void initialized_cube(struct Point points[], int number_of_points)
{
	int points_per_side = number_of_points / 12;

	int SIDE_LENGTH = 200;

	int step_size = SIDE_LENGTH / points_per_side;

	//side 1
	for (int i = 0; i < points_per_side; i++) {
		points[i] = (struct Point){ -SIDE_LENGTH / 2 + i * step_size, -SIDE_LENGTH / 2, +SIDE_LENGTH / 2 };
	}

	//side 2
	for (int i = 0; i < points_per_side; i++) {
		points[i + points_per_side] = (struct Point){ -SIDE_LENGTH / 2 + i * step_size, SIDE_LENGTH / 2, +SIDE_LENGTH / 2 };
	}

	//side 3
	for (int i = 0; i < points_per_side; i++) {
		points[i + 3 * points_per_side] = (struct Point){ SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size, +SIDE_LENGTH / 2 };
	}

	//side 4
	for (int i = 0; i < points_per_side; i++) {
		points[i + 2 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size, +SIDE_LENGTH / 2 };
	}

	//side 5
	for (int i = 0; i < points_per_side; i++) {
		points[i + 4 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2 + i * step_size, -SIDE_LENGTH / 2, -SIDE_LENGTH / 2 };
	}

	//side 6
	for (int i = 0; i < points_per_side; i++) {
		points[i + 5 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2 + i * step_size, SIDE_LENGTH / 2, -SIDE_LENGTH / 2 };
	}

	//side 7
	for (int i = 0; i < points_per_side; i++) {
		points[i + 6 * points_per_side] = (struct Point){ SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size, -SIDE_LENGTH / 2 };
	}

	//side 8
	for (int i = 0; i < points_per_side; i++) {
		points[i + 7 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size, -SIDE_LENGTH / 2 };
	}

	//side 9
	for (int i = 0; i < points_per_side; i++) {
		points[i + 8 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2, -SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size };
	}

	//side 10
	for (int i = 0; i < points_per_side; i++) {
		points[i + 9 * points_per_side] = (struct Point){ SIDE_LENGTH / 2, -SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size };
	}

	//side 11
	for (int i = 0; i < points_per_side; i++) {
		points[i + 10 * points_per_side] = (struct Point){ SIDE_LENGTH / 2, SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size };
	}

	//side 12
	for (int i = 0; i < points_per_side; i++) {
		points[i + 11 * points_per_side] = (struct Point){ -SIDE_LENGTH / 2, SIDE_LENGTH / 2, -SIDE_LENGTH / 2 + i * step_size };
	}
}

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("3D Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_Rect black_screen_rect = (SDL_Rect){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//struct Point point = { 0, 0, 0 };
	struct Point points[NUMBER_OF_POINTS];
	initialized_cube(points, NUMBER_OF_POINTS);
	draw_points_3d(surface, points, NUMBER_OF_POINTS);
	SDL_Event event;
	int simulation_runnig = 1;
	double alpha = 0.01;
	double beta = 0.01;
	double gamma = 0.01;
	while (simulation_runnig)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				simulation_runnig = 0;
			}
		}

		for (int i = 0; i < NUMBER_OF_POINTS; i++)
		{
			apply_rotating(&points[i], alpha, beta, gamma);
		}

		SDL_FillRect(surface, &black_screen_rect, COLOR_BLACK);
		draw_points_3d(surface, points, NUMBER_OF_POINTS);

		SDL_UpdateWindowSurface(window);

		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}