#include "linear_alg.h"
#include <stdio.h>
#include <math.h>


//vector operations
void print_v4(vec4 v){
    printf("[%.4f, %.4f, %.4f, %.4f]\n", v.x, v.y, v.z, v.w);
}


vec4 scalar_vector_mult(float s, vec4 v){
    vec4 newV = {0, 0, 0, 0};

    newV.x = s * v.x;
    newV.y = s * v.y;
    newV.z = s * v.z;
    newV.w = s * v.w;

    return newV;
}


vec4 vector_addition(vec4 v1, vec4 v2){
    vec4 newV = {0, 0, 0, 0};

    //add each individual element together then put into a new vector
    newV.x = v1.x + v2.x;
    newV.y = v1.y + v2.y;
    newV.z = v1.z + v2.z;
    newV.w = v1.w + v2.w;

    return newV;
}


vec4 vector_subtraction(vec4 v1, vec4 v2){
    vec4 newV = {0, 0, 0, 0};

    //subtract each individual element of the two vectors then put into a new vector
    newV.x = v1.x - v2.x;
    newV.y = v1.y - v2.y;
    newV.z = v1.z - v2.z;
    newV.w = v1.w - v2.w;

    return newV;
}


float magnitude_vector(vec4 v){
    float magnitude = 0;

    //take the square root of the sum of each element squared
    magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));

    //absolute value
    if(magnitude < 0)
    {
        magnitude = magnitude * -1;
    }

    return magnitude;
}


vec4 normalize(vec4 v){
    vec4 norm_vector = {0, 0, 0, 0};

    //forumula for normalizing vector: (1/magnitude of element) * element
    norm_vector.x = (1 / (magnitude_vector(v))) * v.x;
    norm_vector.y = (1 / (magnitude_vector(v))) * v.y;
    norm_vector.z = (1 / (magnitude_vector(v))) * v.z;
    norm_vector.w = (1 / (magnitude_vector(v))) * v.w;

    return norm_vector;
}


float dot_product(vec4 v1, vec4 v2){
    float dot_product = 0;

    //multiply each corresponding element together and add all together
    dot_product = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);

    return dot_product;
}


vec4 cross_product(vec4 v1, vec4 v2){
    vec4 cross_product = {0, 0, 0, 0};

    cross_product.x = (v1.y * v2.z) - (v1.z * v2.y);
    cross_product.y = (v1.z * v2.x) - (v1.x * v2.z);
    cross_product.z = (v1.x * v2.y) - (v1.y * v2.x);
    cross_product.w = 0;

    return cross_product;
}



//matrix operations

void print_m4(mat4 m){
    print_v4(m.x);
    print_v4(m.y);
    print_v4(m.z);
    print_v4(m.w);

}


mat4 scalar_matrix_mult(float s, mat4 m){
    mat4 newM = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    //multiply each element by the scalar value using the scalar vector function
    newM.x = scalar_vector_mult(s, m.x);
    newM.y = scalar_vector_mult(s, m.y);
    newM.z = scalar_vector_mult(s, m.z);
    newM.w = scalar_vector_mult(s, m.w);

    return newM;
}


mat4 matrix_addition(mat4 m1, mat4 m2){
    mat4 newM = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    //use the vector addition function to add together vectors within the matrices
    newM.x = vector_addition(m1.x, m2.x);
    newM.y = vector_addition(m1.y, m2.y);
    newM.z = vector_addition(m1.z, m2.z);
    newM.w = vector_addition(m1.w, m2.w);

    return newM;
}


mat4 matrix_subtraction(mat4 m1, mat4 m2){
    mat4 newM = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0 ,0 ,0 ,0}};

    //use the vector subtraction function to subtract vectors within the matrices
    newM.x = vector_subtraction(m1.x, m2.x);
    newM.y = vector_subtraction(m1.y, m2.y);
    newM.z = vector_subtraction(m1.z, m2.z);
    newM.w = vector_subtraction(m1.w, m2.w);

    return newM;
}


