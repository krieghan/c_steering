#include <math.h>
#include "vector.h"

Vector init_vector(float x, float y){
    Vector vector = {.x = x, .y = y};
    return vector;
}

Vector vector_truncate(Vector vector, float cap){
    float magnitude = vector_get_magnitude(vector);
    if (magnitude > cap){
        return vector_set_magnitude(vector, cap);
    } else {
        return vector;
    }
}

Vector vector_multiply_scalar(Vector vector, float scalar){
    Vector new_vector = {
        .x = vector.x * scalar,
        .y = vector.y * scalar};
    return new_vector;
}

Vector vector_add(Vector vector1, Vector vector2){
    Vector new_vector = {
        .x = vector1.x + vector2.x,
        .y = vector1.y + vector2.y};
    return new_vector;
}

float vector_get_magnitude(Vector vector){
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

Vector vector_set_magnitude(Vector vector, float magnitude){
    double directionRadians = vector_get_direction_radians(vector);
    Vector new_vector = {.x = magnitude * cos(directionRadians),
                         .y = magnitude * sin(directionRadians)};
    return new_vector;
}

Vector vector_get_perp(Vector vector){
    Vector new_vector = {.x = -vector.y, .y = vector.x};
    return new_vector;
}
    

double vector_get_direction_radians(Vector vector){
    return atan2(vector.y, vector.x);
}

Vector vector_to_world_space(
        Vector vector,
        Vector fwd,
        Vector side){
    Matrix mat;
    mat._11 = fwd.x; mat._12 = fwd.y; mat._13 = 0;
    mat._21 = side.x; mat._22 = side.y; mat._23 = 0;
    mat._31 = 0; mat._32 = 0; mat._33 = 1;
    return matrix_transform_vector(mat, vector);
}

Vector matrix_transform_vector(Matrix matrix, Vector vector){
    Vector new_vector;
    new_vector.x = 
        (matrix._11 * vector.x) + 
        (matrix._21 * vector.y) + 
        (matrix._31);
    new_vector.y =
        (matrix._12 * vector.x) +
        (matrix._22 * vector.y) +
        (matrix._32);
    return new_vector;
}

