#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QPushButton;
class QCheckBox;
class GLWidget;
class QLCDNumber;
class QLabel;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    GLWidget *glWidget;

    QLCDNumber *levelLcd;
    QLCDNumber *pointsLcd;
    QLCDNumber *timeLcd;
    QPushButton * startButton;
    QPushButton * resetViewButton;
    QLabel *createLabel(const QString &text);
};

#endif // WINDOW_H
