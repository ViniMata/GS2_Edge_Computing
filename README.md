# **Monitor de Consumo Energético com ESP32**

## **Descrição do Projeto**
Este projeto é um sistema de monitoramento de consumo energético utilizando o microcontrolador ESP32, protocolo MQTT, e um display LCD I2C. Ele mede o consumo elétrico em watts, exibe os dados em um display LCD e os publica em tempo real em um broker MQTT. Além disso, o sistema utiliza um cliente NTP para exibir o horário atualizado no display.

---

## **Funcionalidades**
- Conexão à rede Wi-Fi para comunicação em tempo real.
- Monitoramento contínuo do consumo energético em watts.
- Publicação dos dados de consumo no broker MQTT.
- Exibição das informações no display LCD:
  - Consumo em tempo real.
  - Hora atual sincronizada via NTP.
- Reconexão automática ao MQTT e à rede Wi-Fi, caso a conexão seja perdida.

---

## **Componentes Necessários**
### **Hardware**
- ESP32
- Display LCD I2C (20x4)
- Fonte de energia para o ESP32
- Resistores e sensores analógicos (para simulação ou leitura real de consumo)
  
### **Software**
- Arduino IDE ou VS Code com PlatformIO
- Broker MQTT (por exemplo, test.mosquitto.org)

---

## **Configuração do Ambiente**
1. **Instale o Arduino IDE**: [Download](https://www.arduino.cc/en/software).
2. **Adicione a placa ESP32** ao Arduino IDE:
   - Vá em **Arquivo > Preferências**.
   - Adicione o link para ESP32:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Vá em **Ferramentas > Placas > Gestor de Placas** e instale a biblioteca ESP32.
3. **Instale as bibliotecas necessárias**:
   - Vá em **Ferramentas > Gerenciar Bibliotecas** e procure por:
     - `PubSubClient` (para comunicação MQTT)
     - `NTPClient` (para hora sincronizada)
     - `LiquidCrystal_I2C` (para display LCD)
4. **Configuração do Broker MQTT**:
   - Use um broker público como `test.mosquitto.org` ou configure seu próprio broker local.

---

## **Como Usar**
### **Configuração Inicial**
1. Clone ou baixe o código do projeto.
2. Abra o arquivo no Arduino IDE.
3. Edite as configurações de Wi-Fi:
   ```cpp
   const char* ssid = "SEU_SSID";
   const char* password = "SUA_SENHA";
   ```
4. Se necessário, ajuste o servidor MQTT:
   ```cpp
   const char* mqtt_server = "SEU_BROKER_MQTT";
   const char* mqtt_topic = "sensor/consumo/energetico";
   ```
5. Conecte os componentes ao ESP32 conforme o diagrama.

### **Upload do Código**
1. Selecione a placa correta: **ESP32 Dev Module**.
2. Escolha a porta serial conectada ao ESP32.
3. Compile e faça o upload do código.

---

## **Diagrama de Conexões**
### **ESP32 e Display LCD I2C**
| ESP32       | Display LCD I2C |  
|-------------|-----------------|  
| VIN         | VCC             |  
| GND         | GND             |  
| GPIO 21     | SDA             |  
| GPIO 22     | SCL             |

---

## **Fluxo de Operação**
1. Após ligar o ESP32:
   - O dispositivo se conecta ao Wi-Fi.
   - Conecta-se ao broker MQTT.
   - Sincroniza a hora via NTP.
2. A cada 500ms:
   - Lê os valores do ADC (simulando consumo).
   - Publica o consumo em watts no tópico MQTT configurado.
   - Atualiza os dados no display LCD.
3. A cada 1 segundo:
   - Atualiza o relógio e exibe a hora no LCD.

---

## **Requisitos e Dependências**
### **Hardware**
- Um microcontrolador ESP32.
- Sensor analógico para consumo energético.
- Display LCD I2C compatível.

### **Bibliotecas Necessárias**
- **WiFi**: Conexão Wi-Fi para ESP32.
- **PubSubClient**: Para publicar mensagens no broker MQTT.
- **NTPClient**: Sincronização de horário.
- **LiquidCrystal_I2C**: Controle do display LCD.
  
---

## **Exemplo de Uso**
### **Monitoramento no Serial Monitor**
Ao executar o código, o Serial Monitor exibe:
```plaintext
Se conectando a: Wokwi-GUEST
Conexão Wi-Fi realizada
Endereço IP: 192.168.x.x
Tentando conexão MQTT... clientId-123 conectado
Consumo: 100.5 W
Hora: 12:34:56
```

### **Monitoramento pelo Broker**
Os dados de consumo serão publicados no tópico `sensor/consumo/energetico`. Use um cliente MQTT para visualizar.

---

## **Autores**
- **Vinícius Matareli** - RM 555200  
- **Giovanne Charelli** - RM 556223

--- 

## **Link no Wokwi**
<a>https://wokwi.com/projects/415103566822192129</a>
