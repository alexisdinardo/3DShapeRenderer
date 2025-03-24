OPTIONS = -framework GLUT -framework OpenGL
DEFINES = -D GL_SILENCE_DEPRECATION

LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

OBJDIR = ../mylib
OBJS = $(OBJDIR)/initShader.o $(OBJDIR)/linear_alg.o

project1: project1.c $(OBJS)
	gcc -o project1 project1.c $(OBJS) $(OPTIONS) $(DEFINES)

initShader.o: $(OBJDIR)/initShader.c $(OBJDIR)/initShader.h
	gcc -c $(OBJDIR)/initShader.c

$(OBJDIR)/%.o: %.c %.h
	$(CC) -c @< -o $@ $(OPTIONS) $(DEFINES)
