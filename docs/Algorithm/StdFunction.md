# pow

求次方

> 常用原型

```c++
double pow(double base, double exp);
float  pow(float base, float exp);           // C++11 起
long double pow(long double, long double);
```

> 示例

```c++
#include <iostream>
#include <cmath>  // pow 函数

int main() {
    double base = 2.0;
    double exponent = 3.0;

    double result = std::pow(base, exponent);  // 2^3 = 8
    std::cout << result << std::endl;

    return 0;
}

```

---

# sqrt

求平方根

> 常用原型

```c++
double sqrt(double x);
float  sqrt(float x);     // C++11 起
long double sqrt(long double x);
```

> 示例

```c++
#include <iostream>
#include <cmath>  // sqrt 函数

int main() {
    double x = 16.0;

    double result = std::sqrt(x);  // √16 = 4
    std::cout << result << std::endl;

    return 0;
}
```

(注意某些场景，可能导致的精度丢失。)

---