mat4 matrix_matrix_mult(mat4 m1, mat4 m2){
    mat4 newM = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0 ,0 ,0 ,0}, {0 ,0 ,0 ,0}};

    //multiply the first row of the first matrix by the first column of the second matrix
    newM.x.x = (m1.x.x * m2.x.x) + (m1.x.y * m2.y.x) + (m1.x.z * m2.z.x) + (m1.x.w * m2.w.x);
    newM.x.y = (m1.x.x * m2.x.y) + (m1.x.y * m2.y.y) + (m1.x.z * m2.z.y) + (m1.x.w * m2.w.y);
    newM.x.z = (m1.x.x * m2.x.z) + (m1.x.y * m2.y.z) + (m1.x.z * m2.z.z) + (m1.x.w * m2.w.z);
    newM.x.w = (m1.x.x * m2.x.w) + (m1.x.y * m2.y.w) + (m1.x.z * m2.z.w) + (m1.x.w * m2.w.w);
    newM.y.x = (m1.y.x * m2.x.x) + (m1.y.y * m2.y.x) + (m1.y.z * m2.z.x) + (m1.y.w * m2.w.x);
    newM.y.y = (m1.y.x * m2.x.y) + (m1.y.y * m2.y.y) + (m1.y.z * m2.z.y) + (m1.y.w * m2.w.y);
    newM.y.z = (m1.y.x * m2.x.z) + (m1.y.y * m2.y.z) + (m1.y.z * m2.z.z) + (m1.y.w * m2.w.z);
    newM.y.w = (m1.y.x * m2.x.w) + (m1.y.y * m2.y.w) + (m1.y.z * m2.z.w) + (m1.y.w * m2.w.w);
    newM.z.x = (m1.z.x * m2.x.x) + (m1.z.y * m2.y.x) + (m1.z.z * m2.z.x) + (m1.z.w * m2.w.x);
    newM.z.y = (m1.z.x * m2.x.y) + (m1.z.y * m2.y.y) + (m1.z.z * m2.z.y) + (m1.z.w * m2.w.y);
    newM.z.z = (m1.z.x * m2.x.z) + (m1.z.y * m2.y.z) + (m1.z.z * m2.z.z) + (m1.z.w * m2.w.z);
    newM.z.w = (m1.z.x * m2.x.w) + (m1.z.y * m2.y.w) + (m1.z.z * m2.z.w) + (m1.z.w * m2.w.w);
    newM.w.x = (m1.w.x * m2.x.x) + (m1.w.y * m2.y.x) + (m1.w.z * m2.z.x) + (m1.w.w * m2.w.x);
    newM.w.y = (m1.w.x * m2.x.y) + (m1.w.y * m2.y.y) + (m1.w.z * m2.z.y) + (m1.w.w * m2.w.y);
    newM.w.z = (m1.w.x * m2.x.z) + (m1.w.y * m2.y.z) + (m1.w.z * m2.z.z) + (m1.w.w * m2.w.z);
    newM.w.w = (m1.w.x * m2.x.w) + (m1.w.y * m2.y.w) + (m1.w.z * m2.z.w) + (m1.w.w * m2.w.w);


    return newM;
}


mat4 inverse_matrix(mat4 m){

    //follow the forumula from slides to find the matrix (break down into smaller functions)
    mat4 minor = m4_minor(m);
    mat4 cofactor = m4_cofactor(minor);
    mat4 transpose = transpose_matrix(cofactor);
    float determinant = (m.x.x * minor.x.x) - (m.x.y * minor.x.y) + (m.x.z * minor.x.z) - (m.x.w * minor.x.w);
    mat4 a_inv = scalar_matrix_mult((1/determinant), transpose);


    return a_inv;
}


mat4 transpose_matrix(mat4 m){
    mat4 transposed = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0 ,0 ,0 ,0}, {0 ,0 ,0 ,0}};

    //reverse rows and columns
    transposed.x.x = m.x.x;
    transposed.x.y = m.y.x;
    transposed.x.z = m.z.x;
    transposed.x.w = m.w.x;

    transposed.y.x = m.x.y;
    transposed.y.y = m.y.y;
    transposed.y.z = m.z.y;
    transposed.y.w = m.w.y;

    transposed.z.x = m.x.z;
    transposed.z.y = m.y.z;
    transposed.z.z = m.z.z;
    transposed.z.w = m.w.z;

    transposed.w.x = m.x.w;
    transposed.w.y = m.y.w;
    transposed.w.z = m.z.w;
    transposed.w.w = m.w.w;

    return transposed;
}


vec4 matrix_vector_mult(mat4 m, vec4 v){

    vec4 newVec = {0, 0, 0, 0};

    //multiply each element with corresponding element in vector
    newVec.x = (m.x.x * v.x) + (m.x.y * v.y) + (m.x.z * v.z) + (m.x.w * v.w);
    newVec.y = (m.y.x * v.x) + (m.y.y * v.y) + (m.y.z * v.z) + (m.y.w * v.w);
    newVec.z = (m.z.x * v.x) + (m.z.y * v.y) + (m.z.z * v.z) + (m.z.w * v.w);
    newVec.w = (m.w.x * v.x) + (m.w.y * v.y) + (m.w.z * v.z) + (m.w.w * v.w);

    return newVec;
}


