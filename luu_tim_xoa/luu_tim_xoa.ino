#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "VIETTEL_2,4G"
#define WIFI_PASSWORD "Wifi@123#_789"
/* 2. Define the API Key */
#define API_KEY "5mfddlCxx4dQcaxl3FeVOnrY604AHN0loHvZEHXZ"
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://da5-hieu-default-rtdb.firebaseio.com/" 
FirebaseData firebaseData;


#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(4, 5);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id, chonMuc;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Nhap 1 Them, 2 Xoa van tay");
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Tim thay thiet bi van tay!");
  } else {
    Serial.println("Khong tim thay thiet bi lay van tay :(");
    while (1) { delay(1); }
  }
  // finger.getParameters(); // Lấy thông số từ id cảm biến
  // finger.getTemplateCount(); // Lấy số lượng mẫu từ cảm biến vân tay
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(DATABASE_URL, API_KEY);
}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available()); // Lặp đến khi có dữ liệu trong cổng
    num = Serial.parseInt(); // Đọc dữ liệu có sẵn trong cổng
  }
  return num;
}
uint8_t readnumber1(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available()); // Lặp đến khi có dữ liệu trong cổng
    num = Serial.parseInt(); // Đọc dữ liệu có sẵn trong cổng
  }
  return num;
}

void loop() {
  // put your main code here, to run repeatedly:
  Firebase.setInt(firebaseData, "/ttChamCong", 0);
  Serial.print("\nNhap 1 Them, 2 Xoa, 3 Tim van tay");
  chonMuc = readnumber1();
  if(chonMuc == 1){
    Serial.print("\nDat ngon tay muon them voi "); Serial.println(id);
    id = readnumber();
    Serial.print("ID Dang ky #");
    Serial.println(id);  
    while (!  getFingerprintEnroll() );

  } else if( chonMuc == 2){
    int attempts = 0;
    Serial.print("\nDat ngon tay muon kiem tra");
    int fingerID  = getFingerprintIDez();
    if (fingerID == -1) {
      Serial.println("\nKhông tìm thấy ID. Vui lòng thử lại.");
      attempts++;
    } else {
      Serial.print("Đã tìm thấy ID người dùng: ");
      Serial.println(fingerID);
    }    
  } else if(chonMuc == 3){
      Firebase.getInt(firebaseData, "/idXoa");
      uint8_t idXoa = firebaseData.intData();
  }
}
// Ham them dau van tay
uint8_t getFingerprintEnroll() {
  int p = -1;
  // Serial.print("\nCho ngoi tay can lay van #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();  // Chụp ảnh vân tay
    }
  p = finger.image2Tz(1);  
  Serial.println("\nTam thoi bo ngon tay ra khoi 2s");
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
  Serial.print("\nThem van tay thanh cong"); 
  Firebase.setInt(firebaseData, "/idThem", p);
  return true;
}
// Ham tìm kiem dau van tay 
int getFingerprintIDez() {
  Serial.print("\nHam 2 chay"); 
  delay(1000);
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("\nTim thay ID #"); Serial.print(finger.fingerID);
  Serial.print(" do tin cay "); Serial.println(finger.confidence);
  Firebase.setInt(firebaseData, "/ttChamCong", 1);
  Firebase.setInt(firebaseData, "/idChamCong", 1);
  return finger.fingerID;
}
// Ham xoa dau van tay
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  p = finger.deleteModel(id);
  Serial.println("\nXoa thanh cong");
  Serial.println(id);
  return p;
}














