#include <iostream>


template<typename Derived>
class Shape {
public:
    double area() const {
        return static_cast<const Derived*>(this)->areaImpl();
    }
};

class Circle : public Shape<Circle> {
    double r_;
public:
    Circle(double r) : r_(r) {}
    double areaImpl() const { return 3.14159 * r_ * r_; }
};

class Rectangle : public Shape<Rectangle> {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double areaImpl() const { return w_ * h_; }
};

int main() {
    Circle c(5.0);
    Rectangle r(4.0, 6.0);
    c.describe();   // Area = 78.5398
    r.describe();   // Area = 24
}