/*
 * template.c
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
#include "../mylib/initShader.h"
#include "../mylib/linear_alg.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

/*typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

typedef struct
{
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;*/

mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 result = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	result.w.x = x;
	result.w.y = y;
	result.w.z = z;
	return result;
}

GLuint ctm_location;
mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLfloat x_value = 0;
int isGoingRight = 1;

vec4 vertices[6] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 { 0.5,  0.8, -0.5, 1.0},	// top
 { 0.9,  0.0, -0.5, 1.0},	// bottom right
 { 0.1,  0.0, -0.5, 1.0}};	// bottom left

vec4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0}};	// green (for bottom right)

int num_vertices = 6;

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

	ctm_location = glGetUniformLocation(program, "ctm");

	printf("ctm_location:%i\n", ctm_location);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}

void idle(void)
{
	if(isGoingRight)
	{
		x_value += 0.005;
		if(x_value > 1.0)
		{
			x_value = 1.0;
			isGoingRight = 0;
		}
	}
	else
	{
		x_value -= 0.005;
		if(x_value < -1.0)
		{
			x_value = -1.0;
			isGoingRight = 1;
		}
	}

	ctm = translate(x_value, 0.0, 0.0);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Triangle");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
