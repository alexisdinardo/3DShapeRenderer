/*
 * project1.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../mylib/initShader.h"
#include "../mylib/linear_alg.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


//GLOBALS

bool isSphere = false; //to say which shape is being displayed
bool isTorus = false;
bool isSpring = false;
bool isFile = false;

int num = 0;
int num_vertices; //global to store variables

vec4 *colors;
vec4 *vertices;
char *filename;

float scaleFactor = 1.0;
GLfloat rotationAngle = 0.0;

GLuint ctm_location; //initialize the ctm
mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

// Mouse variables
bool isRotating = false;
int prevMouseX = 0;
int prevMouseY = 0;

mat4 trackballRotation = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

vec4 *circle(int *num) {    //circle function as explained in class
    int numTriangles = 360 / 10;
    int numPositions = numTriangles * 3;
    
    vec4 *result = (vec4 *) malloc(sizeof(vec4) * numPositions);
    
    int i;
    
    int v_index = 0;
    
    for(i = 0; i < 36; i++){
        float firstAngle = i * 10;
        float secondAngle = (i * 10) + 10;
        float firstRadian = firstAngle * M_PI / 180.0;
        float secondRadian = secondAngle * M_PI / 180.0;
        
        result[v_index] = (vec4) {0, 0, 0, 1};
        v_index++;
        result[v_index]  = (vec4) {sin(secondRadian), cos(secondRadian), 0, 1};
        v_index++;
        result[v_index]  = (vec4) {sin(firstRadian), cos(firstRadian), 0, 1};
        v_index++;
    }
    
    *num = numPositions;
    return result;
}


vec4 *cone(int *num){     //function to create the cone
    float radius = 0.5;
    float height = 1;
    int numVertices = 36 * 6 * 4; // 2 triangles per segment (3 vertices per triangle)

    vec4 *result = (vec4 *)malloc(sizeof(vec4) * numVertices);
    
    int v_index = 0;

    //make the circle
    for (int i = 0; i < 36; i++) {
        float firstAngle = i * (360.0 / 36);
        float secondAngle = (i + 1) * (360.0 / 36);
        float firstRadian = firstAngle * M_PI / 180.0;
        float secondRadian = secondAngle * M_PI / 180.0;

        result[v_index] = (vec4){0, 0, 0, 1};
        v_index++;
        result[v_index] = (vec4){radius * sin(secondRadian), 0, radius * cos(secondRadian), 1};   //calculate the vertices based on the radius of the base circle
        v_index++;
        result[v_index] = (vec4){radius * sin(firstRadian), 0, radius * cos(firstRadian), 1};
        v_index++;
    }

    //make the side triangles of the cone
    for (int i = 0; i < 52; i++) { //create two triangles for each triangle in the circle to make it smoother
        float firstAngle = i * (360 / 52);
        float secondAngle = (i + 1) * (360 / 52);
        float firstRadian = firstAngle * M_PI / 180.0;
        float secondRadian = secondAngle * M_PI / 180.0;

        result[v_index] = (vec4){0, 0, 0, 1};
        v_index++;
        result[v_index] = (vec4){radius * sin(firstRadian), 0, radius * cos(firstRadian), 1};
        v_index++;
        result[v_index] = (vec4){radius * sin(secondRadian), 0, radius * cos(secondRadian), 1};
        v_index++;

        result[v_index] = (vec4){0, height, 0, 1}; //set the top vertices according to the height
        v_index++;
        result[v_index] = (vec4){radius * sin(firstRadian), 0, radius * cos(firstRadian), 1};  //calculate the bottom vertices based on the radius of the base circle
        v_index++;
        result[v_index] = (vec4){radius * sin(secondRadian), 0, radius * cos(secondRadian), 1};
        v_index++;
    }
    
    *num = numVertices;
    return result;

}


vec4 *sphere(int *num){
    int bands = 18;
    int rectangles = 36;
    float radius = 0.75;
    int numVertices = rectangles * bands * 2 * 3;  // 2 triangles per segment (w 3 vertices per triangle)
   
    vec4 *result = (vec4 *)malloc(sizeof(vec4) * numVertices);

    int v_index = 0;

    for (int i = 0; i < bands; i++) { //for each band of rectangles in the sphere
        
        float horiz1 = (i) * (M_PI / bands); //find the horizontal angles
        float horiz2 = (i + 1) * (M_PI / bands);

        for (int j = 0; j < rectangles; j++) { //for each slice of consecutive rectangles in current band
            
            float vert1 = j * (2 * M_PI / rectangles); //find vertical angles
            float vert2 = (j + 1) * (2 * M_PI / rectangles);
            
            //build rectangle
            
            //first triangle vertices
            result[v_index] = (vec4){radius * sin(horiz1) * cos(vert1), radius * sin(horiz1) * sin(vert1), radius * cos(horiz1), 1.0};
            v_index++;
            result[v_index] = (vec4){radius * sin(horiz2) * cos(vert1), radius * sin(horiz2) * sin(vert1), radius * cos(horiz2), 1.0}; //top of triangle
            v_index++;
            result[v_index] = (vec4){radius * sin(horiz1) * cos(vert2), radius * sin(horiz1) * sin(vert2), radius * cos(horiz1), 1.0};
            v_index++;

            //second triangle vertices
            result[v_index] = (vec4){radius * sin(horiz1) * cos(vert2), radius * sin(horiz1) * sin(vert2), radius * cos(horiz1), 1.0}; //bottom of triange
            v_index++;
            result[v_index] = (vec4){radius * sin(horiz2) * cos(vert1), radius * sin(horiz2) * sin(vert1), radius * cos(horiz2), 1.0};
            v_index++;
            result[v_index] = (vec4){radius * sin(horiz2) * cos(vert2), radius * sin(horiz2) * sin(vert2), radius * cos(horiz2), 1.0};
            v_index++;
        }
    }

    *num = numVertices;
    return result;
}


vec4 *torus(int *num){
    
    int bands = 36;
    int rectangles = 18;
    float radius1 = 0.5;
    float radius2 = 0.25; // Radius of the tube
    int numVertices = rectangles * bands * 2 * 3; // 2 triangles per segment with 3 vertices per triangle

    vec4 *result = (vec4 *)malloc(sizeof(vec4) * numVertices);

    int v_index = 0;

    for (int i = 0; i < bands; i++) { // for each band of rectangles in the torus
        float horiz1 = i * (2.0 * M_PI / bands); // find the horizontal angles
        float horiz2 = (i + 1) * (2.0 * M_PI / bands);

        for (int j = 0; j < rectangles; j++) { // for each slice of consecutive rectangles
            float vert1 = j * (2.0 * M_PI / rectangles); // find vertical angles
            float vert2 = (j + 1) * (2.0 * M_PI / rectangles);

            //build a rectangle

            //build first triangle
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz1), (radius1 + radius2 * sin(vert1)) * sin(horiz1), radius2 * cos(vert1), 1.0};
            v_index++;
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz1), (radius1 + radius2 * sin(vert2)) * sin(horiz1), radius2 * cos(vert2), 1.0};
            v_index++;
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz2), (radius1 + radius2 * sin(vert1)) * sin(horiz2), radius2 * cos(vert1), 1.0};
            v_index++;

            //build second triangle
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz2), (radius1 + radius2 * sin(vert1)) * sin(horiz2), radius2 * cos(vert1), 1.0};
            v_index++;
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz1), (radius1 + radius2 * sin(vert2)) * sin(horiz1), radius2 * cos(vert2), 1.0};
            v_index++;
            result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz2), (radius1 + radius2 * sin(vert2)) * sin(horiz2), radius2 * cos(vert2), 1.0};
            v_index++;
        }
    }

    *num = numVertices;
    return result;

}


vec4 *spring(int *num){
   
    int bands = 18;
    int rectangles = 18;
    int coils = 5; //how many coils in the spring
    float radius1 = 0.6;
    float radius2 = 0.055;
    float verticalGap = 0.35; //space between coils

    int totalbands = bands * coils;
    int totalrectangles = rectangles;

    int numVertices = totalbands * totalrectangles * 2 * 3 * coils; // 2 triangles per segment, 3 vertices per triangle, multiplied by the number of coils

    vec4* result = (vec4*)malloc(sizeof(vec4) * (numVertices));
    int v_index = 0;

    for (int x = 0; x < coils; x++) {
        for (int i = 0; i < totalbands; i++) {
            float horiz1 = i * (2.0 * M_PI / totalbands);
            float horiz2 = (i + 1) * (2.0 * M_PI / totalbands);

            for (int j = 0; j < totalrectangles; j++) {
                float vert1 = j * (2.0 * M_PI / totalrectangles);
                float vert2 = (j + 1) * (2.0 * M_PI / totalrectangles);

                float offsetAngle = (float)i / totalbands;
                float verticalOffset = -offsetAngle * verticalGap; //negative so the tube tilts down 
                
                float coilOffset;
                
                if(x < 3){ //calculate the offset so the spring doesn't leave the frame
                    coilOffset = x * 0.35;
                }
                else{
                    coilOffset = -(5-x) * 0.35;
                }

                result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz1), (radius1 + radius2 * sin(vert1)) * sin(horiz1), radius2 * cos(vert1) + verticalOffset + coilOffset, 1.0};
                v_index++;
                result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz1), (radius1 + radius2 * sin(vert2)) * sin(horiz1), radius2 * cos(vert2) + verticalOffset + coilOffset, 1.0};
                v_index++;
                result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz2), (radius1 + radius2 * sin(vert1)) * sin(horiz2), radius2 * cos(vert1) + verticalOffset + coilOffset, 1.0};
                v_index++;

                result[v_index] = (vec4){(radius1 + radius2 * sin(vert1)) * cos(horiz2), (radius1 + radius2 * sin(vert1)) * sin(horiz2), radius2 * cos(vert1) + verticalOffset + coilOffset, 1.0};
                v_index++;
                result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz1), (radius1 + radius2 * sin(vert2)) * sin(horiz1), radius2 * cos(vert2) + verticalOffset + coilOffset, 1.0};
                v_index++;
                result[v_index] = (vec4){(radius1 + radius2 * sin(vert2)) * cos(horiz2),(radius1 + radius2 * sin(vert2)) * sin(horiz2), radius2 * cos(vert2) + verticalOffset + coilOffset, 1.0};
                v_index++;
            }
        }
    }

    *num = numVertices;
    return result;
}


vec4 *genRandomColors(int numTriangles){    //generate a random color for each of the triangles
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * numTriangles * 3);
    int i;
    int v_index = 0;
    
    for(i = 0; i < numTriangles; i++){
        float red = (float) rand() / RAND_MAX;
        float green = (float) rand() / RAND_MAX;
        float blue = (float) rand() / RAND_MAX;
        colors[v_index] = (vec4) {red, green, blue, 1};
        colors[v_index + 1] = (vec4) {red, green, blue, 1};
        colors[v_index + 2] = (vec4) {red, green, blue, 1};
        v_index += 3;
    }
    
    return colors;
}

mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 result = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	result.w.x = x;
	result.w.y = y;
	result.w.z = z;
	return result;
}

mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
    mat4 result = { //apply transformation matrix
        {x, 0.0, 0.0, 0.0},
        {0.0, y, 0.0, 0.0},
        {0.0, 0.0, z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
    return result;
}



mat4 rotate(GLfloat angle) {

    GLfloat radians = angle * M_PI / 180.0;  // convert the angle to radians

    
    mat4 Rx; //create rotation matrices for Rx, Ry, and Rz
    mat4 Ry; 
    mat4 Rz;


    //rotation matrix and calculations from class

    // Rx(−θx)
    Rx.x = (vec4){1.0, 0.0, 0.0, 0.0};
    Rx.y = (vec4){0.0, cos(radians), -sin(radians), 0.0};
    Rx.z = (vec4){0.0, sin(radians), cos(radians), 0.0};
    Rx.w = (vec4){0.0, 0.0, 0.0, 1.0};

    // Ry(θy)
    Ry.x = (vec4){cos(radians), 0.0, sin(radians), 0.0};
    Ry.y = (vec4){0.0, 1.0, 0.0, 0.0};
    Ry.z = (vec4){-sin(radians), 0.0, cos(radians), 0.0};
    Ry.w = (vec4){0.0, 0.0, 0.0, 1.0};

    // Rz(0)
    Rz.x = (vec4){cos(radians), -sin(radians), 0.0, 0.0};
    Rz.y = (vec4){sin(radians), cos(radians), 0.0, 0.0};
    Rz.z = (vec4){0.0, 0.0, 1.0, 0.0};
    Rz.w = (vec4){0.0, 0.0, 0.0, 1.0};

    
    // combine rotation matrices

    mat4 rotationMatrix = matrix_matrix_mult(Rx, Ry);  // Rx(−θx)Ry(θy)
    rotationMatrix = matrix_matrix_mult(Rz, rotationMatrix);  // Rz(45)(Rx(−θx)Ry(θy))
    rotationMatrix = matrix_matrix_mult(Ry, rotationMatrix);  // Ry(−θy)(Rz(45)(Rx(−θx)Ry(θy)))
    rotationMatrix = matrix_matrix_mult(Rx, rotationMatrix);  // Rx(θx)(Ry(−θy)(Rz(0)(Rx(−θx)Ry(θy))))

    return rotationMatrix;
    
    }


/* tried to use these to create the shapes
 
mat4 rotate_y(GLfloat angle)
{
    //apply transformation matrix
    
    //ROTATE ON Y AXIS
    mat4 result = {{cos(angle),0,sin(angle),0},{0,1,0,0},{-sin(angle), 0, cos(angle),0},{0,0,0,1}};
    return result;
}


mat4 rotate_x(GLfloat angle)
{
    //apply transformation matrix
    
    //ROTATE ON X AXIS
    mat4 result = {{1, 0, 0, 0},{0, cos(angle), -sin(angle),0},{0, sin(angle), cos(angle), 0},{0,0,0,1}};
    
    return result;
}


mat4 rotate_z(GLfloat angle)
{
    //apply transformation matrix
    
    //ROTATE ON Z AXIS
    mat4 result = {{cos(angle),-sin(angle),0, 0},{sin(angle),cos(angle),0,0},{0, 0, 1,0},{0,0,0,1}};
    
    return result;
}
 
*/

