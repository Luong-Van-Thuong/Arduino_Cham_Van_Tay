#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(4, 5);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nDang ky cam bien van tay");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Tim thay cam bien vay tay!");
  } else {
    Serial.println("Khong tim thay cam bien van tay :(");
    while (1) { delay(1); }
  }
  finger.getParameters();
}
uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available()); // Lặp đến khi có dữ liệu trong cổng
    num = Serial.parseInt(); // Đọc dữ liệu có sẵn trong cổng
  }
  return num;
}
void loop()                     // run over and over again
{
  Serial.println("San sang dang ky dau van tay!");
  Serial.println("Vui long nhap id (1 den 127) cho ngon tay muon luu...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("ID Dang ky #");
  Serial.println(id);
  while (!  getFingerprintEnroll() );
}
uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Dang cho ngon tay hop le #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();  // Chụp ảnh vân tay
  }
  p = finger.image2Tz(1);  
  Serial.println("Bo ngon tay, Vui long doi 2s roi de lai");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  p = finger.image2Tz(2); 
  p = finger.createModel();
  p = finger.storeModel(id);
  return true;
}
