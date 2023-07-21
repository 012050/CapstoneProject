#include <WiFi.h>

const char* ssid = "DESKTOP-N75BSJP 8172";
const char* password = "2@6A6o73";

WiFiServer server(80);

String header;
String currentLine;

void setup()
{
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    while (client.connected() )
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n')
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"icon\" href=\"data:,\"></head>");

          client.println("<body><h1>ESP32 Web Server</h1>");
          client.println("<img src=\"https://www.google.com/images/branding/googlelogo/1x/googlelogo_color_272x92dp.png\">");
          client.println("</body></html>");

          client.println();
          client.stop();
          Serial.println("Client disconnected.");
          Serial.println("");
        }
      }
    }
  }
}
