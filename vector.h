#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct VectorStruct {
    float x, y;
} Vector;

typedef struct MatrixStruct {
    double _11, _12, _13;
    double _21, _22, _23;
    double _31, _32, _33;
} Matrix;

typedef struct LineStruct {
    Vector point1, point2;
} Line;

Vector init_vector(float, float);
Vector init_vector_from_direction_and_magnitude(double, double);
Vector vector_truncate(Vector, float);
Vector vector_multiply_scalar(Vector, float);
Vector vector_add(Vector, Vector);
Vector vector_subtract(Vector, Vector);
float vector_get_magnitude(Vector);
double vector_get_magnitude_squared(Vector);
Vector vector_set_magnitude(Vector, float);
Vector vector_get_perp(Vector);
double vector_get_direction_radians(Vector);
double vector_get_distance(Vector, Vector);
double vector_get_manhattan_distance(Vector, Vector);
Vector vector_to_world_space(Vector, Vector, Vector);
Vector matrix_transform_vector(Matrix, Vector);

Vector get_line_segment_intersection(Line, Line, bool*);

double max(double, double);

#endif
