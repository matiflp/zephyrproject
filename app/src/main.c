#include<stdio.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("Running on %s\n host board\n", CONFIG_BOARD);
    return 0;
}