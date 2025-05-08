#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// Cấu hình WiFi
#define WIFI_SSID "Thong"
#define WIFI_PASSWORD "09081969"

// Cấu hình Gmail
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "duongkhoi190504@gmail.com"
#define AUTHOR_PASSWORD "mbzt teut tyki zupd" // Mật khẩu ứng dụng
#define RECIPIENT_EMAIL "iamth4oo@gmail.com"

// Cấu hình chân GPIO
#define DHT11_PIN 23 
#define BUZZER_PIN 25 

// Ngưỡng nhiệt độ
#define TEMP_THRESHOLD 33.0

// Thời gian giữa các lần gửi email cảnh báo (milliseconds)
#define EMAIL_ALERT_INTERVAL 60000 // 30s

// Khởi tạo màn hình SH1106 với giao tiếp I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

DHT dht11(DHT11_PIN, DHT11);
SMTPSession smtp;

String temperature;
String humidity;

// Biến để kiểm soát việc gửi email
unsigned long lastEmailTime = 0;
bool isEmailSending = false;

// Hàm kết nối lại WiFi nếu mất kết nối
void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mất kết nối WiFi. Đang kết nối lại...");
    WiFi.disconnect();
    WiFi.reconnect();
    
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      
      // Timeout kết nối sau 10 giây
      if (millis() - startAttemptTime > 10000) {
        Serial.println("\nKhông thể kết nối WiFi. Khởi động lại...");
        ESP.restart();
      }
    }
    Serial.println("\nĐã kết nối WiFi");
  }
}

// Callback hàm gửi email
void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());

  if (status.success()) {
    Serial.println("Email gửi thành công!");
    lastEmailTime = millis();
  }
}

// Hàm gửi email cảnh báo với timeout
void sendTemperatureAlert(float temperature) {
  // Kiểm tra thời gian giữa các lần gửi email
  if (millis() - lastEmailTime < EMAIL_ALERT_INTERVAL) {
    return;
  }

  // Đảm bảo kết nối WiFi
  reconnectWiFi();

  // Ngăn chặn việc gửi email đồng thời
  if (isEmailSending) return;
  
  isEmailSending = true;
  
  smtp.debug(1);
  
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "Hệ Thống Giám Sát Nhiệt Độ";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "CẢNH BÁO: Nhiệt Độ Cao";
  
  String textMsg = "CẢNH BÁO: Nhiệt độ hiện tại là " + String(temperature, 1) + "°C - Đã vượt quá ngưỡng an toàn!";
  message.text.content = textMsg.c_str();

  message.addRecipient("Người Nhận", RECIPIENT_EMAIL);

  // Kết nối và gửi email
  if (!smtp.connect(&session)) {
    Serial.println("Lỗi kết nối SMTP");
    isEmailSending = false;
    return;
  }

  // Sử dụng callback và gửi email
  if (!MailClient.sendMail(&smtp, &message, smtpCallback)) {
    Serial.println("Lỗi gửi email");
  }
  
  isEmailSending = false;
}

void setup() {
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi");

  // Khởi tạo màn hình OLED
  oled.begin();
  oled.clearBuffer();  
  oled.setFont(u8g2_font_ncenB08_tr);  

  dht11.begin();  // Khởi tạo cảm biến DHT11

  pinMode(BUZZER_PIN, OUTPUT);  
  digitalWrite(BUZZER_PIN, LOW); 

  temperature.reserve(10);  
  humidity.reserve(10);     
}

void loop() {
  // Đảm bảo kết nối WiFi
  reconnectWiFi();
  
  float humi = dht11.readHumidity();      
  float tempC = dht11.readTemperature();  

  // Kiểm tra lỗi khi đọc dữ liệu
  if (isnan(humi) || isnan(tempC)) {
    temperature = "Failed";
    humidity = "Failed";
    
    // Reset cảm biến nếu đọc sai
    dht11.begin();
  } else {
    temperature = String(tempC, 1);  
    temperature += char(176);        
    temperature += "C";
    humidity = String(humi, 1);  
    humidity += "%";
  }

  Serial.print(tempC);  
  Serial.print("°C | ");
  Serial.print(humi);  
  Serial.println("%");

  // Kiểm tra nhiệt độ để bật Buzzer và gửi email
  if (tempC >= TEMP_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH); 
    sendTemperatureAlert(tempC);
  } else {
    digitalWrite(BUZZER_PIN, LOW);  
  }

  // Hiển thị dữ liệu trên OLED
  oledDisplayCenter(temperature, humidity);

  delay(2000);  
}

void buzzThreeBeeps() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH); 
    delay(200);                     
    digitalWrite(BUZZER_PIN, LOW);  
    delay(200);                     
  }
  delay(500); 
}

void oledDisplayCenter(String temperature, String humidity) {
  oled.clearBuffer();  

  // Hiển thị tiêu đề "Team Negav" ở giữa màn hình
  String title = "Team Negav";
  int16_t x = (128 - oled.getStrWidth(title.c_str())) / 2; 
  oled.setCursor(x > 0 ? x : 0, 12); 
  oled.print(title.c_str());

  // Hiển thị nhiệt độ ở giữa màn hình
  String tempText = "Nhiet do: " + temperature;  
  oled.setCursor((128 - oled.getStrWidth(tempText.c_str())) / 2, 32); 
  oled.print(tempText.c_str());

  // Hiển thị độ ẩm ở giữa màn hình
  String humText = "Do am: " + humidity;  
  oled.setCursor((128 - oled.getStrWidth(humText.c_str())) / 2, 52); 
  oled.print(humText.c_str());

  oled.sendBuffer();  
}