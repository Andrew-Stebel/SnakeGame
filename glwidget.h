#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <vector>
#include <QGLWidget>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QtMultimedia>

#include "Camera.h"
#include "snake.h"
#include "light.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    //movement of camera and player
    void increaseY();
    void decreaseY();
    void direction(int d);

public slots:
    void startGame();
    void resetView();

signals:
    void scoreChanged(int points);
    void timeChanged(int timer);
    void levelChanged(int level);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
                   GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                   GLdouble upX, GLdouble upY, GLdouble upZ);
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void doShadowSetup(const Point_E3d& L);         //same function implemented in Assignment 4
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;



private:
    GLuint initTexture(const char * imageFile);
    QColor backgroundColor;
    Camera camera;

    int W, H;                   // Window dimensions
    int xcontrol, ycontrol;

    //timers
    QBasicTimer basic_timer;
    QElapsedTimer elapsed_timer;

    //player settings
    Snake snakePlayer;
    Food food;
    Light light;
    int moveDir;

    //game settings
    int points;
    int timer;
    int level;
    qreal seconds;
    int foodCount;
    QMediaPlayer *audioPlayer;
    bool gameInSession;

    //game functions
    void moveSnake();
    void ateFood();
    bool checkBoarder();
    void initializeGame();
    void checkEvents();
    void gameTime();
    void nextLevel();
    void playAudio();

    //visual display functions
    void drawFloor();
    void displayShadows(const Point_E3d& L);
    void drawScene();
};

#endif // GLWIDGET_HPP
