#ifndef BODY
#define BODY
/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - using openGL now
 *      - new format of positioning
 */

//pieces of the snake's body
//simple object to hold position
class Body
{
public:
    Body(){}
    Body(int X, int Z)
    {
        xPos = X;
        zPos = Z;
    }

    int xPos;
    int zPos;
};

#endif // BODY

