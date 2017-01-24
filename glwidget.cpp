/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - redid design of glWidget (renamed from GamePanel in SnakeGame milestone 1)
 *      - using Assignment 4 as a base due to useful functions (e.g. make a cube)
 *      - added camera class
 *      - now uses openGL for all drawing
 */

#include <QtOpenGL>
#include <QtMultimedia>

#include "glwidget.h"
#include "geom_types.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      W(640), H(480),
      ycontrol(0)
{
    //black background
    backgroundColor = QColor::fromRgb(0, 0, 0);
    gameInSession = false;
    audioPlayer = new QMediaPlayer;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
                         GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                         GLdouble upX, GLdouble upY, GLdouble upZ)
{
    Direction_E3d view(eyeX, eyeY, eyeZ,
                       centerX, centerY, centerZ);
    Direction_E3d up(upX, upY, upZ);
    Direction_E3d s = cross_product(view, up);
    Direction_E3d u = cross_product(s, view);

    GLdouble M[16];             // view matrix
    M[ 0] =     s.x(); M[ 4] =     s.y(); M[ 8] =     s.z(); M[12] = 0;
    M[ 1] =     u.x(); M[ 5] =     u.y(); M[ 9] =     u.z(); M[13] = 0;
    M[ 2] = -view.x(); M[ 6] = -view.y(); M[10] = -view.z(); M[14] = 0;
    M[ 3] =       0.0; M[ 7] =       0.0; M[11] =       0.0; M[15] = 1;

    glMultMatrixd(M);
    glTranslated(-eyeX, -eyeY, -eyeZ);
}


void GLWidget::initializeGL()
{
    qglClearColor(backgroundColor);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
}

void GLWidget::resizeGL(int width, int height)
{
    W = width; H = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); {
        glLoadIdentity();

        if (width <= height)
            glFrustum(-1.0, +1.0,
                      -1.0*(GLdouble)height/(GLdouble)width,
                      +1.0*(GLdouble)height/(GLdouble)width,
                      1.0, 1000.0);
        else
            glFrustum(-1.0*(GLdouble)width/(GLdouble)height,
                      +1.0*(GLdouble)width/(GLdouble)height,
                      -1.0, +1.0,
                      1.0, 1000.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

//-------------------------------------------
//movement of camera and player code
void GLWidget::direction(int d)
{
    if (snakePlayer.getStatus())
    {
        if (d >2)
            d-=2;
        else
            d+=2;
    }
    moveDir = d;
    snakePlayer.changeDirection(moveDir);
}

void GLWidget::resetView()      { camera.resetView();}
void GLWidget::increaseY()      { camera.increaseY(); }
void GLWidget::decreaseY()      { camera.decreaseY(); }
//--------------------------------------------


//---------SHADOW IMPLEMENTATION----------------------------
//generate player and body shadows
void GLWidget::displayShadows(const Point_E3d& L)
{
    doShadowSetup(L);
    snakePlayer.genShadows();
    food.genShadows();
    glPopMatrix();
}


//Implement shadows for the objects
//Accepting the light position (L) as a parameter
//Same method of shadows used in Assignment 4
void GLWidget::doShadowSetup(const Point_E3d& L)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0, 0, 0, 0);

    GLdouble shadowMatrix[16];

    shadowMatrix[ 0] = 1;
    shadowMatrix[ 1] = 0;
    shadowMatrix[ 2] = 0;
    shadowMatrix[ 3] = 0;

    shadowMatrix[ 4] = -L.x()/L.y();
    shadowMatrix[ 5] = 0;
    shadowMatrix[ 6] = -L.z()/L.y();
    shadowMatrix[ 7] = -1.0/L.y();

    shadowMatrix[ 8] = 0;
    shadowMatrix[ 9] = 0;
    shadowMatrix[10] = 1;
    shadowMatrix[11] = 0;

    shadowMatrix[12] = 0;
    shadowMatrix[13] = 0.1;
    shadowMatrix[14] = 0;
    shadowMatrix[15] = 1;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixd(shadowMatrix);
}

//---------------------UPDATE AND PAINT SCENE-----------------
//timer event handles painting the scene every frame
void GLWidget::timerEvent(QTimerEvent * /*event*/)
{
    //repaint
    update();
}

