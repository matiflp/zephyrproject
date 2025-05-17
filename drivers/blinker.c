#include <blinker.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <stdio.h>

#define DT_DRV_COMPAT zephyr_course_binding

struct led_timer_data {
    struct k_timer timer;
    uint8_t led_index;
};

struct my_device_config {
    const struct device* controller;
    struct {
        uint8_t leds_number[3]; // Assuming 3 LEDs for simplicity. Todo: make it dynamic
        uint8_t intervals[3];
    } props;
    struct led_timer_data led_data[3];
};

static void timer_expiry_function(struct k_timer *timer_id) {
    struct led_timer_data *data = CONTAINER_OF(timer_id, struct led_timer_data, timer);

    struct my_device_config *config = CONTAINER_OF(data, struct my_device_config, led_data[data->led_index]);

    uint8_t index = data->led_index;
    led_off(config->controller, config->props.leds_number[index]);
}

static int my_device_blink_on(const struct device *dev, uint8_t duration){
    struct my_device_config* config = (struct my_device_config *)dev->config;

    if(duration == 0){
        for (int i = 0 ; i < ARRAY_SIZE(config->props.leds_number); i++){
            printk("Blinking led %d for %d seconds\n", config->props.leds_number[i], config->props.intervals[i]);
            led_on(config->controller, config->props.leds_number[i]);
            k_timer_start(&config->led_data[i].timer, K_SECONDS(config->props.intervals[i]), K_NO_WAIT);
        }
    }
    else{
        printk("Blinking on for %d ms\n", duration);
        for (int i=0 ; i<ARRAY_SIZE(config->props.leds_number); i++){
            printk("Blinking led %d for %d seconds\n", config->props.leds_number[i], config->props.intervals[i]);
            led_on(config->controller, config->props.leds_number[i]);
        }
    }

    return 0;
}

static int my_device_blink_off(const struct device *dev){
    const struct my_device_config* config = dev->config; 

    for (int i=0 ; i<ARRAY_SIZE(config->props.leds_number); i++){
        printk("Blinking off\n");
        led_off(config->controller, config->props.leds_number[i]);
    }

    return 0;
}

static const struct blinker_driver_api my_device_blinker_api = {
    .on = my_device_blink_on,
    .off = my_device_blink_off,
};

static int my_device_init(const struct device *dev)
{
    printk("Device %s initialized\n", dev->name);

    struct my_device_config* config = (struct my_device_config *) dev->config;

    for (int i = 0; i < ARRAY_SIZE(config->led_data); i++) {
        config->led_data[i].led_index = i;
        k_timer_init(&config->led_data[i].timer, timer_expiry_function, NULL);
    }

    return 0;
}

#define MY_DEVICE_DEFINE(inst) \
    static struct my_device_config my_device_config_##inst = {                            \
        .controller = DEVICE_DT_GET(DT_INST_PHANDLE(inst, interrupt_controller)),               \
        .props = { \
            .leds_number = DT_INST_PROP(inst, leds_number), \
            .intervals = DT_INST_PROP(inst, intervals), \
        },		                                            \
    };                                                                                          \
                                                                                                \
    DEVICE_DT_INST_DEFINE(inst, my_device_init, NULL, NULL, &my_device_config_##inst, POST_KERNEL, 90, &my_device_blinker_api);

DT_INST_FOREACH_STATUS_OKAY(MY_DEVICE_DEFINE);