#define BLYNK_TEMPLATE_ID "TMPL61s9TVlbR"
#define BLYNK_TEMPLATE_NAME "Do am nhiet do"
#define BLYNK_AUTH_TOKEN "HW1vi9X4QtapGv_bmB4OCXFY7Y7JiENh"

#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <BlynkSimpleEsp32.h>

// Cấu hình WiFi
#define WIFI_SSID "Digapdeha"
#define WIFI_PASSWORD "1234678910"

// Cấu hình Gmail
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "duongkhoi190504@gmail.com"
#define AUTHOR_PASSWORD "mbzt teut tyki zupd"
#define RECIPIENT_EMAIL "22521583@gm.uit.edu.vn"

// Cấu hình chân GPIO
#define DHT11_PIN 23 
#define BUZZER_PIN 25 

// Ngưỡng nhiệt độ
#define TEMP_THRESHOLD 32.5

// Thời gian giữa các lần gửi email cảnh báo (milliseconds)
#define EMAIL_ALERT_INTERVAL 60000

// Khởi tạo màn hình SH1106 với giao tiếp I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

DHT dht11(DHT11_PIN, DHT11);
SMTPSession smtp;
BlynkTimer timer; 
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

      if (millis() - startAttemptTime > 20000) {
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

// Hàm gửi email cảnh báo
void sendTemperatureAlert(float temperature) {
  if (millis() - lastEmailTime < EMAIL_ALERT_INTERVAL) {
    return;
  }

  reconnectWiFi();

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

  if (!smtp.connect(&session)) {
    Serial.println("Lỗi kết nối SMTP: " + smtp.errorReason());
    isEmailSending = false;
    return;
  }

  if (!MailClient.sendMail(&smtp, &message, smtpCallback)) {
    Serial.println("Lỗi gửi email");
  }

  isEmailSending = false;
}

// Hàm điều khiển Buzzer cảnh báo
void buzzThreeBeeps() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH); 
    delay(200);                     
    digitalWrite(BUZZER_PIN, LOW);  
    delay(200);                     
  }
}

// Hiển thị dữ liệu trên OLED
void oledDisplayCenter(String temperature, String humidity) {
  oled.clearBuffer();

  String title = "Team Negav";
  int16_t x = (128 - oled.getStrWidth(title.c_str())) / 2; 
  oled.setCursor(x > 0 ? x : 0, 12); 
  oled.print(title.c_str());

  String tempText = "Nhiet do: " + temperature;  
  oled.setCursor((128 - oled.getStrWidth(tempText.c_str())) / 2, 32); 
  oled.print(tempText.c_str());

  String humText = "Do am: " + humidity;  
  oled.setCursor((128 - oled.getStrWidth(humText.c_str())) / 2, 52); 
  oled.print(humText.c_str());

  oled.sendBuffer();
}

// Hàm đọc cảm biến và gửi dữ liệu đến Blynk
void sendSensorData() {
  float humi = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Lỗi đọc cảm biến!");
    return;
  }

  Blynk.virtualWrite(V0, tempC);
  Blynk.virtualWrite(V1, humi);

  temperature = String(tempC, 1) + String((char)176) + "C";
  humidity = String(humi, 1) + "%";

  oledDisplayCenter(temperature, humidity);

  if (tempC >= TEMP_THRESHOLD) {
    buzzThreeBeeps();
    sendTemperatureAlert(tempC);
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi");

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB08_tr);

  dht11.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  reconnectWiFi();
  Blynk.run();
  timer.run();
}