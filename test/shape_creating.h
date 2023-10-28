// test.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>

struct Point 
{
    double x{}, y{}, z{};
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{   
    return os << "point{ " << p.x << ", " << p.y << ", " << p.z << " }";
}

struct Vector 
{
    double x{}, y{}, z{};
};

std::ostream& operator<<(std::ostream& os, const Vector& v)
{   
    return os << "vector{ " << v.x << ", " << v.y << ", " << v.z << " }";
}

class Shape   
{
protected:
    Shape() = default;

public:
    virtual ~Shape() {}

    virtual Point curvePoint(double t) const = 0;    
    virtual Vector curveVector(double t) const = 0;  
};

class Circle : public Shape 
{
    double radiusCircle{};
    const Point supportPointCir;

public:
    Circle() = default;
    Circle(const Point& point, double radius)
        : supportPointCir(point), radiusCircle{ radius } {}

    virtual ~Circle() {}

    double radius() const 
    {
        return radiusCircle;
    }

    virtual Point curvePoint(double t) const override
    {
        return Point
        {
            radiusCircle * std::cos(t) + supportPointCir.x,
            radiusCircle * std::sin(t) + supportPointCir.y,
            supportPointCir.z
        };
    }
    virtual Vector curveVector(double t) const override
    {
        return Vector
        {
            -1 * radiusCircle * std::sin(t),
            radiusCircle * std::cos(t)
        };
    }
};

class Ellipse : public Shape   
{
    double radiusX{};
    double radiusY{};
    const Point supportPointEll;

public:
    Ellipse() = default;
    Ellipse(const Point& point, double radius_x, double radius_y)
        : supportPointEll{ point }, radiusX{ radius_x }, radiusY{ radius_y } {}

    virtual ~Ellipse() {}

    virtual Point curvePoint(double t) const override
    {
        return Point
        {
            radiusX * std::cos(t) + supportPointEll.x,
            radiusY * std::sin(t) + supportPointEll.y,
            supportPointEll.z
        };
    }
    virtual Vector curveVector(double t) const override
    {
        return Vector
        {
            -1 * radiusX * std::sin(t),
            radiusY * std::cos(t)
        };
    }
};

class Helix : public Shape  
{
    double radiusHel{};
    double stepZ{};
    const Point supportPointHel;

public:
    Helix() = default;
    Helix(const Point& point, double radius, double step)
        : supportPointHel{ point }, radiusHel{ radius }, stepZ{ step } {}

    virtual ~Helix() {}

    virtual Point curvePoint(double t) const override
    {
        return Point
        {
            radiusHel * t * std::cos(t) + supportPointHel.x,
            radiusHel * t * std::sin(t) + supportPointHel.y,
            stepZ * t + supportPointHel.z
        };
    }
    virtual Vector curveVector(double t) const override
    {
        return Vector
        {
            radiusHel * (std::cos(t) - t * std::sin(t)),
            radiusHel * (std::sin(t) + t * std::cos(t)),
            stepZ
        };
    }
};

