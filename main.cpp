#include <iostream>
#include <math.h>
#define M_PI       3.14159265358979323846
using namespace std;
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include "Vector.h"

const int DISPLAY_WIDTH = 1366;
const int DISPLAY_HEIGHT = 768;
const float epsilon = 20.0;


void checkIfInside(Vector &pos, Vector &speed, float r){
	if (pos.x + r >= DISPLAY_WIDTH || pos.x - r <= 0){
		speed.x *= -1;
		if (pos.x + r >= DISPLAY_WIDTH){
			pos.x = DISPLAY_WIDTH - r;
		}
		else {
			pos.x = r;
		}
	}

	if (pos.y + r >= DISPLAY_HEIGHT || pos.y - r <= 0){
		speed.y *= -1;
		if (pos.y + r >= DISPLAY_HEIGHT){
			pos.y = DISPLAY_HEIGHT - r;
		}
		else {
			pos.y = r;
		}
	}
}
void checkIfCollide(Vector &ballPos, Vector playerPos, Vector &speed, float R, float r, Vector aps){

	Vector v(playerPos.x - ballPos.x, playerPos.y - playerPos.y);
	float norm = 2*sqrt(speed.x*speed.x + speed.y*speed.y);
	float alpha = atan(speed.y / speed.x);
	float beta = asin(fabsf((ballPos.y - playerPos.y) / (R + r)));
	float sigma = acos((speed.x * v.x + speed.y * v.y) / (sqrt(speed.x * speed.x + speed.y * speed.y) * sqrt(v.y * v.y + v.x * v.x)));

	if (sqrt(pow(ballPos.x - playerPos.x, 2) + pow(ballPos.y - playerPos.y, 2)) < (R + r )){
		if (sigma >= 0 && sigma <= M_PI / 4) {
			speed.rotate(2 * (beta - alpha));
			speed.nullify();
			speed = speed + aps/1.5;
			ballPos = ballPos + speed + speed + speed;
		}
		else {
			speed.rotate((2 * alpha + 2 * beta));
			speed = speed + aps/1.5;
		}
	}
}
void incrementSpeeds(Vector &ballSpeed, Vector &actualPlayerSpeed, float ballRadius, float playerRadius, Vector &ballPosition, Vector &playerPosition){
	if (ballSpeed.x < 0){
		ballSpeed.x += 0.05;
		if (ballSpeed.x >= 0) {
			ballSpeed.x = 0;
		}
	}
	else if (ballSpeed.x > 0) {
		ballSpeed.x -= 0.05;
		if (ballSpeed.x <= 0){
			ballSpeed.x = 0;
		}

	}
	ballSpeed.y += 1;
	if (ballPosition.y >= DISPLAY_HEIGHT){
		ballSpeed.y = DISPLAY_HEIGHT - ballRadius;
		ballSpeed.y = 0;
	}
	actualPlayerSpeed.y += 0.5;
	ballPosition = ballPosition + ballSpeed;
	checkIfInside(ballPosition, ballSpeed, ballRadius);
	playerPosition = playerPosition + actualPlayerSpeed;
	if (playerPosition.y >= DISPLAY_HEIGHT - playerRadius - 1){
		playerPosition.y = DISPLAY_HEIGHT - playerRadius - 1;
		actualPlayerSpeed.y = 0;
	}
}

int main(){
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT event;

	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();

	timer = al_create_timer(1.0 / 60.0);

	display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	al_set_window_position(display, 100, 100);
	
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool running = true, redraw = false;
	bool KEYS[256];
	int c = 0;

	Vector ballPosition(100, 100);
	Vector ballSpeed(5, 5);
	Vector playerPosition(DISPLAY_WIDTH/2, DISPLAY_HEIGHT - 40);
	Vector playerSpeed(5, 0);
	Vector actualPlayerSpeed(0, 0);

	float playerRadius = 50, ballRadius = 20;

	al_start_timer(timer);
	
	while (running){
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER){
			redraw = true;
			incrementSpeeds(ballSpeed, actualPlayerSpeed, ballRadius, playerRadius, ballPosition, playerPosition);
			checkIfCollide(ballPosition, playerPosition, ballSpeed, playerRadius, ballRadius, actualPlayerSpeed);
		}

		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
		}

		else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				running = false;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_W){
				actualPlayerSpeed.y -= 15;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_R){
				ballPosition.reset();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_S){
				ballSpeed.y = 30;
				ballSpeed.x = 30;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_A){
				KEYS['A'] = TRUE;
				actualPlayerSpeed.changeCoords(-5, actualPlayerSpeed.y);
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_D){
				KEYS['D'] = TRUE;
				actualPlayerSpeed.changeCoords(10, actualPlayerSpeed.y);
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE){
				actualPlayerSpeed.y -= 10;
			}
		}

		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			if (event.keyboard.keycode == ALLEGRO_KEY_W){

			}

			if (event.keyboard.keycode == ALLEGRO_KEY_S){

			}

			if (event.keyboard.keycode == ALLEGRO_KEY_A){
				KEYS['A'] = FALSE;
				actualPlayerSpeed.changeCoords(0, actualPlayerSpeed.y);
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_D){
				KEYS['D'] = FALSE;
				actualPlayerSpeed.changeCoords(0, actualPlayerSpeed.y);
			}
		}

		if (redraw){
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_filled_circle(playerPosition.x, playerPosition.y, playerRadius, al_map_rgb(100, 0, 255));
			al_draw_filled_circle(ballPosition.x, ballPosition.y, ballRadius, al_map_rgb(255, 180, 100));

			al_flip_display();
		}
	}
	return 0;
}