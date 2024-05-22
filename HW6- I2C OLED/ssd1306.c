// based on adafruit and sparkfun libraries

#include <string.h> // for memset
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "font.h"

unsigned char SSD1306_ADDRESS = 0b0111100; // 7bit i2c address
unsigned char ssd1306_buffer[513]; // 128x32/8. Every bit is a pixel except first byte

void ssd1306_setup() {
    // first byte in ssd1306_buffer is a command
    ssd1306_buffer[0] = 0x40;
    // give a little delay for the ssd1306 to power up
    //_CP0_SET_COUNT(0);
    //while (_CP0_GET_COUNT() < 48000000 / 2 / 50) {
    //}
    sleep_ms(20);
    ssd1306_command(SSD1306_DISPLAYOFF);
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1306_command(0x80);
    ssd1306_command(SSD1306_SETMULTIPLEX);
    ssd1306_command(0x1F); // height-1 = 31
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);
    ssd1306_command(0x0);
    ssd1306_command(SSD1306_SETSTARTLINE);
    ssd1306_command(SSD1306_CHARGEPUMP);
    ssd1306_command(0x14);
    ssd1306_command(SSD1306_MEMORYMODE);
    ssd1306_command(0x00);
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);
    ssd1306_command(0x02);
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(0x8F);
    ssd1306_command(SSD1306_SETPRECHARGE);
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYON);
    ssd1306_clear();
    ssd1306_update();
}

// send a command instruction (not pixel data)
void ssd1306_command(unsigned char c) {
    //i2c_master_start();
    //i2c_master_send(ssd1306_write);
    //i2c_master_send(0x00); // bit 7 is 0 for Co bit (data bytes only), bit 6 is 0 for DC (data is a command))
    //i2c_master_send(c);
    //i2c_master_stop();

    uint8_t buf[2];
    buf[0] = 0x00;
    buf[1] =c;
    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, buf, 2, false);
}

// update every pixel on the screen
void ssd1306_update() {
    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0);
    ssd1306_command(0xFF);
    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);
    ssd1306_command(128 - 1); // Width

    unsigned short count = 512; // WIDTH * ((HEIGHT + 7) / 8)
    unsigned char * ptr = ssd1306_buffer; // first address of the pixel buffer
    /*
    i2c_master_start();
    i2c_master_send(ssd1306_write);
    i2c_master_send(0x40); // send pixel data
    // send every pixel
    while (count--) {
        i2c_master_send(*ptr++);
    }
    i2c_master_stop();
    */

    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, ptr, 513, false);
}

// set a pixel value. Call update() to push to the display)
void ssd1306_drawPixel(unsigned char x, unsigned char y, unsigned char color) {
    if ((x < 0) || (x >= 128) || (y < 0) || (y >= 32)) {
        return;
    }

    if (color == 1) {
        ssd1306_buffer[1 + x + (y / 8)*128] |= (1 << (y & 7));
    } else {
        ssd1306_buffer[1 + x + (y / 8)*128] &= ~(1 << (y & 7));
    }
}

// zero every pixel value
void ssd1306_clear() {
    memset(ssd1306_buffer, 0, 512); // make every bit a 0, memset in string.h
    ssd1306_buffer[0] = 0x40; // first byte is part of command
}

void drawChar(int x, int y, char c) {
    
    if (c < 32 || c > 126) return;  // Check if its printable

    int index = c - 32;  //Change the index
    for (int col = 0; col < 5; col++) {
        char col_data = ASCII[index][col];

        for (int row = 0; row < 8; row++) {
            // check to draw pixel
            if (col_data & (1 << row)) {
                ssd1306_drawPixel(x + col, y + row, 1);  // Draw pixel
            } else {
                ssd1306_drawPixel(x + col, y + row, 0);  // Clear pixel
            }

        }
    }
}

void drawStr(int x, int y, const char * s) {
    int parse = 0; 
    while(s[parse]){
        drawChar(x + parse * 5, y, s[parse]);
        parse++;
    }

    ssd1306_update();
    
}

#define LED_PIN 25

int main() {
    // Initialize the LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

     // Initialize standard I/O
    stdio_init_all();

    // Initialize I2C on GP16 (SDA) and GP17 (SCL)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(16, GPIO_FUNC_I2C);
    gpio_set_function(17, GPIO_FUNC_I2C);
    gpio_pull_up(16);
    gpio_pull_up(17); 

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);



    ssd1306_setup();




    bool led_state = false; 
    bool pixel_state = false;

    char message[50];
    char timer_message[50];
    int counter = 0; 

while (1) {

        unsigned int start_time = to_us_since_boot(get_absolute_time());

        uint16_t val = adc_read();
        float con_val = (float)val/1212.0; 
        gpio_put(LED_PIN, 1);  // Turn on LED
        sprintf(message, "ADC is %f", con_val);
        drawStr(10, 10, message);
        counter++;

        unsigned int end_time = to_us_since_boot(get_absolute_time());      
        unsigned int total_time = end_time - start_time; 
        float con_time = 1000000.0/(float)total_time; 
        sprintf(timer_message, "Total Time is  %f fps", con_time);
        drawStr(10, 20, timer_message);
        sleep_ms(250);

        gpio_put(LED_PIN, 0); 
        if (counter == 10){
            counter = 0;
        }

    }

    return 0; 

}
