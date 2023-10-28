// test.cpp: определяет точку входа для приложения.
//
#include <iostream>
#include "shape_creating.h"
#include <tbb/parallel_for.h>
#include <random>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <algorithm>

using PtrShape = std::shared_ptr<Shape>; 
using PtrCircle = std::shared_ptr<Circle>;  
constexpr double eps = 1e-6;
constexpr size_t n = 10;   
const double PI = std::acos(-1);    
bool typeC{}, typeE{}, typeH{};
static std::random_device rd;

class ShapeGenerator  
{
    enum shapeType 
    {
        circleType, ellipseType, helixType
    };

    std::uniform_real_distribution<double> udd{ -1, 1 };    
    std::uniform_int_distribution<> usd{ circleType, helixType };   

public:

    PtrShape circle()
    {   
        double radius{};
        while (radius < eps) 
        {
            radius = std::abs(udd(rd)); 
        }

        return std::make_shared<Circle>(Point{ udd(rd), udd(rd) }, radius);
    }

    PtrShape ellipse()
    {   
        double radius_x{};
        while (radius_x < eps) 
        {
            radius_x = std::abs(udd(rd));   
        }

        double radius_y{};
        while (radius_y < eps)
        {
            radius_y = std::abs(udd(rd));   
        }

        return std::make_shared<Ellipse>(Point{ udd(rd), udd(rd) }, radius_x, radius_y);
    }

    PtrShape helix()
    {   
        double radius{};
        while (radius < eps)
        {
            radius = std::abs(udd(rd)); 
        }

        double step{};
        while (std::abs(step) < eps) 
        {
            step = udd(rd);
        }

        return std::make_shared<Helix>(Point{ udd(rd), udd(rd), udd(rd) }, radius, step);
    }

    PtrShape random_shape()
    {  

        switch (usd(rd))
        {
        case circleType:
            typeC = true;
            return circle();

        case ellipseType:
            typeE = true;
            return ellipse();
        case helixType:
            typeH = true;
            return helix();
        default:
            return circle(); 
        }
    }
};

int main()
{
    std::vector<PtrShape> shapeVec;  

    ShapeGenerator shapeGen; 
    
    do
    {
        for (size_t i{}; i < n; ++i)
        {
            shapeVec.emplace_back(shapeGen.random_shape());   
        }
    } while (!typeC  && !typeE  && !typeH);
    

    std::vector<PtrCircle> circleVec;    
    double radiusSum{};   



    for (auto& ptrType : shapeVec)
    {
        const std::type_info& tInfo{ typeid(*ptrType) };    

        std::cout << tInfo.name() << ' ' << ptrType->curvePoint(PI / 4) << ' '  
                  << ptrType->curveVector(PI / 4) << std::endl;   

        if (tInfo.hash_code() == typeid(Circle).hash_code()) 
        {
            circleVec.emplace_back(std::dynamic_pointer_cast<Circle>(ptrType));             
        }
        std::cout << std::endl;
    }

    tbb::parallel_for(tbb::blocked_range<size_t>(0, circleVec.size()),
        [&](const tbb::blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i < r.end(); ++i) {
                radiusSum += circleVec[i]->radius();
            }
        });
    std::sort(circleVec.begin(), circleVec.end(),
        [](const auto& l, const auto& r)  
        {
            return l->radius() < r->radius();
        });

    for (auto& ptrCircle : circleVec)
    {
        std::cout << ptrCircle->radius() << std::endl;
    }

    std::cout << "sum = " << radiusSum << std::endl;   
}
