#include <Wire.h>

int LED_GREEN = 5;
int LED_BLUE = 4;
int LED_RED = 2;

int E2_0 = 0x53;
int E2_1 = 0x57;

void read(int dev_addr, int reg_addr_msb, int reg_addr_lsb, int len, bool hex) {
  Wire.beginTransmission( dev_addr );
  Wire.write( reg_addr_msb );
  Wire.write( reg_addr_lsb );
  Wire.endTransmission( false );
  int uid = Wire.requestFrom( dev_addr, len, true );
  for (int i=0; i < len; i++) {
    if (hex) {
      int d = Wire.read();
      Serial.print(d, HEX);
      Serial.print(' ');
    } else {
      char c = Wire.read();
      Serial.print(c);
    }
  }
  Serial.println();  
}

void write(int dev_addr, int reg_addr_msb, int reg_addr_lsb, int *data, int len) {
  Wire.beginTransmission( dev_addr );
  Wire.write( reg_addr_msb );
  Wire.write( reg_addr_lsb );
  for (int i=0; i < len; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission( true );
}

void dump_system_config(void) {
  read(E2_1, 0x00, 0x00, 0x20, true);
}

void present_password(void) {
  // Default password is {0, 0, 0, 0, 0, 0, 0, 0}
  int data[17] = {0, 0, 0, 0, 0, 0, 0, 0, 0x09, 0, 0, 0, 0, 0, 0, 0, 0};
  write(E2_1, 0x09, 0x00, data, 17);
}

void dump_password(void) {
  read(E2_1, 0x09, 0x00, 8, true);
}

void dump_dynamic_config(void) {
  read(E2_0, 0x20, 0x00, 8, true);
}

void dump_CC_file(void) {
  // In case of ST25DV04K, it should be "E1 40 40 00h" (Application Note: AN4911)
  read(E2_0, 0x00, 0x00, 0x04, true);
}

int dump_NDEF_header(void) {
  // TNF: 3, 
  read(E2_0, 0x00, 0x04, 6, true); 
}

void dump_NDEF_payload(int len) {
  if (len > 32) {
    int repeat = len / 32;
    for (int i=0; i < repeat; i++) {
      read(E2_0, 0x00, 0x0b+i*32, 32, false);
    }
    read(E2_0, 0x00, 0x0b+repeat*32, len-32*repeat, false);    
  } else {
    read(E2_0, 0x00, 0x0b, len, false);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );
  pinMode( LED_GREEN, OUTPUT );  // GREEN
  pinMode( LED_BLUE, OUTPUT );  // BLUE
  pinMode( LED_RED, OUTPUT );  // RED
  Wire.begin();
  dump_system_config();
  present_password();
  dump_password();
  dump_dynamic_config();
  dump_CC_file();
  dump_NDEF_header();
  dump_NDEF_payload(0x47);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite( LED_GREEN, HIGH );
  delay(300);
  digitalWrite( LED_BLUE, HIGH );
  delay(300);
  digitalWrite( LED_RED, HIGH );
  delay(300);
  digitalWrite( LED_GREEN, LOW );
  delay(300);
  digitalWrite( LED_BLUE, LOW );
  delay(300);
  digitalWrite( LED_RED, LOW );
  delay(300);
}
