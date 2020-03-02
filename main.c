#include <stdio.h> 

#include <stdlib.h>
#include <time.h>
#include <GL/glut.h> 
#include <math.h> 
#include "vector.h"
#include "linkedlist.h"
#include "game_element.h"

#define SHIP_TOTAL 10
#define UPDATE_RATE 1

World* world;
Canvas* canvas;
int ship_count = 0;


void display(void)
{ 
    GameElement* current_game_element;
    Wall* current_wall;
    LinkedListNode* current_node;
    glClear(GL_COLOR_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    current_node = world->moving_elements->head;
    while(current_node){
        current_game_element = (GameElement*)current_node->data;
        current_game_element->render(current_game_element);
        current_node = current_node->next;
    }

    current_node = world->walls->head;
    while(current_node){
        current_wall = (Wall*)current_node->data;
        wall_render(current_wall);
        current_node = current_node->next;
    }

    glutSwapBuffers();
      
    glEnd(); 
    glFlush(); 
} 

void setupView(int client_width, int client_height){
    int world_height = world->height;
    int world_width = world->width;
    double screen_ratio = (double)client_width / client_height;
    double world_ratio = (double)world_width / world_height;
    int viewport_left, viewport_bottom, viewport_top, viewport_right;
    int viewport_width, viewport_height;

    if (world_ratio >= screen_ratio){
        viewport_left = 0;
        viewport_bottom = (int)((client_height - (client_width / world_ratio)) / 2);
        //printf("case 1: %d %d %f %d %d\n", client_height, client_width, screen_ratio, viewport_left, viewport_bottom);
        viewport_width = (int)client_width;
        viewport_height = (int)(client_width / world_ratio);
    } else {
        viewport_left = (int)((client_width - (client_height * world_ratio)) / 2);
        viewport_bottom = 0;
        viewport_width = (int)(client_height * world_ratio);
        viewport_height = client_height;
        //printf("case 2: %d %d %f %d %d\n", client_height, client_width, screen_ratio, viewport_left, viewport_bottom);
    }

    viewport_right = viewport_left + viewport_width;
    viewport_top = viewport_bottom + viewport_height;

    //printf("%d %d %d %d %d %d\n", viewport_left, viewport_bottom, viewport_width, viewport_height, world_width, world_height);

    glViewport(
        viewport_left,
        viewport_bottom,
        viewport_width,
        viewport_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, world_width + 1, -1, world_height + 1);
    canvas->screen_ratio = screen_ratio;
    canvas->world_ratio = world_ratio;
    canvas->viewport_left = viewport_left;
    canvas->viewport_bottom = viewport_bottom;
    canvas->viewport_top = viewport_top;
    canvas->viewport_right = viewport_right;
    canvas->viewport_width = viewport_width;
    canvas->viewport_height = viewport_height;
}

// function to initialize 
void myInit (void) 
{ 
    append_to_list(
        world->walls,
        init_wall(0, 0, world->width, 0));
    append_to_list(
        world->walls,
        init_wall(0, world->height, 0, 0));
    append_to_list(
        world->walls,
        init_wall(world->width, world->height, 0, world->height));
    append_to_list(
        world->walls,
        init_wall(world->width, 0, world->width, world->height));
    // making background color black as first  
    // 3 arguments all are 0.0 
    glClearColor(0.0, 0.0, 0.0, 0.0); 
      
    glEnable(GL_TEXTURE_2D);
    setupView(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    // making picture color green (in RGB mode), as middle argument is 1.0 
    glColor3f(0.0, 1.0, 0.0); 
      
    // breadth of picture boundary is 1 pixel 
    glPointSize(1.0); 
} 


void onSize(int width, int height){
    setupView(width, height);
}
 
void plot_ship(int x, int y){
    Vector position = {.x = x, .y = y};
    Vector velocity = {.x = 0, .y = 0};
    GameElement* ship = game_element_init(
        position,
        velocity,
        50, 50,
        1,
        50, 10,
        0, 1, 0,
        &render1);
    append_to_list(world->moving_elements, ship);
    ship_count += 1;
}

void handleTime(int timeElapsed){
    if (ship_count < SHIP_TOTAL){
        plot_ship(world->width / 2, world->height / 2);
    }
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    world_update(world, current_time);
    glutTimerFunc(UPDATE_RATE, &handleTime, 0);
    display();
}


void handleClick(int button, int state, int x, int y){
    int client_height, client_width;
    int transformedX, transformedY;
    double x_ratio, y_ratio;
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP){
            client_height = glutGet(GLUT_WINDOW_HEIGHT);
            client_width = glutGet(GLUT_WINDOW_WIDTH);
            x_ratio = (double)canvas->viewport_width / world->width;
            y_ratio = (double)canvas->viewport_height / world->height;
            transformedY = (
                client_height - (y + canvas->viewport_bottom)) / y_ratio;
            transformedX = (x - canvas->viewport_left) / x_ratio;
            if (transformedX < 0 || 
                transformedX > world->width || 
                transformedY < 0 || 
                transformedY > world->height){
                return;
            }
            plot_ship(transformedX, transformedY);
        }
    }
}

int main (int argc, char** argv) 
{ 
    srand(time(NULL));
    world = world_init(5000, 5000);
    canvas = canvas_init(world);
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
      
    // giving window size in X- and Y- direction 
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(0, 0); 
      
    // Giving name to window 
    glutCreateWindow("Escape"); 
    myInit(); 
      
    glutDisplayFunc(display); 
    glutReshapeFunc(onSize);
    glutTimerFunc(UPDATE_RATE, &handleTime, 0);
    glutMouseFunc(&handleClick);

    glutMainLoop(); 
} 
