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

## 746. 使用最小花费爬楼梯

[746. 使用最小花费爬楼梯](https://leetcode.cn/problems/min-cost-climbing-stairs/)

> 思路

- 要到达 `i`，你可以从 `i-1` 台阶走 1 步，或者从 `i-2` 台阶走 2 步。

```c++
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        // dp[i] 到达 i 的最小花费
        int n = cost.size();
        vector<int> dp(n+1,0);
        // 0 为 第一个台阶
        dp[0] = 0;
        dp[1] = 0;
        for (int i = 2; i <= n;i++){
            dp[i] = min(dp[i-1] + cost[i-1],dp[i-2]+cost[i-2]);
        }
        return dp[n];
    }
};
```

---

## 62. 不同路径

[62. 不同路径](https://leetcode.cn/problems/unique-paths/)

> 思路

- 左 + 上，可以到此坐标。

```c++
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> map(m,vector<int>(n,0));
        // 初始化最上面一行和最左边一列，只有一种方法到大，就是沿着横/竖 走
        for (int i = 0; i < n; i++) map[0][i] = 1;
        for (int i = 0; i < m; i++) map[i][0] = 1;
        // 开始计算 从 1,1 开始出发
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                // 左 + 上
                map[i][j] = map[i-1][j] + map[i][j-1];
            }
        }
        return map[m-1][n-1];
    }
};
```

---

## 63. 不同路径 II

[63. 不同路径 II](https://leetcode.cn/problems/unique-paths-ii/)

> 思路

- 与上题相同，只是添加了障碍物的判断，但是要注意，初始化上、左，的时候，一旦遇到障碍物，后面的就无法继续移动到了。

```c++
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            if (obstacleGrid[0][i] == 0) dp[0][i] = 1;
            else break;
        }
        for (int i = 0; i < m; i++) {
            if (obstacleGrid[i][0] == 0) dp[i][0] = 1;
            else break;
        }
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                if (obstacleGrid[i][j] == 0){
                    dp[i][j] = dp[i-1][j] + dp[i][j-1];
                }
            }
        }
        return dp[m-1][n-1];
    }
};
```

---

## 343. 整数拆分

[343. 整数拆分](https://leetcode.cn/problems/integer-break/)

> 思路

- 拆分判断最大值

  - `dp[i-j] * j` → 左边拆，右边不拆。

  - `(i-j) * j` → 两边都不拆。

```c++
class Solution {
public:
    int integerBreak(int n) {
        vector<int> dp (n+1, 0);
        dp[1] = 1;
        dp[2] = 1;
        for (int i = 3; i <= n; i++) {
            for (int j = 1; j < i - 1; j++) {
                dp[i] = max(
                    // 不断对比寻找最大值
                    dp[i],
                    // 这个随着j的遍历，左边拆，右边不拆，还是左边不拆，右边拆的，都能遍历到
                    // 两遍都拆只可能更小
                    max (dp[i-j] * j,(i-j) * j)
                );
            }
        }
        return dp[n];
    }
};
```

---

## 96. 不同的二叉搜索树

[96. 不同的二叉搜索树](https://leetcode.cn/problems/unique-binary-search-trees/)

> 思路

- 控制节点个数，组合左右子树。

> 补充

- 这个数其实是 **Catalan 数**。

```c++
class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n+1, 0);
        // 设 dp[k] 表示有  k k 个节点时不同 BST 的数量
        dp[0] = 1;
        // 遍历节点个数
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= i; j++) {
                // 以 j 为跟节点，左子树可能 * 右子树可
                dp[i] += dp[j-1] * dp[i-j];
            }
        }
        return dp[n];
    }
};
```

---

# 滑动窗口

## 1493. 删掉一个元素以后全为 1 的最长子数组

[1493. 删掉一个元素以后全为 1 的最长子数组](https://leetcode.cn/problems/longest-subarray-of-1s-after-deleting-one-element/)

> 思路

- 维护只有一个值为0的窗口

```c++
class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int n = nums.size();
        int ret = 0;
        int left = 0;
        int right = 0;
        // 用于判断，如果全是1，也是要删掉一个
        int sum = accumulate(nums.begin(), nums.end(), 0);
        // 维护窗口中的0 和 1 的个数
        unordered_map<int,int> mp;
        for (; right < n; right++) {
            mp[nums[right]] ++;
            // 遇到第二个0个，开始收缩左窗口
            // 不用担心left < n 太大了，既然当前0的个数 > 1，说明左边一定是有个满足条件的 nums[left] == 0
            while (mp[0] > 1 && left < n) {
                mp[nums[left++]] --;
            }
            // 更新结果，因为可以去掉一个0，所以只统计窗口中1的个数即可。
            ret = max(ret,mp[1]);
        }
        return sum == n ? ret - 1 : ret;
    }
};
```

---

# 模拟

## 498. 对角线遍历

[498. 对角线遍历](https://leetcode.cn/problems/diagonal-traverse/)

> 思路

- 一个一个push，模拟路径，到头后调转方向。

```c++
class Solution {
public:
    vector<int> findDiagonalOrder(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        int dir = 1; // 控制方向 1 右上  0 左下
        vector<int> ret {};
        int x = 0;
        int y = 0;
        for (int i = 0; i < m*n; i++) {
            ret.push_back(mat[x][y]);
            if (dir) { // 右上
                // 判断调转方向
                if (y == n-1) {
                    dir = 0;
                    x++;
                } else if ( x == 0 ){
                    dir = 0;
                    y++;
                } else {
                    x--;
                    y++;
                }
            } else { // 左下
                if ( x == m-1 ) {
                    y ++;
                    dir = 1;
                } else if ( y == 0) {
                    x ++;
                    dir = 1;
                } else {
                    x++;
                    y--;
                }
            }
        }
        return ret;
    }
};
```

---

# 哈希表

## 36. 有效的数独

[36. 有效的数独](https://leetcode.cn/problems/valid-sudoku/)

> 思路

- 一次遍历，统计行、列、矩阵是否出现过。

```c++
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int n = board.size();
        int m = board[0].size();
        // 3 * 3

        vector<vector<bool>> matrix_map(9, vector<bool>(9, false));
        vector<vector<bool>> row_map(9, vector<bool>(9, false));
        vector<vector<bool>> col_map(9, vector<bool>(9, false));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                char c = board[i][j];
                if (c == '.') continue;
                int num = c - '0' -1; // 转为为数组下标
                // int k = i / 3 *3  + j / 3;// 定位是哪个自矩阵 x + y 是固定的
                int k = (i / 3) * 3 + (j / 3); // 定位是哪个自矩阵

                if (row_map[i][num] || 
                    col_map[j][num] || 
                    matrix_map[k][num] ) {
                        return false;
                }
                
                row_map[i][num] = true;
                col_map[j][num] = true;
                matrix_map[k][num] = true;

            }
        }
        return true;
    }
};
```

---

# 背包

## 01背包

### 416. 分割等和子集

[416. 分割等和子集](https://leetcode.cn/problems/partition-equal-subset-sum/)

> 思路

- 将问题转化为，背包容量为 sum / 2大小的背包，所能得到的最大物品价值，是不是 sum / 2。

```c++
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sumValue = accumulate(nums.begin(), nums.end(), 0);
        if (sumValue % 2) return false;
        int target = sumValue / 2;
        int n = nums.size();
        // dp[i][j] = 从前 i 个数（nums[0..i-1]）中，是否能选出一些数，使得它们的和恰好等于 j
        vector<vector<bool>> dp(n+1,vector<bool>(target+1,false));
        for (int i = 0;i <= n; i++) {
            dp[i][0] = true ;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= target; j++) {
                // 选择 i - 1 来对齐数组下标

                // 不选 i-1
                // 此时能不能拼出 j ，取决于前面的[i-1]能不能拼出 j
                dp[i][j] = dp[i-1][j];

                // 选 i-1
                if (j >= nums[i-1]) {
                    dp[i][j] = dp[i][j] || dp[i-1][j-nums[i-1]];
                }

            }
        }
        return dp[n][target];
    }
};
```

