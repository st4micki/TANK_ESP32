ESP32 applications for a mobile wheeled robot. The project requires 2 ESP32 microcontrollers with built in WiFi modules
Features:
 - L298N DC motor driver throuh GPIO ports control
 - servo motor 16-bit PWM control
 - 3 channel ADC support for 2 joysticks
 - joystick button support through external interrupt
 - Wireless communication thtougs ESP-NOW protocol

Board 1 pinout:
y_adc_pin - GPIO33;
x_adc_pin - GPIO32;
x2_adc_pin - GPIO35;
switch_pin - GPIO25;

Board 2 pinout:
ENA - GPIO21;
IN1 - GPIO22;
IN2 - v23;
IN4 - GPIO18;
PWM_pin - GPIO5;
