#include <iostream>
#include <string>
#include <zephyr/logging/log.h>
using namespace std;

LOG_MODULE_REGISTER(my_log_module, CONFIG_ZEPHYR_COURSE_LOG_LEVEL);

int main(void)
{
    constexpr bool is_native_posix = (CONFIG_BOARD == "native_posix_64");

    if (is_native_posix) {
        cout << "The application is running on the native_posix_64 host board\n";
    } else {
        cout << "Running on " << CONFIG_BOARD << " host board.\n";
    }

    LOG_ERR("LOG_ERR Message: Running on board: %s", CONFIG_BOARD);
    
    return 0;
}