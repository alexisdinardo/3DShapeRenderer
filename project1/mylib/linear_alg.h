#ifndef _LINEAR_ALG_H_
#define _LINEAR_ALG_H_

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    vec3 x;
    vec3 y;
    vec3 z;
} mat3;

//vector operations
void print_v4(vec4 v);
vec4 scalar_vector_mult(float s, vec4 v);
vec4 vector_addition(vec4 v1, vec4 v2);
vec4 vector_subtraction(vec4 v1, vec4 v2);
float magnitude_vector(vec4 v);
vec4 normalize(vec4 v);
float dot_product(vec4 v1, vec4 v2);
vec4 cross_product(vec4 v1, vec4 v2);

//matrix operations
void print_m4(mat4 m);
mat4 scalar_matrix_mult(float s, mat4 m);
mat4 matrix_addition(mat4 m1, mat4 m2);
mat4 matrix_subtraction(mat4 m1, mat4 m2);
mat4 matrix_matrix_mult(mat4 m1, mat4 m2);
mat4 inverse_matrix(mat4 m);
mat4 transpose_matrix(mat4 m);
vec4 matrix_vector_mult(mat4 m, vec4 v);

//sub functions for inverse matrix
mat4 m4_minor(mat4 m);
mat4 m4_cofactor(mat4 m);
float m3_determinant(mat3 m);




#endif
