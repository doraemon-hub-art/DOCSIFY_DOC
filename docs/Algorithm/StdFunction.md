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

---

# std::any_of(C++11) 和 ranges::any_of(C++20) 

## std::any_of(C++11)

检查 **区间 `[first, last)` 中是否存在至少一个元素** 使得谓词 `p(element)` 返回 `true`。

- 如果找到这样的元素，返回 `true`；
- 如果遍历结束都没有找到，返回 `false`。

> 函数原型

```C++
template< class InputIt, class UnaryPredicate >
bool any_of(InputIt first, InputIt last, UnaryPredicate p);
```

**`first, last`**
 输入迭代器，表示要检查的序列范围。

**`p`**
 一元谓词（函数、函数对象或 lambda 表达式），返回 `true` 或 `false`。

> 示例

```C++
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums = {1, 3, 5, -2, 7};

    bool hasNegative = std::any_of(nums.begin(), nums.end(),
                                   [](int x) { return x < 0; });

    std::cout << std::boolalpha << hasNegative << std::endl; 
    // 输出 true
}
```

---

## ranges::any_of(C++20) 

定义在 `<algorithm>` 但在 **`std::ranges` 命名空间**里（C++20 引入 **Ranges** 库）。

直接接受 **整个容器** 或 **ranges 视图**(划分区间)，而不是一对迭代器。

> 示例

```C++
#include <algorithm>
#include <string>
#include <iostream>

int main() {
    std::string s = "hello";

    bool res = std::ranges::any_of(s, [](char c) {
        return c == 'e';
    });

    std::cout << std::boolalpha << res << std::endl; // true
    
    // 切割子区间 -----------------------------------------
    std::vector<int> v = {1, 2, 3, 4, 5};

    // 只检查前 3 个元素
    bool res = std::ranges::any_of(v | std::views::take(3), [](int x){
        return x == 3;
    });

    std::cout << std::boolalpha << res << std::endl; // true
}
```

---

