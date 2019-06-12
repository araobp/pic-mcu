#include <Wire.h>

int LED_GREEN = 5;
int LED_BLUE = 4;
int LED_RED = 2;

int E2_0 = 0x53;
int E2_1 = 0x57;

void dump(uint8_t dev_addr, uint8_t reg_addr_msb, uint8_t reg_addr_lsb, uint8_t len, bool hex) {
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr_msb);
  Wire.write(reg_addr_lsb);
  Wire.endTransmission(false);
  Wire.requestFrom((int)dev_addr, (int)len, true);
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

uint8_t read_one_byte(uint8_t dev_addr, uint8_t reg_addr_msb, uint8_t reg_addr_lsb) {
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr_msb);
  Wire.write(reg_addr_lsb);
  Wire.endTransmission(false);
  Wire.requestFrom((int)dev_addr, 0x01, true);
  return Wire.read();
}

void write(uint8_t dev_addr, uint8_t reg_addr_msb, uint8_t reg_addr_lsb, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr_msb);
  Wire.write(reg_addr_lsb);
  for (int i=0; i < len; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission(true);
}

void dump_system_config(void) {
  dump(E2_1, 0x00, 0x00, 0x20, true);
}

void present_password(void) {
  // Default password is {0, 0, 0, 0, 0, 0, 0, 0}
  uint8_t data[17] = {0, 0, 0, 0, 0, 0, 0, 0, 0x09, 0, 0, 0, 0, 0, 0, 0, 0};
  write(E2_1, 0x09, 0x00, data, 17);
}

void dump_password(void) {
  dump(E2_1, 0x09, 0x00, 8, true);
}

void dump_dynamic_config(void) {
  dump(E2_0, 0x20, 0x00, 8, true);
}

void dump_CC_file(void) {
  // In case of ST25DV04K, it should be "E1 40 40 00h" (Application Note: AN4911)
  dump(E2_0, 0x00, 0x00, 0x04, true);
}

int dump_NDEF_message_type(void) {
  dump(E2_0, 0x00, 0x04, 1, true); 
}

int dump_NDEF_message_length(void) {
  dump(E2_0, 0x00, 0x05, 1, true);   
}

int dump_NDEF_header(void) {
  dump(E2_0, 0x00, 0x06, 4, true); 
}

void dump_NDEF_payload_length(void) {
  dump(E2_0, 0x00, 0x09, 1, true);   
}

void dump_NDEF_payload_identifier_code(void) {
  dump(E2_0, 0x00, 0x0a, 1, true);   
}

void dump_NDEF_payload_uri_field(void) {
  uint8_t len = read_one_byte(E2_0, 0x00, 0x08) - 1;
  int repeat = len / 32;  // Wire lib's buffer size
  for (int i=0; i < repeat; i++) {
    dump(E2_0, 0x00, 0x0b+i*32, 32, false);
  }
  dump(E2_0, 0x00, 0x0b+repeat*32, len-32*repeat, false);    
}

// Write HTTPS URL
void write_URL(char *pUrl) {
  uint8_t CC_HEADER[] = { 0xE1, 0x40, 0x40, 0x00, 0x03, 0x00, 0xD1, 0x01, 0x00, 0x55 };
  uint8_t HTTPS = 0x04;
  uint8_t len = strlen(pUrl) + 1; // Including "HTTPS"
  uint8_t total_len = len + 10;
  uint8_t buf[256];

  CC_HEADER[5] = len + 4;
  CC_HEADER[8] = len;

  memcpy(buf, CC_HEADER, 10);
  buf[10] = HTTPS;
  memcpy(buf+11, pUrl, len-1);
  
  // TODO: Why 30 for write??? 32 did not work.
  int repeat = total_len / 30;  // Wire lib's buffer size.
  for (int i=0; i < repeat; i++) {
    write(E2_0, 0x00, 0x00+i*30, buf+i*30, 30);      
    delay(100);  // TODO: polling    
  }
  write(E2_0, 0x00, 0x00+repeat*30, buf+repeat*30, total_len-30*repeat);
  delay(100);  // TODO: polling    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );
  pinMode( LED_GREEN, OUTPUT );  // GREEN
  pinMode( LED_BLUE, OUTPUT );  // BLUE
  pinMode( LED_RED, OUTPUT );  // RED

  Wire.begin();
  Wire.setClock(400000);

  present_password();

  char url[] = "github.com/araobp/pic16f1-mcu/blob/master/BLINKERS.md";
  write_URL(url);
  
  Serial.print("ST25 system config: ");
  dump_system_config();
  Serial.print("ST25 password: ");
  dump_password();
  Serial.print("ST25 dynamic config: ");
  dump_dynamic_config();
  Serial.print("NDEF CC file: ");
  dump_CC_file();
  Serial.print("NDEF message type: ");
  dump_NDEF_message_type();
  Serial.print("NDEF message length: ");
  dump_NDEF_message_length();
  Serial.print("NDEF header: ");
  dump_NDEF_header();
  Serial.print("NDEF payload length: ");
  dump_NDEF_payload_length();  
  Serial.print("NDEF Identifier code: ");
  dump_NDEF_payload_identifier_code();
  Serial.println("NDEF URI field:");
  dump_NDEF_payload_uri_field();
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
