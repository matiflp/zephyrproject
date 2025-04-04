#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/kernel.h>

typedef int (*blink_on)(const struct device *dev, uint8_t duration);
typedef int (*blink_off)(const struct device *dev);

__subsystem struct blinker_driver_api {
    blink_on on;
    blink_off off;
};

__syscall int custom_blink_on(const struct device *dev, uint8_t duration);
static inline int z_impl_custom_blink_on(const struct device *dev, uint8_t duration)
{
    const struct blinker_driver_api *api = (const struct blinker_driver_api *)dev->api;
    return api->on(dev, duration);
}

__syscall int custom_blink_off(const struct device *dev);
static inline int z_impl_custom_blink_off(const struct device *dev)
{
    const struct blinker_driver_api *api = (const struct blinker_driver_api *)dev->api;
    return api->off(dev);
}

#ifdef __cplusplus
}
#endif

#include <syscalls/blinker.h>