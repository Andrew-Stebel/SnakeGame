#ifndef DRAWSHAPES
#define DRAWSHAPES

#include <QtOpenGL>

void myglutSolidCube(GLdouble size);
void myglutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
void drawBox(GLfloat size, GLenum type);

#endif // DRAWSHAPES