//paint function - draws everything viewed by the user
void GLWidget::paintGL()
{
    if (gameInSession)
    {
        // Handle camera motion. elapsed() returns milliseconds.
        seconds = elapsed_timer.elapsed() / 1000.0;
        camera.update(ycontrol, 640/*W*/, 480/*H*/, seconds);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        gluLookAt(camera.viewer().x(), camera.viewer().y(), camera.viewer().z(),
                  camera.coi().x(), camera.coi().y(), camera.coi().z(),
                  0.0, 1.0, 0.0);

        gameTime();
        //draw the game
        drawScene();
        //check game situations
        checkEvents();
    }
}
//--------------------------------------------------------------



//--------------------GAME FUNCTIONS----------------------------
void GLWidget::startGame()
{
    gameInSession = true;

    //default settings
    points = 0;
    level = 1;
    emit scoreChanged(points);
    emit levelChanged(level);

    //timer sequence
    basic_timer.start(50, this); // interval between drawings
    elapsed_timer.start();       // time since start
    camera.resetView(); // initialize eye/coi/up vectors
    timer = 51;

    //game system variables
    foodCount = 0;
    moveDir = 0;

    //randomly place player/food
    snakePlayer = Snake(50,50);
    food = Food(50,50,level);
}

void GLWidget::gameTime()
{
    int curTime = timer - seconds;
    light.updatePosition(seconds, timer);
    emit timeChanged(curTime);

    if ( curTime == 0 )
    {
        gameInSession = false;
    }
}

//verify the player stage in the boarder (50x50)
bool GLWidget::checkBoarder()
{
    if (moveDir == 1 && (snakePlayer.getzPos() < -50))
        return false;
    else if (moveDir == 3 && (snakePlayer.getzPos() > 50))
        return false;
    else if (moveDir == 2 && snakePlayer.getxPos() > 50)
        return false;
    else if (moveDir == 4 && snakePlayer.getxPos() < -50)
        return false;
    else
        return true;
}
//move the snake in required direction
void GLWidget::moveSnake()
{
    snakePlayer.updateBody();

    //move the snake along its inputted path continuously
    if (moveDir == 1)
    {
        snakePlayer.moveOnAxisZ(-3);
    }
    else if (moveDir == 2)
    {
        snakePlayer.moveOnAxisX(3);
    }
    else if (moveDir == 3)
    {
        snakePlayer.moveOnAxisZ(3);
    }
    else if (moveDir == 4)
    {
        snakePlayer.moveOnAxisX(-3) ;
    }
}

void GLWidget::checkEvents()
{
    //handle player events
    if (snakePlayer.collisionCheck())
    {
        gameInSession = false;
    }
    else if (snakePlayer.foundFood(food))
    {
        ateFood();
    }
    if (checkBoarder())
        moveSnake();
}

//increment points and make new food object
void GLWidget::ateFood()
{
    points += 100;
    emit scoreChanged(points);
    foodCount++;

    playAudio();

    if (foodCount > 10)
    {
        nextLevel();
    }
    food = Food(50,50,level);
}

void GLWidget::nextLevel()
{
    level++;
    emit levelChanged(level);
    foodCount=0;
    moveDir=0;

    snakePlayer = Snake(50,50);

    elapsed_timer.restart();
}

//-----------Audio Functions-------------------------------
//used to play the eating noise mp3
void GLWidget::playAudio()
{
    audioPlayer->setMedia(QUrl::fromLocalFile("C:/Users/Andrew/Documents/SnakeGame3/sounds/byte.mp3"));
    audioPlayer->setVolume(50);
    audioPlayer->play();
}

//----------------------DRAWING FUNCTIONS-----------------
void GLWidget::drawScene()
{
    drawFloor();
    snakePlayer.drawSnake();
    food.drawFood(seconds);
    displayShadows(light.getPosition());
    light.lightMotion();
}

void GLWidget::drawFloor()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    GLfloat floorAmbient[] = { 0.6f, 0.3f, 1.0f, 1.0f };
    GLfloat floorSpecular[] = { 0.6f, 0.3f, 1.0f, 1.0f };
    GLfloat floorDiffuse[] = { 0.3f, 0.3f, 1.0f, 1.0f };
    GLfloat floorShininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, floorAmbient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floorSpecular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floorDiffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, floorShininess);

    glNormal3f(0.0, 1.0, 0.0);
    for(double x=-60; x<60; x+=10)
    {
        glBegin(GL_QUAD_STRIP);
        {
            for(double z=-60; z<=60; z+=10)
            {
                glVertex3d(x+10, 0, z);
                glVertex3d(x, 0, z);
            }
        } glEnd();
    }
    glEnable(GL_TEXTURE_2D);
}


