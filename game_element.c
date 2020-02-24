#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "game_element.h"

void render1(GameElement* game_element){
    float halfHeight = game_element->height / 2;
    float halfWidth = game_element->width / 2;
    float quarterHeight = game_element->height / 4;
    glPushMatrix();
    glTranslatef(game_element->position.x, game_element->position.y, 0);
    glRotatef(game_element->direction_degrees, 0, 0, 1);
    glColor3f(game_element->red, game_element->green, game_element->blue);
    glBegin(GL_LINES);
    glVertex2f(halfHeight, halfWidth);
    glVertex2f(-halfHeight, halfWidth);
    glVertex2f(halfHeight, -halfWidth);
    glVertex2f(-halfHeight, -halfWidth);
    glVertex2f(quarterHeight, halfWidth);
    glVertex2f(quarterHeight, -halfWidth);
    glVertex2f(-quarterHeight, halfWidth);
    glVertex2f(-quarterHeight, -halfWidth);
    glVertex2f(quarterHeight, 0);
    glVertex2f(halfHeight, 0);
    glEnd();
    glPopMatrix();
}

GameElement* game_element_init(
        Vector position,
        Vector velocity,
        float width,
        float height,
        float mass,
        float max_force,
        float max_speed,
        float red,
        float green,
        float blue,
        void (*render)(GameElement*)){
    GameElement* game_element = malloc(sizeof(GameElement));
    game_element->steering_controller = init_steering_controller(game_element);
    game_element->position = position;
    game_element->velocity = velocity;
    game_element->height = height;
    game_element->width = width;
    game_element->mass = mass;
    game_element->max_force = max_force;
    game_element->max_speed = max_speed;
    game_element->red = red;
    game_element->green = green;
    game_element->blue = blue;
    game_element->render = render;
    game_element_update_heading(game_element);
    return game_element;
}

void game_element_update(GameElement* game_element, int timeElapsed){
    Vector acceleration;
    Vector force = calculate_force(game_element->steering_controller);
    force = vector_truncate(force, game_element->max_force);
    acceleration = vector_multiply_scalar(
            force,  
            timeElapsed / (game_element->mass));
    //printf("%d %f %f\n", timeElapsed, acceleration.x, acceleration.y);
    game_element->velocity = vector_truncate(
            vector_add(
                game_element->velocity,
                acceleration),
            game_element->max_speed);
    game_element->position = vector_add(
            game_element->position,
            game_element->velocity);
    game_element_update_heading(game_element);
}

void game_element_update_heading(GameElement* game_element){
    game_element->heading = vector_set_magnitude(
            game_element->velocity,
            1);
    game_element->heading_perp = vector_get_perp(
            game_element->heading);
    game_element->direction_radians = vector_get_direction_radians(
            game_element->heading);
    game_element->direction_degrees =
        game_element->direction_radians * (180 / M_PI);

}

Wall* init_wall(double x1, double y1, double x2, double y2){
    Wall* wall = malloc(sizeof(Wall));
    Vector point1 = {.x = x1, .y = y1};
    Vector point2 = {.x = x2, .y = y2};
    wall->point1 = point1;
    wall->point2 = point2;
    return wall;
}

void wall_render(Wall* wall){
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(wall->point1.x, wall->point1.y);
    glVertex2f(wall->point2.x, wall->point2.y);
    glEnd();
    glPopMatrix();
}
