/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

static const struct gpio_dt_spec level_shifter1 = GPIO_DT_SPEC_GET(DT_ALIAS(level_shifter1), gpios);

static const struct gpio_dt_spec relay1 = GPIO_DT_SPEC_GET(DT_ALIAS(relay1), gpios);
static const struct gpio_dt_spec relay2 = GPIO_DT_SPEC_GET(DT_ALIAS(relay2), gpios);
static const struct gpio_dt_spec relay3 = GPIO_DT_SPEC_GET(DT_ALIAS(relay3), gpios);
static const struct gpio_dt_spec relay4 = GPIO_DT_SPEC_GET(DT_ALIAS(relay4), gpios);

static const struct device *const dht11 = DEVICE_DT_GET(DT_ALIAS(dht11));
static const struct device *const dht22 = DEVICE_DT_GET(DT_ALIAS(dht22));
static const struct device *const ds18b20 = DEVICE_DT_GET(DT_ALIAS(ds18b20));

static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_ALIAS(servo));
static const uint32_t min_pulse = DT_PROP(DT_ALIAS(servo), min_pulse);
static const uint32_t max_pulse = DT_PROP(DT_ALIAS(servo), max_pulse);

volatile uint32_t pulse_width;

int main(void)
{
	int ret;
	struct sensor_value dht11_temperature;
	struct sensor_value dht11_humidity;
	struct sensor_value dht22_temperature;
	struct sensor_value dht22_humidity;
	struct sensor_value ds18b20_temperature;

	pulse_width = (uint32_t)((max_pulse + min_pulse) / 2);
	printk("Pulse Width: %d\n", pulse_width);

    ///////////////////////////////////////////////////////////////////////////
	// VERIFY IF THE GPIO ATTACHED TO THE OE OF THE LEVEL SHIFTER IS READY
	ret = gpio_is_ready_dt(&level_shifter1);
	if (ret == 0) {
		printk("Error: level_shifter1 is not ready\n");
		return 0;
	}
	// CONFIGURE THE GPIO ATTACHED TO THE OE OF THE LEVEL SHIFTER AS A GPIO OUT 
  	// AND SET IT TO ACTIVE STATE
	ret = gpio_pin_configure_dt(&level_shifter1, GPIO_OUTPUT_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure level shifter 1\n", ret);
		return 0;
	}
  	// SET THE GPIO ATTACHED TO THE OE OF THE LEVEL SHIFTER TO LOGIC LEVEL '1'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&level_shifter1, 1);
	if (ret != 0) {
		printk("Error %d: failed to set level shifter 1 at logic level 1\n", ret);
		return 0;
	}


    ///////////////////////////////////////////////////////////////////////////
	// VERIFY IF THE GPIO ATTACHED TO THE RELAY IS READY
	ret = gpio_is_ready_dt(&relay1);
	if (ret == 0) {
		printk("Error: relay1 is not ready\n");
		return 0;
	}
	// CONFIGURE THE GPIO ATTACHED TO THE OE OF THE RELAY AS A GPIO OUT 
  	// AND SET IT TO ACTIVE STATE
	ret = gpio_pin_configure_dt(&relay1, GPIO_OUTPUT_HIGH);
	if (ret != 0) {
		printk("Error %d: failed to configure relay 1\n", ret);
		return 0;
	}

	// VERIFY IF THE GPIO ATTACHED TO THE RELAY IS READY
	ret = gpio_is_ready_dt(&relay2);
	if (ret == 0) {
		printk("Error: relay2 is not ready\n");
		return 0;
	}
	// CONFIGURE THE GPIO ATTACHED TO THE OE OF THE RELAY AS A GPIO OUT 
  	// AND SET IT TO ACTIVE STATE
	ret = gpio_pin_configure_dt(&relay2, GPIO_OUTPUT_HIGH);
	if (ret != 0) {
		printk("Error %d: failed to configure relay 2\n", ret);
		return 0;
	}

    // VERIFY IF THE GPIO ATTACHED TO THE RELAY IS READY
	ret = gpio_is_ready_dt(&relay3);
	if (ret == 0) {
		printk("Error: relay3 is not ready\n");
		return 0;
	}
	// CONFIGURE THE GPIO ATTACHED TO THE OE OF THE RELAY AS A GPIO OUT 
  	// AND SET IT TO ACTIVE STATE
	ret = gpio_pin_configure_dt(&relay3, GPIO_OUTPUT_HIGH);
	if (ret != 0) {
		printk("Error %d: failed to configure relay 3\n", ret);
		return 0;
	}

	// VERIFY IF THE GPIO ATTACHED TO THE RELAY IS READY
	ret = gpio_is_ready_dt(&relay4);
	if (ret == 0) {
		printk("Error: relay4 is not ready\n");
		return 0;
	}
	// CONFIGURE THE GPIO ATTACHED TO THE OE OF THE RELAY AS A GPIO OUT 
  	// AND SET IT TO ACTIVE STATE
	ret = gpio_pin_configure_dt(&relay4, GPIO_OUTPUT_HIGH);
	if (ret != 0) {
		printk("Error %d: failed to configure relay 4\n", ret);
		return 0;
	}


  	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '1'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay1, 0); // ON
	if (ret != 0) {
		printk("Error %d: failed to set relay 1 at logic level 1\n", ret);
		return 0;
	}
	k_sleep(K_MSEC(500));
	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '0'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay1, 1); // OFF
	if (ret != 0) {
		printk("Error %d: failed to set relay 1 at logic level 0\n", ret);
		return 0;
	}

  	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '1'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay2, 0); // ON
	if (ret != 0) {
		printk("Error %d: failed to set relay 2 at logic level 1\n", ret);
		return 0;
	}
	k_sleep(K_MSEC(500));
	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '0'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay2, 1); // OFF
	if (ret != 0) {
		printk("Error %d: failed to set relay 2 at logic level 0\n", ret);
		return 0;
	}

  	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '1'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay3, 0); // ON
	if (ret != 0) {
		printk("Error %d: failed to set relay 3 at logic level 1\n", ret);
		return 0;
	}
	k_sleep(K_MSEC(500));
	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '0'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay3, 1); // OFF
	if (ret != 0) {
		printk("Error %d: failed to set relay 3 at logic level 0\n", ret);
		return 0;
	}

  	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '1'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay4, 0); // ON
	if (ret != 0) {
		printk("Error %d: failed to set relay 4 at logic level 1\n", ret);
		return 0;
	}
	k_sleep(K_MSEC(500));
	// SET THE GPIO ATTACHED TO THE OE OF THE RELAY TO LOGIC LEVEL '0'
  	// (FOR DEBUG PURPOSES)
  	ret = gpio_pin_set_dt(&relay4, 1); // OFF
	if (ret != 0) {
		printk("Error %d: failed to set relay 4 at logic level 0\n", ret);
		return 0;
	}


    ///////////////////////////////////////////////////////////////////////////
	// VERIFY IF THE DHT11 SENSOR DEVICE IS READY
	ret = device_is_ready(dht11);
	if (ret == 0) {
		printk("Device %s is not ready\n", dht11->name);
		return 0;
	}
	k_sleep(K_MSEC(2000));
	// DO A FETCH OF THE DHT11 SENSOR DEVICE
	ret = sensor_sample_fetch(dht11);
	if (ret != 0) {
		printk("Sensor %s fetch failed: %d\n", dht11->name, ret);
		//return 0;
	} else {
		// GET THE TEMP AND HUMIDITY VALUES OBTAINED FROM THE FETCH
		ret = sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &dht11_temperature);
		if (ret == 0) {
			ret = sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY, &dht11_humidity);
		}
		if (ret != 0) {
			printk("get failed: %d\n", ret);
			return 0;
		}
		// PRINT THE READ TEMP AND HUMIDITY VALUES
		printk("temp: %d\n%d\n", dht11_temperature.val1, dht11_temperature.val2);
		printk("hum: %d\n%d\n", dht11_humidity.val1, dht11_humidity.val2);
	}

	
	// VERIFY IF THE DHT22 SENSOR DEVICE IS READY
	ret = device_is_ready(dht22);
	if (ret == 0) {
		printk("Device %s is not ready\n", dht22->name);
		return 0;
	}
	k_sleep(K_MSEC(2000));
	// DO A FETCH OF THE DHT22 SENSOR DEVICE
	ret = sensor_sample_fetch(dht22);
	if (ret != 0) {
		printk("Sensor %s fetch failed: %d\n",dht22->name, ret);
		//return 0;
	} else {
		// GET THE TEMP AND HUMIDITY VALUES OBTAINED FROM THE FETCH
		ret = sensor_channel_get(dht22, SENSOR_CHAN_AMBIENT_TEMP, &dht22_temperature);
		if (ret == 0) {
			ret = sensor_channel_get(dht22, SENSOR_CHAN_HUMIDITY, &dht22_humidity);
		}
		if (ret != 0) {
			printk("get failed: %d\n", ret);
			return 0;
		}
		// PRINT THE READ TEMP AND HUMIDITY VALUES
		printk("temp: %d\n%d\n", dht22_temperature.val1, dht22_temperature.val2);
		printk("hum: %d\n%d\n", dht22_humidity.val1, dht22_humidity.val2);
	}

	// VERIFY IF THE DS18B20 SENSOR DEVICE IS READY
	ret = device_is_ready(ds18b20);
	if (ret == 0) {
		printk("Device %s is not ready\n", ds18b20->name);
		return 0;
	}
	k_sleep(K_MSEC(2000));
	// DO A FETCH OF THE DS18B20 SENSOR DEVICE
	ret = sensor_sample_fetch(ds18b20);
	if (ret != 0) {
		printk("Sensor %s fetch failed: %d\n",ds18b20->name, ret);
		//return 0;
	} else {
		// GET THE TEMP VALUE OBTAINED FROM THE FETCH
		ret = sensor_channel_get(ds18b20, SENSOR_CHAN_AMBIENT_TEMP, &ds18b20_temperature);
		if (ret != 0) {
			printk("get failed: %d\n", ret);
			return 0;
		}
		// PRINT THE READ TEMP VALUES
		printk("temp: %d\n%d\n", ds18b20_temperature.val1, ds18b20_temperature.val2);
	}


	k_sleep(K_MSEC(1500));

    ///////////////////////////////////////////////////////////////////////////
	// VERIFY IF THE SERVO DEVICE IS READY
	ret = device_is_ready(servo.dev);
	if (ret == 0) {
		printk("Error: PWM device %s is not ready\n", servo.dev->name);
		return 0;
	}
	// SET THE SERVO PULSE PARAMETER ACCORDINGLY TO THE OVERLAY DEFINITION
	ret = pwm_set_pulse_dt(&servo, pulse_width);
	if (ret != 0) {
		printk("Error %d: failed to set pulse width\n", ret);
		return 0;
	}
	k_sleep(K_SECONDS(3));
	// SET THE SERVO PULSE PARAMETER ACCORDINGLY TO THE OVERLAY DEFINITION
	ret = pwm_set_pulse_dt(&servo, 0);
	if (ret != 0) {
		printk("Error %d: failed to set pulse width\n", ret);
		return 0;
	}

	while (1) {
		k_sleep(K_SECONDS(1));
	}

	return 0;
}
