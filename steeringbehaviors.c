#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "game_element.h"
#include "vector.h"

#define WANDER_DISTANCE 30
#define WANDER_RADIUS 10
#define WANDER_JITTER .03
#define RADIANS45 M_PI / 4


Vector wander(World* world, GameElement* game_element){
    Vector projectionVector = {.x = WANDER_DISTANCE, .y = 0};
    Vector centerToTargetVector = {.x = 0, .y = 0};
    Vector targetAdjustment = {
        .x = ((double)(rand())/RAND_MAX*2.0-1.0) * WANDER_JITTER,
        .y = ((double)(rand())/RAND_MAX*2.0-1.0) * WANDER_JITTER}; 
    centerToTargetVector = vector_set_magnitude(
        vector_add(
            centerToTargetVector,
            targetAdjustment),
        WANDER_RADIUS);
    Vector localTargetVector = vector_add(
            centerToTargetVector,
            projectionVector);
    Vector worldTargetVector = vector_to_world_space(
            localTargetVector,
            game_element->heading,
            game_element->heading_perp);
    //printf("%f %f\n", worldTargetVector.x, worldTargetVector.y);

    return worldTargetVector;
}

void create_feelers(
        GameElement* game_element,
        Line* lines){

    double feeler_length = game_element->height * 1.5;
    Vector left_vector = init_vector_from_direction_and_magnitude(
        game_element->direction_radians + RADIANS45,
        feeler_length);
    Vector forward_vector = init_vector_from_direction_and_magnitude(
        game_element->direction_radians,
        feeler_length);
    Vector right_vector = init_vector_from_direction_and_magnitude(
        game_element->direction_radians - RADIANS45,
        feeler_length);
    Vector left_point = vector_add(game_element->position, left_vector);
    Vector forward_point = vector_add(game_element->position, forward_vector);
    Vector right_point = vector_add(game_element->position, right_vector);

    lines[0].point1 = game_element->position;
    lines[0].point2 = left_point;
    lines[1].point1 = game_element->position;
    lines[1].point2 = forward_point;
    lines[2].point1 = game_element->position;
    lines[2].point2 = right_point;
}

Vector avoid_walls(World* world, GameElement* game_element){
    Line feelers[3];
    create_feelers(game_element, feelers);
    LinkedListNode* current_node;
    Wall* current_wall;
    Wall* closest_wall = NULL;
    Vector intersection_point, element_to_intersection;
    Vector closest_intersection;
    Vector force;
    int closest_feeler_index;
    double dist_sq_to_closest_intersection = DBL_MAX;
    double dist_sq_to_intersection;
    bool success = false;
    
    for (int i = 0; i < 3; i++){
        current_node = world->walls->head;
        while (current_node != NULL){
            current_wall = (Wall*)current_node->data;
            current_node = current_node->next;
            /*
            printf(
                "feeler: %f %f %f %f\n",
                feelers[i].point1.x,
                feelers[i].point1.y,
                feelers[i].point2.x,
                feelers[i].point2.y);
            */
            intersection_point = get_line_segment_intersection(
                feelers[i],
                current_wall->line,
                &success);
            if (!success){
                continue;
            }

            element_to_intersection = vector_subtract(
                intersection_point,
                game_element->position);
            dist_sq_to_intersection = vector_get_magnitude_squared(
                element_to_intersection);
            //printf("dist: %f %f\n", dist_sq_to_intersection, dist_sq_to_closest_intersection);
            if (dist_sq_to_intersection < dist_sq_to_closest_intersection){
                //printf("picking a wall\n");
                dist_sq_to_closest_intersection = dist_sq_to_intersection;
                closest_wall = current_wall;
                closest_intersection = intersection_point;
                closest_feeler_index = i;
            }
        }
    }

    if (!closest_wall){
        force.x = 0;
        force.y = 0;
        return force;
    }
    Vector normalized_wall_vector = vector_set_magnitude(
        vector_subtract(
            closest_wall->line.point2,
            closest_wall->line.point1),
        1);
    Vector wall_perp = vector_get_perp(normalized_wall_vector);
    Vector intersection_to_feeler_endpoint = vector_subtract(
        feelers[closest_feeler_index].point2,
        closest_intersection);
    double overshoot_length = vector_get_magnitude(
            intersection_to_feeler_endpoint);
    force = vector_multiply_scalar(
        wall_perp,
        overshoot_length * 100);
    //printf("avoid: %f %f\n", force.x, force.y);
    return force;
}

