#ifndef Point_E3_HPP
#define Point_E3_HPP

template<typename number_type>
class Point_E3
{
public:
    Point_E3()
        : _x(), _y(), _z()
    {}
    Point_E3(number_type x, number_type y, number_type z)
        : _x(x), _y(y), _z(z)
    {}
    number_type x() const { return _x; }
    number_type y() const { return _y; }
    number_type z() const { return _z; }
private:
    number_type _x, _y, _z;
};

template<typename number_type>
number_type
squared_distance(const Point_E3<number_type>& P,
                 const Point_E3<number_type>& Q)
{
    number_type x = P.x() - Q.x();
    number_type y = P.y() - Q.y();
    number_type z = P.z() - Q.z();
    return x*x + y*y + z*z;
}

#endif // Point_E3_HPP
