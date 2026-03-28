#include <cerrno>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex> // include std::call_once and std::once_flag
#include <vector>

std::once_flag resource_flag;

void initialize_resource() {
    std::cout << __func__ << std::endl;
    try {

      std::vector<int> vec;
      // int a = vec.front() + 1; // Undefine Behavior
      int a = vec.at(0); // abnormal
    } catch (...) { 
      std::cout << "recv error" << std::endl;
      throw; // to reset
  }
}

void worker_thread(int id) {
    try {
      // block on here
      std::call_once(resource_flag, initialize_resource);
      std::cout << "thread: " << id << " 正在运行" << std::endl;
    } catch (...) {
      std::cout << __func__ << " : error" << std::endl;
    }
   }

int main() {
    std::thread t1(worker_thread, 1);
    std::thread t2(worker_thread, 2);
    std::thread t3(worker_thread, 3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