Vector separate(World* world, GameElement* agent, LinkedList* neighbors){
    Vector agent_position = agent->position;
    Vector neighbor_to_agent, neighbor_heading_to_agent;
    Vector cumulative_force = {.x = 0, .y = 0};
    double distance_to_neighbor, magnitude;
    LinkedListNode* current_node = neighbors->head;
    GameElement* current_neighbor;

    while (current_node){
        current_neighbor = (GameElement*)current_node->data;
        current_node = current_node->next;
        neighbor_to_agent = vector_subtract(
            agent_position,
            current_neighbor->position);
        distance_to_neighbor = vector_get_magnitude(
            neighbor_to_agent);
        if (distance_to_neighbor == 0){
            neighbor_heading_to_agent.x = 
                (double)(rand()) / RAND_MAX;
            neighbor_heading_to_agent.y = 
                (double)(rand()) / RAND_MAX;
            neighbor_heading_to_agent = vector_set_magnitude(
                neighbor_heading_to_agent,
                1);
            magnitude = 100;
        } else {
            neighbor_heading_to_agent = vector_set_magnitude(
                neighbor_to_agent,
                1);
            magnitude = 
                max(agent->height, agent->width) / distance_to_neighbor;
        }
        vector_add(
            cumulative_force,
            vector_multiply_scalar(
                neighbor_heading_to_agent,
                magnitude));
    }
    return cumulative_force;
}

Vector align(World* world, GameElement* agent, LinkedList* neighbors){
    LinkedListNode* current_node = neighbors->head;
    GameElement* current_neighbor;

    Vector cumulative_heading = {.x = 0, .y = 0};
    Vector neighbor_heading;

    if (!neighbors->size){
        return cumulative_heading;
    }

    while (current_node){
        current_neighbor = (GameElement*)current_node->data;
        cumulative_heading = vector_add(
            cumulative_heading,
            vector_set_magnitude(
                current_neighbor->velocity,
                1));
        current_node = current_node->next;
    }
    cumulative_heading = vector_multiply_scalar(
        cumulative_heading,
        (1 / neighbors->size));

    return cumulative_heading;
}

Vector seek(World* world, GameElement* agent, Vector seek_point){
    return vector_subtract(
        vector_set_magnitude(
            vector_subtract(
                seek_point,
                agent->position),
            agent->max_speed),
        agent->velocity);
}

Vector cohere(World* world, GameElement* agent, LinkedList* neighbors){
    Vector agent_position = agent->position;
    Vector cumulative_vector = {.x = 0, .y = 0};
    LinkedListNode* current_node = neighbors->head;
    GameElement* current_neighbor;

    if (!neighbors->size){
        return cumulative_vector;
    }

    while (current_node){
        current_neighbor = (GameElement*)current_node->data;
        cumulative_vector = vector_add(
            cumulative_vector,
            current_neighbor->position);
        current_node = current_node->next;
    }

    Vector average_point = vector_multiply_scalar(
        cumulative_vector,
        (1 / neighbors->size));

    return seek(world, agent, average_point);
}

Vector flock(World* world, GameElement* game_element){
    double neighbor_threshold = 100;
    Vector force;
    LinkedList* neighbors = game_element_get_neighbors(
            world,
            game_element,
            neighbor_threshold);
    Vector separation_force = separate(world, game_element, neighbors);
    Vector alignment_force = align(world, game_element, neighbors);
    Vector cohesion_force = cohere(world, game_element, neighbors);
    destruct_linked_list(neighbors, false);
    force = vector_add(
        separation_force,
        vector_add(
            alignment_force,
            cohesion_force));
    printf("%f %f\n", force.x, force.y);
    return force;
}



