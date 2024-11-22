#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

// Configuração Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configuração MQTT
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/consumo/energetico";

#define COLUNAS 20
#define LINHAS 4

char clientId[50];
char consumo_array[10]; // Para armazenar o valor do consumo como string

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long int time_ms;
unsigned int valor_ad;

String atual, anterior = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", -10800, 60000);

LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  lcd.init();
  lcd.backlight();

  Serial.print("Se conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConexão Wi-Fi realizada");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado!");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());

  timeClient.begin();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void mqttReconnect()
{
  while (!client.connected())
  {
    Serial.print("Tentando conexão MQTT... ");
    long r = random(1);
    sprintf(clientId, "clientId-%ld", r);

    if (client.connect(clientId))
    {
      Serial.print(clientId);
      Serial.println(" conectado");
      client.subscribe(mqtt_topic);
    }
    else
    {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Mensagem recebida no tópico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");

  String stMessage;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
}

void loop()
{
  if (!client.connected())
  {
    mqttReconnect();
  }

  client.loop();

  if (millis() - time_ms >= 500)
  {
    time_ms = millis();

    // Lê o valor do ADC
    valor_ad = analogRead(39);

    // Mapeia para consumo em watts (0 a 4095 -> 0 a 3000 watts)
    float consumo_watts = map(valor_ad, 0, 4095, 0, 3000);
    consumo_watts = consumo_watts / 10.0; // Para exibir até uma casa decimal

    // Atualiza o array para MQTT
    dtostrf(consumo_watts, 6, 1, consumo_array);

    // Publica no tópico MQTT
    client.publish(mqtt_topic, consumo_array);

    // Atualiza o LCD
    lcd.setCursor(0, 2);
    lcd.print("Consumo: ");
    lcd.print(consumo_watts);
    lcd.print(" W");

    // Log no Serial Monitor
    Serial.print("Consumo: ");
    Serial.print(consumo_watts);
    Serial.println(" W");
  }

  // Atualiza o relógio
  if (millis() - time_ms >= 1000)
  {
    time_ms = millis();
    timeClient.update();
    atual = timeClient.getFormattedTime();
  }

  if (atual != anterior)
  {
    anterior = atual;
    lcd.setCursor(0, 3);
    lcd.print("Hora: ");
    lcd.print(atual);
  }
}
