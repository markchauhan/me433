/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_PIN 16

int main() {
    /// \tag::setup_pwm[]

    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN); // Get PWM slice number
    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
   
    while(true){
        pwm_set_gpio_level(PWM_PIN, wrap / 40); // set the duty cycle to 50%
        sleep_ms(2000);
        pwm_set_gpio_level(PWM_PIN, wrap / 8);  // set the duty cycle 
        sleep_ms(2000);

    }
}