vec4 *read_file(char *fileName, int *num) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) { //exit if file fails to open 
        fprintf(stderr, "Failed to open the file '%s'.\n", fileName);
        printf("Make sure the file exists in the current directory.\n");
        printf("Current working directory: ");
        fflush(stdout);
        system("pwd");
        
        // Don't exit, just return NULL so we can handle this gracefully
        *num = 0;
        return NULL;
    }
    
    // Print loading message to give user feedback
    printf("Loading file, please wait...\n");
    
    int numVertices;
    int result_scan = fscanf(file, "%d", &numVertices); //scan how many vertices to in the file
    
    if (result_scan != 1 || numVertices <= 0) {
        fprintf(stderr, "Invalid vertex count in file or file format incorrect.\n");
        fclose(file);
        *num = 0;
        return NULL;
    }

    // Print number of vertices to give user feedback
    printf("Preparing to load %d vertices...\n", numVertices);
    
    // Allocate memory in one chunk to avoid fragmentation
    vec4* result = (vec4*)malloc(sizeof(vec4) * numVertices);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed for vertices.\n");
        fclose(file);
        *num = 0;
        return NULL;
    }

    // Use a buffer for faster file reading
    char buffer[256];
    int verticesRead = 0;
    int line = 1; // start at line 1 since we already read the vertex count
    
    // Pre-scale all vertices by a constant factor to avoid repeated matrix operations
    float scale_factor = 0.5;
    
    // Read vertices in batches for better performance
    while (verticesRead < numVertices && fgets(buffer, sizeof(buffer), file) != NULL) {
        line++;
        
        // Skip empty lines
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            continue;
        }
        
        float x, y, z, w;
        if (sscanf(buffer, "%f,%f,%f,%f", &x, &y, &z, &w) == 4) {
            // Apply scaling directly during reading to avoid separate loop
            result[verticesRead].x = x * scale_factor;
            result[verticesRead].y = y * scale_factor;
            result[verticesRead].z = z * scale_factor;
            result[verticesRead].w = w;
            verticesRead++;
            
            // Provide progress feedback for very large files
            if (numVertices > 10000 && verticesRead % 5000 == 0) {
                printf("Loaded %d/%d vertices (%.1f%%)\n", 
                       verticesRead, numVertices, 
                       (float)verticesRead / numVertices * 100);
            }
        } else {
            fprintf(stderr, "Warning: Invalid vertex format at line %d\n", line);
        }
    }

    fclose(file);
    
    if (verticesRead != numVertices) {
        fprintf(stderr, "Warning: Expected %d vertices but read only %d.\n", numVertices, verticesRead);
        // Continue with the vertices we did read
    }
    
    printf("File loaded successfully: %d vertices\n", verticesRead);
    
    *num = verticesRead;
    return result;
}



