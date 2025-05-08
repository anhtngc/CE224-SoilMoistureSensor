#define BLYNK_TEMPLATE_ID "TMPL6dR8yMeZT"
#define BLYNK_TEMPLATE_NAME "ali alo ola"
#define BLYNK_AUTH_TOKEN "EJymJE6-BPKTqM9wleM85dIBiJ3i0hN3"


#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// Khởi tạo màn hình SH1106 với giao tiếp I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// Thông tin WiFi
char ssid[] = "Digapdeha";         // Tên WiFi
char pass[] = "1234678910";     // Mật khẩu WiFi


// Chân cảm biến DHT11
#define DHT11_PIN 23
DHT dht11(DHT11_PIN, DHT11);

BlynkTimer timer; // Tạo timer cho Blynk

// Hàm hiển thị dữ liệu trên OLED
void oledDisplayCenter(String temperature, String humidity) {
  oled.clearBuffer(); // Xóa nội dung cũ

  // Hiển thị tiêu đề
  oled.setCursor((128 - oled.getStrWidth("Team Negav")) / 2, 10);
  oled.print("Team Negav");

  // Hiển thị nhiệt độ
  oled.setCursor(0, 30);
  oled.print("Nhiet do: ");
  oled.print(temperature.c_str());

  // Hiển thị độ ẩm
  oled.setCursor(0, 50);
  oled.print("Do am: ");
  oled.print(humidity.c_str());

  oled.sendBuffer(); // Cập nhật màn hình
}

// Hàm gửi dữ liệu cảm biến tới Blynk
void sendSensorData() {
  float humi = dht11.readHumidity();      // Đọc độ ẩm
  float tempC = dht11.readTemperature();  // Đọc nhiệt độ

  // Nếu không đọc được dữ liệu từ cảm biến
  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Lỗi đọc cảm biến!");
    return;
  }

  // Gửi dữ liệu tới Blynk
  Blynk.virtualWrite(V0, tempC); // Gửi nhiệt độ tới V0
  Blynk.virtualWrite(V1, humi);  // Gửi độ ẩm tới V1

  // Hiển thị dữ liệu trên OLED
  String temperature = String(tempC, 1) + char(176) + "C"; // Nhiệt độ với ký tự °
  String humidity = String(humi, 1) + "%";                // Độ ẩm với ký tự %
  oledDisplayCenter(temperature, humidity);

  // Ghi log ra Serial Monitor
  Serial.print("Nhiet do: ");
  Serial.print(tempC);
  Serial.print("°C, Do am: ");
  Serial.print(humi);
  Serial.println("%");
}

void setup() {
  Serial.begin(9600);

  // Khởi tạo OLED
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB08_tr); // Đặt font chữ

  // Kết nối WiFi và Blynk
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Khởi tạo cảm biến DHT11
  dht11.begin();

  // Đặt lịch gửi dữ liệu mỗi 2 giây
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();  // Chạy Blynk
  timer.run();  // Chạy Timer
}