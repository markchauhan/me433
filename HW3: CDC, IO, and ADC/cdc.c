#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    
    //initializing pins
    gpio_init(16); //21 is I/O
    gpio_set_dir(16, GPIO_OUT); //output set as direction
    gpio_put(16, 1); //led on
    
    while (gpio_get(15) == 0){
        //wait for button press
    }

    gpio_put(16, 0); //led off

    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0


    while (1) {
        printf("Enter a number of analog samples to take: \n");
        char samples[100];
        scanf("%d", samples);

        for(int i = 0; i < *samples; i++){
            uint16_t adc_value = adc_read();

            float conv_adc_value = (float) adc_value / 1212;
            printf("Sample %d \n", i);
            printf("%f V\n", conv_adc_value);
            sleep_ms(10);
        }

    
    }


}