#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "vector.h"

Vector init_vector(float x, float y){
    Vector vector = {.x = x, .y = y};
    return vector;
}

Vector init_vector_from_direction_and_magnitude(
        double direction_radians,
        double magnitude){
    Vector new_vector = {
        .x = magnitude * cos(direction_radians),
        .y = magnitude * sin(direction_radians)};
    return new_vector;
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

double vector_get_magnitude_squared(Vector vector){
    return pow(vector.x, 2) + pow(vector.y, 2);
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

Vector vector_subtract(Vector vector1, Vector vector2){
    Vector new_vector = {
        .x = vector1.x - vector2.x,
        .y = vector1.y - vector2.y};
    return new_vector;
}

double vector_get_distance(Vector point1, Vector point2){
    double x_delta = point1.x - point2.x;
    double y_delta = point1.y - point2.y;
    return sqrt(pow(x_delta, 2) + pow(y_delta, 2));
}

double vector_get_direction_radians(Vector vector){
    return atan2(vector.y, vector.x);
}

double vector_dot_product(Vector vector1, Vector vector2){
    double result = (vector1.x * vector2.x) + (vector1.y * vector2.y);
    /*
    printf(
        "%f * %f + %f * %f = %f\n",
        vector1.x,
        vector2.x,
        vector1.y,
        vector2.y,
        result);
    */

    return result;
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

bool lines_are_parallel(Line line1, Line line2){
    Vector line1_vector = vector_subtract(line1.point2, line1.point1);
    Vector line2_vector = vector_subtract(line2.point2, line2.point1);
    Vector line2_perp = vector_get_perp(line2_vector);

    double dot = vector_dot_product(line2_perp, line1_vector);
    /*
    printf("dot %f\n", dot);
    */
    return dot == 0;
}

Vector get_line_segment_intersection(Line line1, Line line2, bool* success){
    Vector a_vector, b_vector, c_vector, d_vector;
    Vector a_point, b_point, c_point, d_point;
    Vector bperp, dperp;
    Vector a_to_intersection_point, intersection_point;
    Vector error_vector;
    if (lines_are_parallel(line1, line2)) {
        *success = false;
        return error_vector;
    }

    a_point = line1.point1;
    b_point = line1.point2;
    c_point = line2.point1;
    d_point = line2.point2;

    b_vector = vector_subtract(b_point, a_point);
    d_vector = vector_subtract(d_point, c_point);
    c_vector = vector_subtract(c_point, a_point);

    bperp = vector_get_perp(b_vector);
    dperp = vector_get_perp(d_vector);

    double dperpDotB = vector_dot_product(dperp, b_vector);
    double dperpDotC = vector_dot_product(dperp, c_vector);
    double bperpDotC = vector_dot_product(bperp, c_vector);

    double distance_along_b = dperpDotC / dperpDotB;
    double distance_along_d = bperpDotC / dperpDotB;

    //printf("inters: %f %f\n", distance_along_b, distance_along_d);
    if (distance_along_b > 0 && distance_along_b < 1 &&
           distance_along_d > 0 && distance_along_d < 1){
        a_to_intersection_point = vector_multiply_scalar(
            b_vector, distance_along_b);
        intersection_point = vector_add(
            a_point, a_to_intersection_point);
        *success = true;
        return intersection_point;
    } else {
        *success = false;
        return error_vector;
    }
}

double max(double x, double y){
    ((x) >= (y)) ? (x) : (y);
}
