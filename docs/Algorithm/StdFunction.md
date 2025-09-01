#

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

# accumulate

求和，求运算结果(根据给定的运算函数)

> 函数原型

```c++
template<class InputIt, class T>
T accumulate(InputIt first, InputIt last, T init);

template<class InputIt, class T, class BinaryOperation>
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op);
```

`first, last` ：迭代器区间 `[first, last)`，要处理的序列。

`init` ：初始值，累加从它开始。

`op` （可选）：二元操作函数，定义如何“累加”。默认是 `operator+`。

> 示例

```c++
#include <numeric>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    vector<int> v{1, 2, 3, 4, 5};
    int sum = accumulate(v.begin(), v.end(), 0);
    cout << sum << endl; // 输出 15
    
	return 0;
}
```

