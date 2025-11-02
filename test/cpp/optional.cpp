#include <iostream>
#include <optional>
#include <syslog.h>
using namespace std;

void testWorker() {
    std::cout << "start work" << std::endl;
    for (int i = 0 ; i < 10; i++) {
        std::cout << i << std::endl;
    }
    std::cout << "end work" << std::endl;
}

int main(void) {
    std::optional<std::string> str;
    // std::cout << str.value() << std::endl;
    syslog(LOG_INFO, "Test null optional str value is: %s", str.value().c_str());
    testWorker();
    std::cout << "will be end" << std::endl;
    return 0;
}