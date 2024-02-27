//DRIVER
#include <esp_now.h>
#include <WiFi.h>
const int ENA = 21;
const int IN1 = 22;
const int IN2 = 23;
const int IN3 = 19;
const int IN4 = 18;
const int PWM_pin = 5;
char function_received;
char servo_function_received;
bool toggle_light;
const uint8_t LED = 32;
typedef struct struct_message{
  char movement_data;
  char servo_data;
  bool light_status;
} struct_message;

struct_message messageData;
struct_message board1;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len){
  //Serial.print("Packet received from: ");
  memcpy(&messageData, incomingData, sizeof(messageData));
  //Serial.printf("Board ID %u: %u bytes\n", messageData.id, len);
  board1.movement_data = messageData.movement_data;
  board1.light_status = messageData.light_status;
  board1.servo_data = messageData.servo_data;
  //Serial.printf("x value: %f \n", boardsStruct[messageData.id-1].data);
  function_received = board1.movement_data;
  toggle_light = board1.light_status;
  servo_function_received = board1.servo_data;
}

void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
 // Serial.print("function: forward\n");
}

void backward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
  //Serial.print("function: backward\n");
}

void stop(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  //Serial.print("function: STOP\n");
}

void left(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  //Serial.print("function: left\n");
}

void right(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH);
  //Serial.print("function: right\n");
}

void servo_right(){
  ledcWrite(0, 4700);
}
void servo_left(){
  ledcWrite(0, 5000);
}
void servo_stop(){
  ledcWrite(0, 0);
}

void setup(){
  Serial.begin(115200);
  ledcAttachPin(PWM_pin, 0);
  ledcSetup(0, 50, 16);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);

  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("ESP-NOW initialized");
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){

  switch (function_received)
  {
    case 'w':
      forward();
      break;
    case 's':
      backward();
      break;
    case 'a':
      left();
      break;
    case 'd':
      right();
      break;
    case 'x':
      stop();
      break;
  }
  switch (toggle_light)
  {
    case false:
      digitalWrite(LED, LOW);;
      break;
    case true:
      digitalWrite(LED, HIGH);
      break;
  }

  switch (servo_function_received)
  {
    case 'q':
      servo_left();
      break;
    case 'e':
      servo_right();
      break;
    case 'n':
      servo_stop();
      break;
  }


  Serial.print(function_received);
  Serial.print(servo_function_received);
  Serial.print(toggle_light);
  Serial.print("\n");
  function_received = 'x';
  servo_function_received = 'n';
  toggle_light = false;
  delay(30);
}