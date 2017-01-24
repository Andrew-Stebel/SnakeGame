#ifndef CAMERA_HPP
#define CAMERA_HPP

/*Program created by: Andrew Stebel
 *Date of last Modification: Oct 27, 2015
 *Concept: Snake Game
 * New Implementations:
 *      - added camera class
 *          - using Assignment 4's camera class as a template
 */

#include "geom_types.h"

//Base class used from Assignment 4 - Shadow implementation
class Camera {
public:
    Camera();

    const Point_E3<double>&  viewer()  const { return currEye; }
    const Point_E3<double>&  coi()     const { return currCoi; }

    /// This is convenient if we "get lost."
    void resetView();

    void update(int ycontrol, int, int W, float seconds);

    void increaseY();
    void decreaseY();
protected:
    // Y increment/decrement by keys 'z'/'x'
    static const double Y_INC;

    Point_E3<double>  currEye;
    Vector_E3<double> viewDir;
    Point_E3<double>  currCoi;

};

#endif // CAMERA_HPP
