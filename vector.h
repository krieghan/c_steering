#ifndef VECTOR_H
#define VECTOR_H

typedef struct VectorStruct {
    float x, y;
} Vector;

typedef struct MatrixStruct {
    double _11, _12, _13;
    double _21, _22, _23;
    double _31, _32, _33;
} Matrix;

Vector init_vector(float, float);
Vector vector_truncate(Vector, float);
Vector vector_multiply_scalar(Vector, float);
Vector vector_add(Vector, Vector);
float vector_get_magnitude(Vector);
Vector vector_set_magnitude(Vector, float);
Vector vector_get_perp(Vector);
double vector_get_direction_radians(Vector);
Vector vector_to_world_space(Vector, Vector, Vector);
Vector matrix_transform_vector(Matrix, Vector);


#endif
