/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - redid design of window (renamed from GameWidget in SnakeGame milestone 1)
 *      - using Assignment 4 as a base due to useful functions (e.g. make a cube)
 *      - now uses openGL for all drawing
 *      - inputs are all handled from the window, rather in the game
 */


#include <QtWidgets>
#include <QPushButton>
#include <QCheckBox>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidget = new GLWidget;
    levelLcd = new QLCDNumber(5);
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    pointsLcd = new QLCDNumber(5);
    pointsLcd->setSegmentStyle(QLCDNumber::Filled);
    timeLcd = new QLCDNumber(2);
    timeLcd->setSegmentStyle(QLCDNumber::Filled);

    startButton = new QPushButton(tr("Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    resetViewButton = new QPushButton(tr("Reset View"));
    resetViewButton->setFocusPolicy(Qt::NoFocus);

    //connections
    connect(startButton, SIGNAL(clicked()), glWidget, SLOT(startGame()));
    connect(glWidget, SIGNAL(levelChanged(int)), levelLcd, SLOT(display(int)));
    connect(glWidget, SIGNAL(scoreChanged(int)), pointsLcd, SLOT(display(int)));
    connect(glWidget, SIGNAL(timeChanged(int)), timeLcd, SLOT(display(int)));
    connect(resetViewButton, SIGNAL(clicked()), glWidget, SLOT(resetView()));


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(createLabel(tr("LEVEL")),0,0);
    layout->addWidget(levelLcd,1,0);
    layout->addWidget(createLabel(tr("SCORE")),0,1);
    layout->addWidget(pointsLcd,1,1);
    layout->addWidget(createLabel(tr("TIME")),0,2);
    layout->addWidget(timeLcd,1,2);

    layout->addWidget(glWidget,3,0,1,3);
    layout->addWidget(startButton,4,0);
    layout->addWidget(resetViewButton,4,2);
    setLayout(layout);

    setWindowTitle(tr("Snakes on a Game"));
}

//handles the input from the user
// z|x move the camera
//arrows move the snake
void Window::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
    case Qt::Key_Z:
        glWidget->increaseY();
        break;
    case Qt::Key_X:
        glWidget->decreaseY();
        break;
    case Qt::Key_Left:
        glWidget->direction(1);
        break;
    case Qt::Key_Up:
    {
        glWidget->direction(2);
        break;
    }
    case Qt::Key_Right:
        glWidget->direction(3);
        break;
    case Qt::Key_Down:
        glWidget->direction(4);
    default:
        QWidget::keyPressEvent(event);
    }
}



QLabel *Window::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
