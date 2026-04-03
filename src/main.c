#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS   1000

int main(void)
{
	while (1) {
		printf("Hello World");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