//inverse sub-functions
mat4 m4_minor(mat4 m){
    mat4 newM = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};


    //calculate determinant of the 3x3 matrix based on what element is being calculated
    mat3 m1 = {{m.y.y, m.y.z, m.y.w}, {m.z.y, m.z.z, m.z.w}, {m.w.y, m.w.z, m.w.w}};
    newM.x.x = m3_determinant(m1);
    mat3 m2 = {{m.y.x, m.y.z, m.y.w}, {m.z.x, m.z.z, m.z.w}, {m.w.x, m.w.z, m.w.w}};
    newM.x.y = m3_determinant(m2);
    mat3 m3 = {{m.y.x, m.y.y, m.y.w}, {m.z.x, m.z.y, m.z.w}, {m.w.x, m.w.y, m.w.w}};
    newM.x.z = m3_determinant(m3);
    mat3 m4 = {{m.y.x, m.y.y, m.y.z}, {m.z.x, m.z.y, m.z.z}, {m.w.x, m.w.y, m.w.z}};
    newM.x.w = m3_determinant(m4);

    mat3 m5 = {{m.x.y, m.x.z, m.x.w}, {m.z.y, m.z.z, m.z.w}, {m.w.y, m.w.z, m.w.w}};
    newM.y.x = m3_determinant(m5);
    mat3 m6 = {{m.x.x, m.x.z, m.x.w}, {m.z.x, m.z.z, m.z.w}, {m.w.x, m.w.z, m.w.w}};
    newM.y.y = m3_determinant(m6);
    mat3 m7 = {{m.x.x, m.x.y, m.x.w}, {m.z.x, m.z.y, m.z.w}, {m.w.x, m.w.y, m.w.w}};
    newM.y.z = m3_determinant(m7);
    mat3 m8 = {{m.x.x, m.x.y, m.x.z}, {m.z.x, m.z.y, m.z.z}, {m.w.x, m.w.y, m.w.z}};
    newM.y.w = m3_determinant(m8);

    mat3 m9 = {{m.x.y, m.x.z, m.x.w}, {m.y.y, m.y.z, m.y.w}, {m.w.y, m.w.z, m.w.w}};
    newM.z.x = m3_determinant(m9);
    mat3 m10 = {{m.x.x, m.x.z, m.x.w}, {m.y.x, m.y.z, m.y.w}, {m.w.x, m.w.z, m.w.w}};
    newM.z.y = m3_determinant(m10);
    mat3 m11 = {{m.x.x, m.x.y, m.x.w}, {m.y.x, m.y.y, m.y.w}, {m.w.x, m.w.y, m.w.w}};
    newM.z.z = m3_determinant(m11);
    mat3 m12 = {{m.x.x, m.x.y, m.x.z}, {m.y.x, m.y.y, m.y.z}, {m.w.x, m.w.y, m.w.z}};
    newM.z.w = m3_determinant(m12);

    mat3 m13 = {{m.x.y, m.x.z, m.x.w}, {m.y.y, m.y.z, m.y.w}, {m.z.y, m.z.z, m.z.w}};
    newM.w.x = m3_determinant(m13);
    mat3 m14 = {{m.x.x, m.x.z, m.x.w}, {m.y.x, m.y.z, m.y.w}, {m.z.x, m.z.z, m.z.w}};
    newM.w.y = m3_determinant(m14);
    mat3 m15 = {{m.x.x, m.x.y, m.x.w}, {m.y.x, m.y.y, m.y.w}, {m.z.x, m.z.y, m.z.w}};
    newM.w.z = m3_determinant(m15);
    mat3 m16 = {{m.x.x, m.x.y, m.x.z}, {m.y.x, m.y.y, m.y.z}, {m.z.x, m.z.y, m.z.z}};
    newM.w.w = m3_determinant(m16);

    return newM;
}

mat4 m4_cofactor(mat4 m){
    mat4 newM = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};

    //make every other element negative
    newM.x.x = m.x.x;
    newM.x.y = m.x.y * -1;
    newM.x.z = m.x.z;
    newM.x.w = m.x.w * -1;
    newM.y.x = m.y.x * -1;
    newM.y.y = m.y.y ;
    newM.y.z = m.y.z * -1;
    newM.y.w = m.y.w ;
    newM.z.x = m.z.x ;
    newM.z.y = m.z.y * -1;
    newM.z.z = m.z.z ;
    newM.z.w = m.z.w * -1;
    newM.w.x = m.w.x * -1;
    newM.w.y = m.w.y ;
    newM.w.z = m.w.z * -1;
    newM.w.w = m.w.w ;

    return newM;
}

float m3_determinant(mat3 m){
    float determinant = 0;

    //formula for determinant: aei + bfg + cdh - gec - hfa - idb
    determinant = ((m.x.x * m.y.y * m.z.z) + (m.x.y * m.y.z * m.z.x) + (m.x.z * m.y.x * m.z.y)) - ((m.z.x * m.y.y * m.x.z) + (m.z.z * m.x.y * m.y.x) + (m.z.y * m.y.z * m.x.x));

    return determinant;
}
