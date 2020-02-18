
// C program to demonstrate 
// drawing a circle using 
// OpenGL 
#include <stdio.h> 
#include <stdlib.h>
#include <GL/glut.h> 
#include <math.h> 
#include "world.h"
#include "linkedlist.h"
#include "game_element.h"
#define pi 3.142857 

World* world;

// function to initialize 
void myInit (void) 
{ 
    // making background color black as first  
    // 3 arguments all are 0.0 
    glClearColor(0.0, 0.0, 0.0, 0.0); 
      
    glEnable(GL_TEXTURE_2D);
    //setupView(glutGet(GLUT_WINDOW_HEIGHT), glutGet(GLUT_WINDOW_WIDTH)):
    // making picture color green (in RGB mode), as middle argument is 1.0 
    glColor3f(0.0, 1.0, 0.0); 
      
    // breadth of picture boundary is 1 pixel 
    glPointSize(1.0); 
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
      
    // setting window dimension in X- and Y- direction 
    gluOrtho2D(0, world->width, 0, world->height); 
} 

void display(void)
{ 
    GameElement* current_game_element;
    LinkedListNode* current_node;
    glClear(GL_COLOR_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    current_node = world->elements->head;
    while(current_node){
        current_game_element = (GameElement*)current_node->data;
        current_game_element->render(current_game_element);
        current_node = current_node->next;
    }

    glutSwapBuffers();
      
    glEnd(); 
    glFlush(); 
} 
 
void handleTime(int timeElapsed){
    display();
    glutTimerFunc(1, &handleTime, 0);
}

void handleClick(int button, int state, int x, int y){
    int transformedY = world->height - y;
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP){
            GameElement* ship = game_element_init(
                x, transformedY,
                0,
                10, 10,
                0, 1, 0,
                &render1);
            append_to_list(world->elements, ship);
        }
    }
}

int main (int argc, char** argv) 
{ 
    world = world_init(500, 500);
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
      
    // giving window size in X- and Y- direction 
    glutInitWindowSize(world->width, world->height); 
    glutInitWindowPosition(0, 0); 
      
    // Giving name to window 
    glutCreateWindow("Escape"); 
    myInit(); 
      
    glutDisplayFunc(display); 
    //glutReshapeFunc(onSize);
    glutTimerFunc(1, &handleTime, 0);
    glutMouseFunc(&handleClick);

    glutMainLoop(); 
} 