void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    if(isSphere){  //update the num_vertices variable according to what shape is selected
          int numSpherePositions;
          vertices = sphere(&numSpherePositions);
          colors = genRandomColors(numSpherePositions / 3);
          num_vertices = numSpherePositions;

    }else if(isTorus){
          int numTorusPositions;
          vertices = torus(&numTorusPositions);
          colors = genRandomColors(numTorusPositions / 3);
          num_vertices = numTorusPositions;

    }else if(isSpring){
          int numSpringPositions;
          vertices = spring(&numSpringPositions);
          colors = genRandomColors(numSpringPositions / 3);
          num_vertices = numSpringPositions;

    } else if(isFile){
          int numFilePositions;
          // Use the filename provided by the user
          if (filename == NULL || strlen(filename) == 0) {
              printf("No filename specified. Using default 'menger_sponge.txt'\n");
              vertices = read_file("menger_sponge.txt", &numFilePositions);
          } else {
              vertices = read_file(filename, &numFilePositions);
          }
          
          // Check if file reading failed
          if (vertices == NULL || numFilePositions == 0) {
              printf("File reading failed. Defaulting to sphere.\n");
              isSphere = true;
              isFile = false;
              vertices = sphere(&numFilePositions);
          }
          
          colors = genRandomColors(numFilePositions / 3);
          num_vertices = numFilePositions;
    }
    

    //initialize the view

    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices * 2, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
        
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	ctm_location = glGetUniformLocation(program, "ctm");

	//printf("ctm_location:%i\n", ctm_location);

    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}


