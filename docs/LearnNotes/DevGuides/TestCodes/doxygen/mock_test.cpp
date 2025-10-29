#include "mock_test.hpp"

namespace mock_test {

MyClass::MyClass(int value) : value_(value) {}

int MyClass::getValue() const {
    return value_;
}

void MyClass::setValue(int value) {
    value_ = value;
}

double MyClass::calculate(double x, double y) {
    return x * y + value_;
}

} // namespace mock_test
