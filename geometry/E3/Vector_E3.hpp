#ifndef VECTOR_E3_HPP
#define VECTOR_E3_HPP

#include <cmath>

template<typename number_type>
class Vector_E3
{
public:
    Vector_E3()
        : _x(), _y(), _z()
    {}
    Vector_E3(number_type x, number_type y, number_type z)
        : _x(x), _y(y), _z(z)
    {}
    number_type x() const { return _x; }
    number_type y() const { return _y; }
    number_type z() const { return _z; }

    void rotateY(double d)
    {
        double c = std::cos(d);
        double s = std::sin(d);
        double nx =   _x * c + _z * s;
        double nz = - _x * s + _z * c;
        _x = nx;
        _z = nz;
    }

private:
    number_type _x, _y, _z;
};

template<typename number_type>
Point_E3<number_type>
operator+(const Point_E3<number_type>& p, const Vector_E3<number_type>&  v)
{
    return Point_E3<number_type>(p.x()+v.x(), p.y()+v.y(), p.z()+v.z());
}

template<typename number_type>
Vector_E3<number_type>
operator*(const Vector_E3<number_type> v, const number_type& d)
{
    return Vector_E3<number_type>(v.x()*d, v.y()*d, v.z()*d);
}

template<typename number_type>
Vector_E3<number_type>
operator-(const Point_E3<number_type>& p1, const Point_E3<number_type>& p2)
{
    return Vector_E3<number_type>(p1.x()-p2.x(), p1.y()-p2.y(), p1.z()-p2.z());
}

#endif // VECTOR_E3_HPP
