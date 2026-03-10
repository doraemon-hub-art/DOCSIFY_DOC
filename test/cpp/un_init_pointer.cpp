#include <iostream>

int main(void) {

  void* custom_ptr_ {nullptr} ;
  if (custom_ptr_) {
    delete custom_ptr_;
    custom_ptr_ = nullptr;
  }

  return 0;
}


