#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Konfigurasi WiFi
const char* WIFI_SSID = "IOT DONE";
const char* WIFI_PASSWORD = "albiansyah";

// Token Bot dan Chat ID Telegram
const char* BOT_TOKEN = "7159718445:AAEzp5xqvI0TiT7G22G0HEagMn2GG3UAZ-o";
String chatId = "631469317";

// Deklarasi untuk Telegram bot
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Pin untuk relay dan sensor suara
const int relayPin = D1;  // Pin yang digunakan untuk mengendalikan relay (lampu)
const int soundSensorPin = A0;  // Pin yang digunakan untuk sensor suara

// Variabel untuk menyimpan status lampu
bool lampStatus = false;

void setup() {
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Memulai dengan lampu mati

  // Menghubungkan ke WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Memulai koneksi ke Telegram
  client.setInsecure(); // Menggunakan koneksi yang tidak aman
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < numNewMessages; i++) {
    chatId = bot.messages[i].chat_id;  // Menyimpan chat_id dari pesan terbaru
    String text = bot.messages[i].text;
    Serial.println(text);

    if (text == "on") {
      Serial.println("Perintah nyalakan lampu diterima");
      digitalWrite(relayPin, HIGH);
      lampStatus = true;
      bot.sendMessage(chatId, "Lampu telah dihidupkan!");
    } else if (text == "off") {
      Serial.println("Perintah matikan lampu diterima");
      digitalWrite(relayPin, LOW);
      lampStatus = false;
      bot.sendMessage(chatId, "Lampu telah dimatikan!");
    }
  }

  // Membaca nilai dari sensor suara
  int soundValue = analogRead(soundSensorPin);

  // Jika suara tepuk terdeteksi (nilai ambang batas bisa disesuaikan)
  if (soundValue > 200 ) { // Anda bisa menyesuaikan nilai ini sesuai dengan kebutuhan
    Serial.println("Suara tepuk terdeteksi");
    lampStatus = !lampStatus;  // Toggle status lampu
    digitalWrite(relayPin, lampStatus ? HIGH : LOW);
    bot.sendMessage(chatId, lampStatus ? "Lampu dihidupkan oleh suara tepuk!" : "Lampu dimatikan oleh suara tepuk!");
    delay(1000);  // Hindari multiple detection untuk satu tepukan
  }

  delay(1000);
}