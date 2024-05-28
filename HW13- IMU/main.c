/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define ADDR _u(0b1101000)

//hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*---------setting up----------*/
void init() {
  uint8_t buf[2];
  uint8_t acc[2];
  uint8_t gyr[2];

  //send to register
  buf[0] = PWR_MGMT_1;
  buf[1] = 0x00; 

  acc[0] = ACCEL_CONFIG;
  acc[1] = 0b11000;

  gyr[0] = GYRO_CONFIG;
  gyr[1] = 0b11000;

  i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
  i2c_write_blocking(i2c_default, ADDR, acc, 2, false);
  i2c_write_blocking(i2c_default, ADDR, gyr, 2, false);

}

void set(char v){ 
  uint8_t buf[2];

  buf[0] = REG_OLAT; 
  buf[1] = v<<7; 
  i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

}

int read() { 
  uint8_t buf[1]; 
  uint8_t reg = REG_GPIO; 

  //Write GPIO register to the device
  i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);

  //Read the value from the GPIO regist
  i2c_read_blocking(i2c_default, ADDR, buf, 1, false);

  //Check the first bit of the read value to find state
  if ((buf[0] & 0x01) == 0x01) {
    return 1; //GPI0 is hight
  }
  else{
    return 0; //GPIo is low
  }
}

void data(){
  uint8_t buf[14];
  uint8_t reg = ACCEL_XOUT_H; 
//true to keep master control of bus
i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
//false finished with bus
i2c_read_blocking(i2c_default, ADDR, buf, 14, false);

//combine high and low for each sensor value
int16_t result[7];

for(int i=0; i <7; i++){
  result[i] = buf[i*2] << 8 | buf[i*2+1];
}

//print sensor values
for(int i=0; i<7; i++){
  if(i < 3){
    printf("Acceleration: %f g\n", result[i] * .000061);
  }
  else if(i==3){
    printf("Temperature: %f Celsius\n", result[i] / 340.0 + 36.53);
  }
  else{
    printf("Gyroscope: %f degrees/sec\n", result);
  }
}
printf("_________________________________\n");
printf("_________________________________\n");

}

void who_am_i() { 
  uint8_t buf[1];
  uint8_t reg = WHO_AM_I;
  i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
  i2c_read_blocking(i2c_default, ADDR, buf, 1, false);
}

/*------------- MAIN -------------*/
int main(void)
{
  board_init();
  tusb_init();
  stdio_init_all();
  i2c_init(i2c_default, 100000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  who_am_i();
  init();


  while (1)
  {
    tud_task(); // tinyusb device task
    led_blinking_task();

    hid_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
   //   int8_t const delta = 5;

     uint8_t buf[14];
     uint8_t reg = ACCEL_XOUT_H;
     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
     i2c_read_blocking(i2c_default, ADDR, buf, 14, false);
     

     int16_t result[7];
     for(int i=0; i<7; i++){
      result[i] = buf[i*2] << 8 | buf[i*2+1];
     }

     uint8_t x_val = 0;
     uint8_t y_val = 0;

    if (result[0]>= -.3 && result[0] <= .3){
      x_val = 0;
    }
    else if(result[0]>=.8){
      x_val = -5;
    }
    else if(result[0]<=-.8){
      x_val = 5;
    }
    else if (result[0]< -.8 && result[0] > -.3){
      x_val = -2;
    }
    else if (result[0]> -.8 && result[0] < -.3){
        x_val = 2;
    }


  if (result[0]>= -.3 && result[0] <= .3){
    y_val = 0;
  }
  else if(result[0]>=.8){
    y_val = -5;
  }
  else if(result[0]<=-.8){
    y_val = 5;
  }
  else if (result[0]< -.8 && result[0] > -.3){
    y_val = -2;
  }
  else if (result[0]> -.8 && result[0] < -.3){
      y_val = 2;
  }

 
     
     

      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, x_val, y_val, 0, 0);
   
   
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  // Remote wakeup
  if ( tud_suspended() && btn )
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }else
  {
    // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
    send_hid_report(REPORT_ID_KEYBOARD, btn);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1;

  if (next_report_id < REPORT_ID_COUNT)
  {
    send_hid_report(next_report_id, board_button_read());
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms) return;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
