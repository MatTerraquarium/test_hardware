/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

static const struct device *const dht11 = DEVICE_DT_GET(DT_ALIAS(dht11));
static const struct gpio_dt_spec level_shifter = GPIO_DT_SPEC_GET(DT_ALIAS(level_shifter), gpios);
static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_ALIAS(servo));
static const uint32_t min_pulse = DT_PROP(DT_ALIAS(servo), min_pulse);
static const uint32_t max_pulse = DT_PROP(DT_ALIAS(servo), max_pulse);

int main(void)
{

	int ret;
	uint32_t pulse_width = (uint32_t)((max_pulse + min_pulse) / 2);
	struct sensor_value temperature;
	struct sensor_value humidity;

	ret = gpio_is_ready_dt(&level_shifter);
	if (ret == 0) {
		printk("Error: level_shifter is not ready\n");
		return 0;
	}
	ret = gpio_pin_configure_dt(&level_shifter, GPIO_OUTPUT_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure level shifter\n", ret);
		return 0;
	}
	ret = gpio_pin_set_dt(&level_shifter, 1);
	if (ret != 0) {
		printk("Error %d: failed to set level shifter at logic level 1\n", ret);
		return 0;
	}

	ret = device_is_ready(servo.dev);
	if (ret == 0) {
		printk("Error: PWM device %s is not ready\n", servo.dev->name);
		return 0;
	}
	ret = pwm_set_pulse_dt(&servo, pulse_width);
	if (ret != 0) {
		printk("Error %d: failed to set pulse width\n", ret);
		return 0;
	}

	ret = device_is_ready(dht11);
	if (ret == 0) {
		printk("Device %s is not ready\n", dht11->name);
		return 0;
	}
	ret = sensor_sample_fetch(dht11);
	if (ret != 0) {
		printk("Sensor fetch failed: %d\n", ret);
		return 0;
	}
	ret = sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &temperature);
	if (ret == 0) {
		ret = sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &humidity);
	}
	if (ret != 0) {
		printk("get failed: %d\n", ret);
		return 0;
	}

	printk("temp: %d\n%d\n", temperature.val1, temperature.val2);
	printk("hum: %d\n%d\n", humidity.val1, humidity.val2);

	while (1) {
		k_sleep(K_SECONDS(1));
	}

	return 0;
}
