#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <time.h>

// Wi-Fi credentials
const char* ssid = "Redmi Note 11 Pro+ 5G";
const char* password = "@polu1411P";

// Gmail SMTP settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "prolayjitbiswas14112004@gmail.com"
#define AUTHOR_PASSWORD "lspi lxjl lkcb jajj"

// Multiple recipients
#define RECIPIENT1 "prolayjitbiswas14112004@gmail.com"
#define RECIPIENT2 "bubun15072006@gmail.com"
#define RECIPIENT3 "ankanbhowmik11@gmail.com"

SMTPSession smtp;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");

  // Use direct IPs to avoid DNS issues (esp. on hotspot)
  configTime(0, 0, "132.163.96.1", "129.6.15.28"); 

  Serial.print("⏳ Waiting for NTP sync");

  time_t now = time(nullptr);
  unsigned long startAttemptTime = millis();
  while (now < 8 * 3600 * 2 && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  if (now < 8 * 3600 * 2) {
    Serial.println("\n❌ Time sync failed. No internet?");
    return;
  }
  Serial.println("\n✅ Time synced: " + String(ctime(&now)));

  // Send the email
  sendEmailAlert();
}

void sendEmailAlert() {
  SMTP_Message message;

  // Sender info
  message.sender.name = "ESP32 Security Alert";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "🚨 [ESP32 ALERT] Motion/Trigger Detected!";
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;

  // Multiple recipients
  message.addRecipient("Admin", RECIPIENT1);
  message.addRecipient("User 1", RECIPIENT2);
  message.addRecipient("User 2", RECIPIENT3);

  // Email body (plain text, formatted)
  time_t now = time(nullptr);
  String timeNow = String(ctime(&now));
  String bodyText =
    "📢 ESP32 Alert Notification\n"
    "-----------------------------\n"
    "⚠️  A trigger was detected on your ESP32.\n"
    "📍 Location: Main Lab Entrance\n"
    "📅 Date/Time: " + timeNow +
    "\n\nPlease investigate the cause immediately.\n\n"
    "Regards,\n"
    "ESP32 Monitoring System";

  message.text.content = bodyText;
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  // SMTP session setup
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  smtp.debug(1);

  // Connect to SMTP and send email
  if (!smtp.connect(&session)) {
    Serial.println("❌ SMTP connection failed.");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message, true)) {
    Serial.println("❌ Email send failed.");
    Serial.println("Error: " + smtp.errorReason());
  } else {
    Serial.println("✅ Email sent to all recipients successfully!");
  }

  smtp.closeSession();
}

void loop() {
  // Empty loop for now
}

