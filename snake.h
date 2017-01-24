#ifndef Snake_H
#define Snake_H

/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - using openGL now
 *      - new format of positioning
 *      - new format for extending body/validating collisions
 */
#include <QtOpenGL>
#include <ctime>
#include <vector>

#include "body.h"
#include "food.h"
#include "drawshapes.h"

//The snake
//Handles the head of the snake (the player)
class Snake
{
public:
    Snake(){}

    //initialize the head of the snake
    Snake(int maxX, int maxZ)
    {
        srand(time(NULL));

        xPos = rand() % (maxX*2 + 1) - maxX;
        zPos = rand() % (maxZ*2 + 1) - maxZ;
        poisoned = false;
    }
private:
    int xPos;
    int zPos;
    int moveDirection;
    bool poisoned;

    //vector holds snake's body parts
    std::vector<Body> body;
public:
    void changeDirection(int i){ moveDirection = i; }
    int getxPos() {return xPos;}
    int getzPos() {return zPos;}
    bool getStatus() { return poisoned;}

    void moveOnAxisX(int x) { xPos +=x;}
    void moveOnAxisZ(int z) { zPos +=z;}

    bool foundFood(Food food)
    {
        if (abs(food.xPos - this->xPos) < 6 && abs(food.zPos - this->zPos) < 6)
        {
            if (food.type == 2)
                poisoned = true;
            else
                poisoned = false;

            body.push_back(Body());
            return true;
        }
        return false;
    }

    //move all the body parts ahead
    //follow the leader style
    void updateBody()
    {
        Body prevPiece;

        typedef std::vector<Body>::iterator It;
        for(It piece=body.begin(); piece!=body.end(); ++piece)
        {
            if (piece == body.begin())
            {
                prevPiece = *piece;

                piece->xPos = xPos;
                piece->zPos = zPos;
            }
            else
            {
                Body tmp = *piece;
                piece->xPos = prevPiece.xPos;
                piece->zPos = prevPiece.zPos;

                prevPiece = tmp;
            }
        }
    }

    //verify if the player has hit the snake's body at all
    //return true for game over
    bool collisionCheck()
    {
        typedef std::vector<Body>::iterator It;
        for(It piece=body.begin(); piece!=body.end(); ++piece)
        {
            if (abs(piece->xPos - this->xPos) < 3 && abs(piece->zPos - this->zPos) <3)
                return true;
        }
        return false;
    }

    //--------------------------------------
    //SNAKE DISPLAY FUNCTIONS
    void drawSnake()
    {
        drawSnakeBody();
        drawSnakeEyes();

    }

    void drawSnakeBody()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);

        GLfloat snakeAmbient[] = { 0.6f, 0.3f, 1.0f, 1.0f };
        GLfloat snakeSpecular[] = { 0.6f, 0.3f, 1.0f, 1.0f };
        GLfloat snakeDiffuse[] = { 0.3f, 5.3f, 1.0f, 1.0f };
        if (poisoned)
        {
            snakeDiffuse[0] = 0.0f;
            snakeDiffuse[1] = 0.0f;
            snakeDiffuse[2] = 5.3f;
            snakeDiffuse[3] = 1.0f;
        }
        GLfloat snakeShininess[] = { 100.0 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, snakeAmbient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, snakeSpecular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, snakeDiffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, snakeShininess);

        displayGeometry();

        glEnable(GL_TEXTURE_2D);
    }

    //create snake object and body objects
    void displayGeometry()
    {
        glPushMatrix();
        {
            glTranslatef(xPos, 3, zPos);
            glScalef(6,6,6);
            myglutSolidCube(1.0);
        } glPopMatrix();

        //create body parts
        typedef std::vector<Body>::iterator It;
        for(It piece=body.begin(); piece!=body.end(); ++piece)
        {
            glPushMatrix();
            {
                glTranslatef(piece->xPos, 2, piece->zPos);

                if (piece == --(body.end()))
                {
                    glScalef(3,3,3);
                    myglutSolidSphere(1,2,2);
                }
                else
                {
                    glScalef(5,5,5);
                    myglutSolidCube(1.0);
                }
            } glPopMatrix();
        }
    }

    void drawSnakeEyes()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);

        GLfloat snakeAmbient[] = { 0.3f, 0.6f, 1.0f, 1.0f };
        GLfloat snakeSpecular[] = { 0.3f, 0.6f, 1.0f, 1.0f };
        GLfloat snakeDiffuse[] = { 5.3f, 5.3f, 0.0f, 1.0f };
        GLfloat snakeShininess[] = { 100.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, snakeAmbient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, snakeSpecular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, snakeDiffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, snakeShininess);

        displayEyeGeometry();

        glEnable(GL_TEXTURE_2D);
    }

    void displayEyeGeometry()
    {
        glPushMatrix();
        if (moveDirection == 1)
        {
            glTranslatef(xPos-2, 6, zPos-2);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
            glTranslatef(4, 0, 0);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        }
        else if (moveDirection ==2)
        {
            glTranslatef(xPos+2, 6, zPos+2);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
            glTranslatef(0, 0, -4);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        }
        else if (moveDirection == 3)
        {
            glTranslatef(xPos+2, 6, zPos+2);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
            glTranslatef(-4, 0, 0);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        }
        else if (moveDirection == 4)
        {
            glTranslatef(xPos-2, 6, zPos-2);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
            glTranslatef(0, 0, 4);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        }
        else
        {
            glTranslatef(xPos+2, 6, zPos+2);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
            glTranslatef(-4, 0, 0);
            glScalef(1,1,1);
            myglutSolidSphere(2,45,45);
        }
        glPopMatrix();

        drawPupils();
    }


    void drawPupils()
    {
        GLfloat snakeAmbient[] = { 0.6f, 0.3f, 1.0f, 1.0f };
        GLfloat snakeSpecular[] = { 0.6f, 0.3f, 1.0f, 1.0f };
        GLfloat snakeDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat snakeShininess[] = { 100.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, snakeAmbient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, snakeSpecular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, snakeDiffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, snakeShininess);

        glPushMatrix();
        if (moveDirection == 1)
        {
            glTranslatef(xPos-2, 6, zPos-4);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
            glTranslatef(4, 0, 0);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
        }
        else if (moveDirection == 3)
        {
            glTranslatef(xPos+2, 6, zPos+4);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
            glTranslatef(-4, 0, 0);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
        }
        else if (moveDirection == 4)
        {
            glTranslatef(xPos-4, 6, zPos-2);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
            glTranslatef(0, 0, 4);
            glScalef(1,1,1);
            myglutSolidSphere(1,45,45);
        }
        glPopMatrix();
    }

    void genShadows()
    {
        displayGeometry();
        displayEyeGeometry();
    }
};  //snake

#endif
