#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "iphone13";        // 🔁 your hotspot name
const char* password = "nikhils23j";  // 🔁 your password

// Telegram Bot credentials
String botToken = "8084528112:AAGIYHP5aF9EZV2S-4Z9y9jj-2M5z8gNLe0";  // 🔁 your bot token
String chatID = "1519496749";                                       // 🔁 your chat ID

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Send Telegram message
    sendTelegramMessage("👋 Hi from ESP32!");
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void loop() {
  // Nothing in loop
}

void sendTelegramMessage(String message) {
  // URL encode the message
  String safeMessage = urlencode(message);

  // Prepare the URL
  String url = "https://api.telegram.org/bot" + botToken +
               "/sendMessage?chat_id=" + chatID + "&text=" + safeMessage;

  // Send the HTTP GET request to Telegram API
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    Serial.println("Message sent! HTTP code: " + String(httpCode));
  } else {
    Serial.println("Failed to send message. HTTP error: " + String(httpCode));
  }
  http.end();
}

String urlencode(String str) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}
