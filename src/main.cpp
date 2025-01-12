#include <iostream>
#include <string>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
using namespace std;

LOG_MODULE_REGISTER(my_log_module, CONFIG_ZEPHYR_COURSE_LOG_LEVEL_ERR);

#define MY_CONS_INTERFACE DT_ALIAS(my_cons_interface)
const struct device *uart= DEVICE_DT_GET(MY_CONS_INTERFACE);

int main(void)
{
    constexpr bool is_native_posix = (CONFIG_BOARD == "native_posix_64");
    if (is_native_posix) {
       LOG_ERR("The application is running on the native_posix_64 host board\n");
    } else {
        LOG_ERR("Running on %s", CONFIG_BOARD);
    }

    // ======================================

    LOG_ERR("LOG_ERR Message: Running on board: %s", CONFIG_BOARD);

    // ======================================
    if (!device_is_ready(uart)){
        printk("UART device not ready\r\n");
        return 1 ;
	}

    if(CONFIG_UART_DEVICE_STATUS == "y"){
        LOG_ERR("UART device is enabled");
        LOG_ERR("Device current-speed: %d", DT_PROP(MY_CONS_INTERFACE, current_speed));
        LOG_ERR("Device status: %s", DT_PROP(MY_CONS_INTERFACE, status));
        LOG_ERR("UART Base Address: 0x%x", DT_REG_ADDR(MY_CONS_INTERFACE));
    }else{
        LOG_ERR("UART device is not enabled");
    }

    return 0;
}