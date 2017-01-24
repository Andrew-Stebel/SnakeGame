#ifndef LIGHT
#define LIGHT

#include "geom_types.h"
#include "drawshapes.h"

class Light
{
public:
    Light()
    {

    }

    //light point
    Point_E3d lightPosition;
    Point_E3d getPosition() { return lightPosition;}

    //move the light's position based on the timer
    //the closer to 0 = the closer to sunset
    void updatePosition(qreal seconds, int timer)
    {
        double timeOfDay = (6.28/timer)/2;

        lightPosition = Point_E3d(0,
                                  std::abs(100 *std::sin(seconds* timeOfDay)),
                                  -100 * std::cos(seconds* timeOfDay));
    }

    //handle properties of the light and it's movement
    void lightMotion()
    {
        GLfloat myLightPosition[] = {0,0,0, 1};

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        glPushMatrix();
        {
            glColor3f(1.0,1.0,1.0);
            glTranslatef(lightPosition.x(), lightPosition.y(), lightPosition.z());
            glLightfv(GL_LIGHT0, GL_POSITION, myLightPosition);
            myglutSolidSphere(1,25,25);
        } glPopMatrix();
    }
};
#endif // LIGHT

