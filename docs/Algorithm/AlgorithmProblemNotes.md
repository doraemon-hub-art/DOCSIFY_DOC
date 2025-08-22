# 

# 栈

## 20. 有效的括号

[20. 有效的括号 - 力扣（LeetCode）](https://leetcode.cn/problems/valid-parentheses/description/)

> 思路

- 用栈去匹配，遍历字符串，左括号进栈，当匹配到右括号时，判断栈顶是不是与之对应的左括号。

```c++
class Solution {
public:
    bool isValid(string s) {
        std::unordered_map<char, char> lefts {
            {'(', ')'},
            {'{', '}'},
            {'[', ']'}
        };
        std::unordered_map<char, char> rights {
            {')', '('},
            {'}', '{'},
            {']', '['}
        };
        
        stack<char> st;
        for (auto& c : s){
            // is left, push it into stack
            if (lefts.find(c) != lefts.end()){
                st.push(c);
            }else if (rights.find(c) != rights.end()){
                // is right ,find current stack stop is target left ?
                if (st.empty() || 
                    (st.top() != rights[c])) return false;
                st.pop();
            }
        }
        return st.empty();
    }
};
```

---

## 1047. 删除字符串中的所有相邻重复项

[1047. 删除字符串中的所有相邻重复项](https://leetcode.cn/problems/remove-all-adjacent-duplicates-in-string/)

> 思路

- 遍历字符串，对比当前字符与栈顶元素，相同即弹出，反之，压栈。最后取出来翻转字符串。

```c++
class Solution {
public:
    string removeDuplicates(string s) {
        stack<char> st;
        for (auto c : s){
            if (!st.empty() &&
                st.top() == c){
                    st.pop();
            }else{
                st.push(c);
            }
        }
        string retStr = "";
        while (!st.empty()){
            retStr += st.top();
            st.pop();
        }
        reverse(retStr.begin(),retStr.end());
        return retStr;
    }
};
```

---

## 150. 逆波兰表达式求值

[150. 逆波兰表达式求值](https://leetcode.cn/problems/evaluate-reverse-polish-notation/)

> 思路

- 遍历字符串，数子压栈，遇到运算符pop出来前面的两个数字，然后压栈运算

```c++
class Solution {
public:
    int cal(int a,int b,string op){
        if (op == "+"){
            return a + b;
        }else if(op == "-"){
            return a - b;
        }else if(op == "*"){
            return a * b;
        }else if(op == "/"){
            return a / b;
        }
        return 0;
    }
    int evalRPN(vector<string>& tokens) {
        unordered_set<string> operates {
            "+",
            "-",
            "*",
            "/"
        };
        stack<string> st;
        for (auto c : tokens){
            // is number
            if (operates.find(c) == operates.end()){
                st.push(c);
            }else { // is operator
                int b = stoi(st.top());
                st.pop();
                int a = stoi(st.top());
                st.pop();
                int tmp = cal(a,b,c);
                st.push(to_string(tmp));
            }
        }
        return stoi(st.top());
    }
};
```

---
# 滑动窗口

## 239. 滑动窗口最大值

[239. 滑动窗口最大值](https://leetcode.cn/problems/sliding-window-maximum/)

> 概念补充

**单调队列**

- 一种 **特殊的双端队列（deque）**，队列中的元素按照一定的单调性（递增 / 递减）排列。
- 插入时会把队尾“不符合单调性”的元素弹出，以保持整体的有序性。
- 常用于 **滑动窗口最值** 等问题。

**优先级队列**

- 一般用 **堆（heap）** 实现。
- 队列中的元素没有整体顺序，但能保证每次取出的元素是当前“最大 / 最小值”。
- 插入和取出最大 / 最小值的复杂度是 **O(log n)**。

> 思路: 使用STD中的优先队列(大根堆)；

- i - k 控制当前堆中的最大元素是否在窗口范围内
- i >= k - 1控制从哪开始收集结果值

![](http://oss.banshengua.top//blogimages/91f18118d44ad54df297e12fc7133584.gif)

```c++
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        using P = pair<int,int>;
        std::priority_queue<P> heap;
        vector<int> ret{};
        int n = nums.size();
        for (int i = 0; i< n ;i++){
            heap.push({nums[i],i});
            // using i - k keep cureent window
            while (heap.top().second <= i - k){
                // beyond the scope of the current window
                heap.pop();
            }
            // control where start to save result
            if (i >= k - 1){
                ret.push_back(heap.top().first);
            }
        }
        return ret;
    }
};
```

---

## 347. 前 K 个高频元素

[347. 前 K 个高频元素](https://leetcode.cn/problems/top-k-frequent-elements/)

> 思路1: 

- 遍历存储到map中，然后构造pair按照次数排序，之后遍历pair数组取出前k个。

```c++
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int> mp;
        for (auto n : nums){
            mp[n]++;
        }
        using P = pair<int,int>;
        vector<P> retPairs;
        for (auto m : mp){
            retPairs.emplace_back(m.first,m.second);
        }
        sort(retPairs.begin(),retPairs.end(),[&](P a, P b){
            return a.second > b.second;
        });
        vector<int> ret {};
        // while (k--)
        int n = retPairs.size();
        for (int i = 0 ;i < n && k--;i++){
            ret.push_back(retPairs[i].first);
        }
        return ret;

    }
};
```

> 思路2: 

- 小跟堆实现，堆顶为出现次数最小的。
- 注意: priority_queue 和 std::sort 的比较函数意义正好相反。

```c++
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        using P = pair<int,int>;
        auto compare_mode =  [](P& a, P& b){
            return a.second > b.second;
        };
        unordered_map<int,int> map;
        for (auto n : nums){
            map[n]++;
        }
        // heap need vector save
        priority_queue<P,vector<P>,decltype(compare_mode)> pri_que;
        for (auto m : map){
            pri_que.push(m);
            // control 7 numbers
            if (pri_que.size() > k){
                pri_que.pop();
            }
        }
        vector<int> ret {};
        while (!pri_que.empty()){
            ret.emplace_back(pri_que.top().first);
            pri_que.pop();
        }
        return ret;
    }
};
```

---

## 2348. 全 0 子数组的数目

[2348. 全 0 子数组的数目](https://leetcode.cn/problems/number-of-zero-filled-subarrays/)

> 思路

- 维护都是0的子数组窗口，累加以左端为0的全0子数组个数；

```c++
class Solution {
public:
    long long zeroFilledSubarray(vector<int>& nums) {
        long long ret = 0;
        int n = nums.size();
        // 左端点
        int left = -1;
        for(int i = 0 ; i < n ; i++){
            if (nums[i]){
                left = i;
            }else{ // 以 left 为左端点的连续 0 子数组的个数
                // left + 1, left + 2, left + 3 ... 都是符合条件的子数组
                ret += i - left;
            }
        }
        return ret;
    }
};
```

---

# 动态规划

## 509. 斐波那契数

[509. 斐波那契数](https://leetcode.cn/problems/fibonacci-number/)

>DP 思路

- 确定dp数组
- 确定递推公式——递推公式决定dp数组要如何初始化
- dp数组如何初始化
- 确定遍历顺序
- 举例推导dp数组

> 思路

- 入门的DP题目

```c++
class Solution {
public:
    int fib(int n) {
        if (n < 2)return n;
        // just use 2 size vector can do it
        vector<int> matrit(n+1, 0);
        // init status
        matrit[0] = 0;
        matrit[1] = 1;
        for (int i = 2; i <= n; i++) {
            // recursive formula
            matrit[i] = matrit[i-1] + matrit[i-2];
        }
        return matrit[n];
    }
};
```

---

## 70. 爬楼梯

[70. 爬楼梯](https://leetcode.cn/problems/climbing-stairs/)

> 思路

- 递推公式解释
  - matrix[i] = matrix[i-1] + matrix[i-2] 表示 “到第 i 阶的所有方案 = 最后一步是 1 阶时的方案数 + 最后一步是 2 阶时的方案数”。

```c++
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;
        vector<int> matrix (n+1,0);
        matrix[0] = 0;
        matrix[1] = 1;
        matrix[2] = 2;
        for (int i = 3; i <= n; i++) {
            matrix[i] = matrix[i-1] + matrix[i-2];
        }
        return matrix[n];
    }
};
```

---



