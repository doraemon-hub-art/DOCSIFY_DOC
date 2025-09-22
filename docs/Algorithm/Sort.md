# 冒泡排序

> 基本实现

```C++
void popSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0 ; i < n; i++) {
        for (int j = i + 1; j < n ; j++) {
            if (arr[j] > arr[i]) {
                swap(arr[j], arr[i]);
            }
        }
    }
}
```


---

# 快排

> 基本实现

```C++

```