#include <stdio.h>
#include <GL/glut.h>
#include "game_element.h"
#include "steering.h"

void render1(GameElement* game_element){
    float halfHeight = game_element->height / 2;
    float halfWidth = game_element->width / 2;
    float quarterHeight = game_element->height / 4;
    glPushMatrix();
    glTranslatef(game_element->xPosition, game_element->yPosition, 0);
    glRotatef(game_element->heading, 0, 0, 1);
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
        float xPosition,
        float yPosition,
        float heading,
        float width,
        float height,
        float red,
        float green,
        float blue,
        void (*render)(GameElement*)){
    GameElement* game_element = malloc(sizeof(GameElement));
    game_element->steering_controller = init_steering_controller();
    game_element->xPosition = xPosition;
    game_element->yPosition = yPosition;
    game_element->height = height;
    game_element->width = width;
    game_element->red = red;
    game_element->green = green;
    game_element->blue = blue;
    game_element->render = render;
    return game_element;
}


