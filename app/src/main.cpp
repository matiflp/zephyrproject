#include <iostream>
#include <string>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#include "nrfx_temp.h"

using namespace std;

LOG_MODULE_REGISTER(my_log_module, CONFIG_ZEPHYR_COURSE_LOG_LEVEL_ERR);

#define MY_CONS_INTERFACE DT_ALIAS(my_cons_interface)
const struct device *uart= DEVICE_DT_GET(MY_CONS_INTERFACE);

#define ZEPHYR_COURSE_NODE DT_NODELABEL(zephyr_course_node)

#define SLEEP_TIME_MS 100

static const struct device *const longpress_dev = DEVICE_DT_GET(
		DT_NODELABEL(longpress));

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

#define SW0_NODE DT_ALIAS(sw0) 
#define SW1_NODE DT_ALIAS(sw1) 
#define SW2_NODE DT_ALIAS(sw2) 
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);

static void input_event_handler(struct input_event *evt)
{
    // Print event output data
    LOG_ERR("input event: dev=%-16s %3s type=%2x code=%3d value=%d",
		evt->dev ? evt->dev->name : "NULL",
		evt->sync ? "SYN" : "",
		evt->type,
		evt->code,
		evt->value);

    if (evt->value == 1)
    {
        switch (evt->code) {

            // release before two second
            case INPUT_KEY_A:
                gpio_pin_toggle_dt(&led0);
                LOG_ERR("INPUT_KEY_A\n");
                break;
            case INPUT_KEY_B:
                gpio_pin_toggle_dt(&led1);
                LOG_ERR("INPUT_KEY_B\n");
                break;
            case INPUT_KEY_C:
                gpio_pin_toggle_dt(&led2);
                LOG_ERR("INPUT_KEY_C\n");
                break;

            // hold for more than two second
            case INPUT_KEY_X:
                gpio_pin_toggle_dt(&led0);
                LOG_ERR("INPUT_KEY_X\n");
                break;
            case INPUT_KEY_Y:
                gpio_pin_toggle_dt(&led1);
                LOG_ERR("INPUT_KEY_Y\n");
                break;
            case INPUT_KEY_Z:
                gpio_pin_toggle_dt(&led2);
                LOG_ERR("INPUT_KEY_Z\n");
                break;
        }
    }
}
INPUT_CALLBACK_DEFINE(longpress_dev, input_event_handler);

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

    // ======================================

    // 32 bits
    int64_t negative_valuee = (int32_t)DT_PROP(ZEPHYR_COURSE_NODE, negative_value);
    LOG_ERR("negative-value prop value: %lld", negative_valuee);

    // 64 bits
    int32_t negative_value_64_bits_high = DT_PROP_BY_IDX(ZEPHYR_COURSE_NODE, negative_value_64_bits, 0);
    uint32_t negative_value_64_bits_low  = DT_PROP_BY_IDX(ZEPHYR_COURSE_NODE, negative_value_64_bits, 1);
    int64_t full_value = (static_cast<int64_t>(negative_value_64_bits_high) << 32) | negative_value_64_bits_low;
    LOG_ERR("Full value (64-bit): %lld", full_value);

    // ======================================

	if (!device_is_ready(button0.port)) {
        LOG_ERR("Port device not ready\r\n");
		return -1;
	}

    gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);

    gpio_pin_configure_dt(&button0, GPIO_INPUT);
    gpio_pin_configure_dt(&button1, GPIO_INPUT);
    gpio_pin_configure_dt(&button2, GPIO_INPUT);

    // ======================================

    nrfx_err_t status;
    (void)status;

    nrfx_temp_config_t config = NRFX_TEMP_DEFAULT_CONFIG;
    status = nrfx_temp_init(&config, NULL);
    NRFX_ASSERT(status == NRFX_SUCCESS);

    status = nrfx_temp_measure();
    NRFX_ASSERT(status == NRFX_SUCCESS);

    int32_t temperature = nrfx_temp_result_get();
    int32_t kelvin_temperature = nrfx_temp_calculate(temperature);

    LOG_ERR("Get value: %d [K]", kelvin_temperature);

    int32_t whole_kelvin = kelvin_temperature / 100;
    uint8_t fraction_kelvin = NRFX_ABS(kelvin_temperature % 100);

    LOG_ERR("Measured temperature: %d.%02u [K]", whole_kelvin, fraction_kelvin);

    while (1) {
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}