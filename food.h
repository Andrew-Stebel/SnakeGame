#ifndef FOOD_H
#define FOOD_H

/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - using openGL now
 *      - new format of positioning
 */

#include <stdlib.h>
#include <ctime>
#include "drawshapes.h"

//objects player must get to/eat
class Food
{
public:
    Food(){}

    //randomly place a food on the board
    Food(int maxX, int maxZ, int level)
    {
        srand(rand());

        xPos = rand() % (maxX*2 + 1) - maxX;
        zPos = rand() % (maxZ*2 + 1) - maxZ;
        yPos = 3;

        if (level < 2)
            type = 1;
        else if (level < 6)
            type = rand() % 2 + 1;

    }

    int xPos;
    int zPos;
    int yPos;

    qreal seconds;
    int type;

    void drawFood(qreal seconds)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glDisable(GL_TEXTURE_2D);

        GLfloat foodAmbient[] = { 0.3f, 0.6f, 1.0f, 1.0f };
        GLfloat foodSpecular[] = { 0.3f, 0.6f, 1.0f, 1.0f };
        GLfloat foodDiffuse[] = { 5.3f, 0.3f, 1.0f, 1.0f };
        GLfloat foodShininess[] = { 50.0 };

        if (type == 2)
        {
            foodDiffuse[0] = 0.3f;
            foodDiffuse[1] = 0.3f;
            foodDiffuse[2] = 5.3f;
            foodDiffuse[3] = 1.0f;
        }

        glMaterialfv(GL_FRONT, GL_AMBIENT, foodAmbient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, foodSpecular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, foodDiffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, foodShininess);

        displayGeometry(seconds);
        glEnable(GL_TEXTURE_2D);
    }

    //create food object (cube)
    void displayGeometry(qreal sec)
    {
        seconds = sec;
        glPushMatrix();
        {
            glTranslatef(xPos, std::abs(yPos * std::cos(seconds)), zPos);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        } glPopMatrix();
    }

    void genShadows()
    {
        displayGeometry(seconds);
    }
};

#endif // FOOD

