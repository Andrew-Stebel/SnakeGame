
#include "Camera.h"

const double Camera::Y_INC    = 1.0/5.0;

Camera::Camera() :
    currEye(),                 // initialize to (0,0,0)
    viewDir(),
    currCoi()
{}

void Camera::resetView()
{
    currEye = Point_E3<double> ( -100.0, 35, 0 );
    viewDir = Vector_E3<double> (1.0, 0.0, .0);

    currCoi = currEye + viewDir;
}

void Camera::increaseY()
{
    currEye = Point_E3<double>(currEye.x(),
                               currEye.y() + Y_INC,
                               currEye.z());
    currCoi = Point_E3<double>(currCoi.x(),
                               currCoi.y() + Y_INC ,
                               currCoi.z());
}

void Camera::decreaseY()
{
    currEye = Point_E3<double>(currEye.x(),
                               currEye.y() - Y_INC,
                               currEye.z());
    currCoi = Point_E3<double>(currCoi.x(),
                               currCoi.y() - Y_INC,
                               currCoi.z());
}

void Camera::update(int ycontrol,
                    int /*W*/, int /*H*/, float seconds)
{
    static float lastSeconds = 0;

    if(lastSeconds == 0) {
        lastSeconds = seconds;
        return;
    }

    float timegap = seconds - lastSeconds;
    lastSeconds = seconds;

    if(ycontrol) {
        double factor = -double(ycontrol) * timegap;
        currEye = currEye + viewDir * factor;
        currCoi = currCoi + viewDir * factor;
        viewDir = currCoi - currEye;	// avoid accumulating errors
    }
}
