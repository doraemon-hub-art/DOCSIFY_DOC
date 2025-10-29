/**
 * @file mock_test.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef MOCK_TEST_HPP
#define MOCK_TEST_HPP

namespace mock_test {

/**
 * @brief A test class.
 * 
 */
class MyClass {
public:
    MyClass(int value);
    int getValue() const;
    void setValue(int value);
    /**
     * @brief calculate two double member
     * 
     * @param x 
     * @param y 
     * @return double 
     */
    double calculate(double x, double y);

private:
    int value_;
};

} // namespace mock_test

#endif // MOCK_TEST_HPP
