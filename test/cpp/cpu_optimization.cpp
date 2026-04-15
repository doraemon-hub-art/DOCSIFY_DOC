#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

// 模拟轻量级工作
// 防止内联优化
__attribute__((noinline)) void do_work(int &target) { target += 1; }

int main() {
  const int SIZE = 10'000'000;
  vector<int> random_data(SIZE);
  mt19937 rng(42);
  uniform_int_distribution<int> dist(0, 1); // 0 或 1 随机

  for (int &x : random_data)
    x = dist(rng);

  int sink = 0;

  // 1. 基准测试：无分支
  auto s1 = high_resolution_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    do_work(sink);
  }
  auto e1 = high_resolution_clock::now();
  cout << "无分支耗时:         " << duration_cast<milliseconds>(e1 - s1).count()
       << " ms" << endl;

  // 2. 可预测分支：模拟你的视频帧逻辑（始终成立或始终不成立）
  sink = 0;
  auto s2 = high_resolution_clock::now();
  bool test_flag = false;
  for (int i = 0; i < SIZE; ++i) {
    if (!test_flag) { // 永远不成立
      test_flag = true;
      do_work(sink);
    }
    // 为了公平，这里我们要确保执行了逻辑，只是展示分支预测的威力
    // 我们改为一个始终成立的分支
    if (i >= 0) {
      do_work(sink);
    }
  }
  auto e2 = high_resolution_clock::now();
  cout << "可预测分支耗时:     " << duration_cast<milliseconds>(e2 - s2).count()
       << " ms" << endl;

  // 3. 不可预测分支：CPU 无法猜到 if 是否成立
  sink = 0;
  auto s3 = high_resolution_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    if (random_data[i] == 1) { // 50% 概率，完全随机
      do_work(sink);
    } else {
      // 这里我们保持工作量一致，或者仅仅观察 if 带来的损耗
      sink += 0;
    }
  }
  auto e3 = high_resolution_clock::now();
  cout << "不可预测分支耗时:   " << duration_cast<milliseconds>(e3 - s3).count()
       << " ms" << endl;

  return 0;
}
