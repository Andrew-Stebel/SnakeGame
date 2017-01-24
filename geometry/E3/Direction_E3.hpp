#ifndef DIRECTION_E3_HPP
#define DIRECTION_E3_HPP

#include <cmath>

template<typename number_type>
inline
number_type
determinant(const number_type & a, const number_type & b,
            const number_type & c, const number_type & d)
{
    return a * d - b * c;
}

template<typename number_type>
class Direction_E3
{
public:
    Direction_E3(number_type x, number_type y, number_type z)
        : _x(x), _y(y), _z(z)
    {
        normalize();
    }
    Direction_E3(number_type fromX, number_type fromY, number_type fromZ,
                 number_type toX, number_type toY, number_type toZ)
    {
        _x = toX - fromX;
        _y = toY - fromY;
        _z = toZ - fromZ;
        normalize();
    }
    number_type x() const { return _x; }
    number_type y() const { return _y; }
    number_type z() const { return _z; }

private:
    void normalize()
    {
        number_type d = _x*_x + _y*_y + _z*_z;
        d = std::sqrt(d);
        _x /= d;
        _y /= d;
        _z /= d;
    }
    
    number_type _x, _y, _z;
};

template<class number_type>
Direction_E3<number_type>
cross_product(const Direction_E3<number_type>& v1,
              const Direction_E3<number_type>& v2)
{
    typedef Direction_E3<number_type> Direction_E3NT;
    return Direction_E3NT(+ determinant(v1.y(), v2.y(), v1.z(), v2.z()),
                          - determinant(v1.x(), v2.x(), v1.z(), v2.z()),
                          + determinant(v1.x(), v2.x(), v1.y(), v2.y()));
}

#endif // DIRECTION_E3_HPP