void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);


  	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices); //draw the current shape
  
    glutSwapBuffers();
}


void keyboard(unsigned char key, int mousex, int mousey){
    printf("%c\n", key);
    
    if(key == 'q'){
        free(vertices);
        free(colors);
        exit(1);}
        
    if(key == 'r'){
        // Reset transformation matrix and scale factor
        ctm = (mat4){{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        scaleFactor = 1.0;
        glutPostRedisplay();
    }

    if(key == '1'){
        isSphere = true; //adjust the variables based on what shape is selected
        isTorus = false;
        isSpring = false;
        isFile = false;
        init();
        glutPostRedisplay();
    }

    if(key == '2'){
        isTorus = true;
        isSphere = false;
        isSpring = false;
        isFile = false;
        init();
        glutPostRedisplay();
    }

    if(key == '3'){
        isSpring = true;
        isSphere = false;
        isTorus = false;
        isFile = false;
        init();
        glutPostRedisplay();
    }

    if (key == '4') {
        isSpring = false;
        isSphere = false;
        isTorus = false;
        isFile = true;
        
        // Prompt user for filename
        printf("Enter the filename (e.g., menger_sponge.txt): ");
        if (filename == NULL) {
            filename = (char*)malloc(256); // Allocate space for filename
        }
        scanf("%255s", filename);
        printf("Loading file: %s\n", filename);
        printf("Note: File format should be:\n");
        printf("  - First line: number of vertices (e.g. 100)\n");
        printf("  - Each subsequent line: x,y,z,w coordinates (e.g. 0.5,0.3,0.1,1.0)\n");
        
        init();
        glutPostRedisplay();
    }
    
   // glutPostRedisplay();
}


void specialKeyboard(int key, int x, int y)
{  
    float scaleChange = 1.0;
    
    if (key == GLUT_KEY_UP) { // up arrow key
        scaleChange = 1.05; // zoom in by 5%
    } else if (key == GLUT_KEY_DOWN) { // down arrow key
        scaleChange = 0.95; // zoom out by 5%
    }
    
    // Update scaleFactor but keep it within bounds
    scaleFactor *= scaleChange;
    
    if (scaleFactor < 0.1) { //so the zoom doesn't go out of range
        scaleFactor = 0.1; // min
    } else if (scaleFactor > 5.0) {
        scaleFactor = 5.0; // max
    }
   
    // Create a scaling matrix for this change only
    mat4 scalingMatrix = scale(scaleChange, scaleChange, scaleChange);
    
    // Apply scaling to the current ctm
    ctm = matrix_matrix_mult(scalingMatrix, ctm);

    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
   
    if (button == GLUT_LEFT_BUTTON) { //making sure correct button is down
        
        if (state == GLUT_DOWN) { //if button is clicked
            isRotating = true; //update the rotation variable
            prevMouseX = x;    // Initialize the previous position to current position
            prevMouseY = y;    // when the button is first pressed
        } else if (state == GLUT_UP) { 
            isRotating = false;
        }
    }
}



void motion(int x, int y){  

    if (isRotating) { //if rotating variable is true (updated in mouse function)
    
        // Calculate the change in mouse position
        int changeX = x - prevMouseX;
        int changeY = y - prevMouseY;
        
        // Scale down the mouse movement to reduce sensitivity
        float sensitivity = 0.5;
        float angle_x = changeX * 0.01 * sensitivity;
        float angle_y = changeY * 0.01 * sensitivity;
        
        // Create rotation matrices for X and Y axes
        mat4 rotationY = {{cos(angle_x),0,sin(angle_x),0},{0,1,0,0},{-sin(angle_x),0,cos(angle_x),0},{0,0,0,1}};
        mat4 rotationX = {{1,0,0,0},{0,cos(angle_y),-sin(angle_y),0},{0,sin(angle_y),cos(angle_y),0},{0,0,0,1}};
        
        // Combine rotations
        mat4 rotation = matrix_matrix_mult(rotationY, rotationX);
        
        // Apply to CTM
        ctm = matrix_matrix_mult(rotation, ctm);

        // Update previous mouse position
        prevMouseX = x;
        prevMouseY = y;

        // Trigger redisplay
        glutPostRedisplay();
    }
}



void idle(void)
{
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
    printf("Choose one of the following: \n 1: Sphere \n 2: Torus \n 3: Spring \n 4: File \n");
    
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(specialKeyboard);
   // glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
