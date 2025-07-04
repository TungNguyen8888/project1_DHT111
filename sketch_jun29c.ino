#include <ESP8266WiFi.h>
#include<DHT.h>
#define DHTPIN 2       // GPIO2 (D4 trên NodeMCU)
#define DHTTYPE DHT11  // Chọn DHT11 hoặc DHT22 nếu bạn dùng loại khác

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "134628";         // Thay bằng WiFi của bạn
const char* password = "tung310304";
const char* host = "api.thingspeak.com";
const char* apiKey = "N4WWV4RORELXD1OV";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("Connection failed");
    return;
  }

  // Gửi nhiệt độ lên field1 và độ ẩm lên field2
  String url = "/update?api_key=" + String(apiKey) +
               "&field1=" + String(temperature) +
               "&field2=" + String(humidity);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Sent: Temp = " + String(temperature) +
                 "°C, Humidity = " + String(humidity) + "%");

  delay(15000);  // chờ 15 giây để ThingSpeak xử lý
}
