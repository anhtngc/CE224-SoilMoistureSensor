#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// Khởi tạo màn hình SH1106 với giao tiếp I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

#define DHT11_PIN 23 // Chân kết nối DHT11

DHT dht11(DHT11_PIN, DHT11);

String temperature;
String humidity;

void setup() {
  Serial.begin(9600);

  // Khởi tạo màn hình OLED
  oled.begin();
  oled.clearBuffer();  // Xóa bộ đệm
  oled.setFont(u8g2_font_ncenB08_tr);  // Đặt font chữ

  dht11.begin();  // Khởi tạo cảm biến DHT11

  temperature.reserve(10);  // Tối ưu bộ nhớ
  humidity.reserve(10);     // Tối ưu bộ nhớ
}

void loop() {
  float humi = dht11.readHumidity();      // Đọc độ ẩm
  float tempC = dht11.readTemperature();  // Đọc nhiệt độ

  // Kiểm tra lỗi khi đọc dữ liệu
  if (isnan(humi) || isnan(tempC)) {
    temperature = "Failed";
    humidity = "Failed";
  } else {
    temperature = String(tempC, 1);  // 1 chữ số thập phân
    temperature += char(176);        // Ký tự độ °
    temperature += "C";
    humidity = String(humi, 1);  // 1 chữ số thập phân
    humidity += "%";
  }

  Serial.print(tempC);  // Hiển thị nhiệt độ trên Serial Monitor
  Serial.print("°C | ");
  Serial.print(humi);  // Hiển thị độ ẩm trên Serial Monitor
  Serial.println("%");

  // Hiển thị dữ liệu trên OLED
  oledDisplayCenter(temperature, humidity);

  delay(2000);  // Đợi 2 giây
}

void oledDisplayCenter(String temperature, String humidity) {
  oled.clearBuffer();  // Xóa nội dung cũ

  // Hiển thị tiêu đề "Team Negav" ở giữa màn hình
  String title = "Team Negav";
  int16_t x = (128 - oled.getStrWidth(title.c_str())) / 2; // Căn giữa theo chiều ngang
  oled.setCursor(x > 0 ? x : 0, 12); // Nếu x âm, đặt x=0 để không bị cắt chữ
  oled.print(title.c_str());

  // Hiển thị nhiệt độ ở giữa màn hình
  String tempText = "Nhiet do: " + temperature;  // Tạo chuỗi nhiệt độ
  oled.setCursor((128 - oled.getStrWidth(tempText.c_str())) / 2, 32); // Căn giữa theo chiều ngang
  oled.print(tempText.c_str());

  // Hiển thị độ ẩm ở giữa màn hình
  String humText = "Do am: " + humidity;  // Tạo chuỗi độ ẩm
  oled.setCursor((128 - oled.getStrWidth(humText.c_str())) / 2, 52); // Căn giữa theo chiều ngang
  oled.print(humText.c_str());

  oled.sendBuffer();  // Cập nhật màn hình
}