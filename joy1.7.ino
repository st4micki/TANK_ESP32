#include <esp_now.h>
#include <WiFi.h>
//78:21:84:45:8E:F8 - receiver address
//40:91:51:FC:CA:A4 - transmitter address
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0x45, 0x8E, 0xF8};
const int y_adc_pin = 33;
const int x_adc_pin = 32;
const int x2_adc_pin = 35;

const int switch_pin = 25;
int y_movement;
int x_movement;
int servo_movement;
char function;
char servo_function;
const uint8_t BUTTON = 18;
const uint8_t LED = 2;
bool button_flag = false;
typedef struct struct_message{
  char movement_data;
  char servo_data;
  bool light_status = false;
} struct_message;
struct_message messageData;

esp_now_peer_info_t peerInfo;

void IRAM_ATTR isr(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > 200){
    messageData.light_status = !messageData.light_status;
    digitalWrite(LED, !digitalRead(LED));
    last_interrupt_time = interrupt_time;
  }

  //if(times_pressed % 2 == 0){
  //  digitalWrite(LED, HIGH);
 // }
 // else{
//  digitalWrite(LED,LOW);
 // }
 
 
}



void forward(){
  Serial.print("function: forward\n");
}

void backward(){
  Serial.print("function: backward\n");
}

void stop(){
  Serial.print("function: STOP\n");
}

void left(){
  Serial.print("function: left\n");
}

void right(){
  Serial.print("function: right\n");
}

void joy_test(int *x, int *y){
  Serial.print("\nPot0(y_axis)| analog:  ");
  Serial.print(*y);
  Serial.print(" |Pot1(x_axis)| analog: ");
  Serial.print(*x);
  
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, isr, FALLING);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("ESP-NOW initialized");
  }
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  else{
    Serial.print("Peer added. MAC:");
    for(int i  =  0; i < sizeof(broadcastAddress)/sizeof(broadcastAddress[0]); i++){
      Serial.print(broadcastAddress[i]);
    }
    Serial.print("\n");
  }  

}




void loop() {
  y_movement = analogRead(y_adc_pin);
  x_movement = analogRead(x_adc_pin);
  servo_movement = analogRead(x2_adc_pin);  
  if (1000< x_movement && x_movement < 2600){
    if(y_movement > 2600){
      function = 'w';
    }
    else if (y_movement < 1000){
      function = 's';
    }
    else{
      function = 'x';
    }
  }
  else{
    if(1000< y_movement && y_movement < 2600){
      if(x_movement > 2600){
        function = 'a';
      }
      else if (x_movement < 1000){
        function = 'd';
      }
      else{
        function = 'x';
    }
    }
  }

  if(servo_movement > 2600){
      servo_function = 'q';
    }
    else if (servo_movement < 1000){
      servo_function = 'e';
    }
    else{
      servo_function = 'n';
  }
    
  //if(toggle_light){
  //  messageData.light_status = 1;
 // }
 // else{
 //   messageData.light_status = 0;
 // }

  messageData.movement_data = function;
  messageData.servo_data = servo_function;
  esp_now_send(broadcastAddress, (uint8_t *) &messageData, sizeof(messageData));
  Serial.print(function);
  Serial.print(" ");
  Serial.print(servo_function);
  Serial.print("\n");

  delay(10);
}