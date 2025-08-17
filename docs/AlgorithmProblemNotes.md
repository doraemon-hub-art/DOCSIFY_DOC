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

