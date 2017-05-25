
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "DHT.h"


#define PORTA_MQTT 17093
#define USUARIO_MQTT "cdhwvxkk"
#define SENHA_MQTT "uoHN9SWskIFf"


#define TOPICO_LED_COMANDO "/led/comando/"
#define TOPICO_LED_ESTADO "/led/estado/"


char SERVER_ADDRESS[] = "m13.cloudmqtt.com";
char MENSAGEM_BROKER[100];

// Update these with values suitable for your network.
byte mac[]    = { 0xf8, 0xb1, 0x56, 0xfc, 0x4b, 0xbe };

IPAddress ip(10, 0, 0, 98);
IPAddress server(34,200,51,91);

bool led = false;

// Assinatura do metodo callback
void callback(char* topic, byte* payload, unsigned int length);

// Metodo para manipular mensagens recebidas do broker
void callback(char* topic, byte* payload, unsigned int length) 
{
  int i = 0;
  for(i=0; i<length; i++)
  {
    MENSAGEM_BROKER[i] = payload[i];
  }
  MENSAGEM_BROKER[i] = '\0';
  String topicoMensagem = String(topic);

  
  // Executa acao apropriada ao topico recebido
  if(topicoMensagem == TOPICO_LED_COMANDO){
    
    if(strcmp(MENSAGEM_BROKER,"ligar")==0)
    {
      liga_led();
    }else if(strcmp(MENSAGEM_BROKER,"desligar")==0)
    {
      desliga_led();
    }
  }else if(topicoMensagem == TOPICO_LED_ESTADO){
    if(strcmp(MENSAGEM_BROKER,"ler")==0)
    {
      estado_led();
    }
  }
  
}

EthernetClient ethClient;
PubSubClient client(SERVER_ADDRESS, PORTA_MQTT, callback, ethClient);

// Metodo para conexao com o broker
void reconnect()
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduino_minicurso", USUARIO_MQTT, SENHA_MQTT))
    {
      Serial.println("conectado!");
  
      
      sprintf(MENSAGEM_BROKER, "ola mundo!");
      client.publish("/arduino", MENSAGEM_BROKER);  
    
      client.subscribe(TOPICO_LED_COMANDO);
      client.subscribe(TOPICO_LED_ESTADO);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

void setup()
{
  Serial.begin(9600);
  //definindo que o pino do led é do tipo saída
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  
  Serial.println("Conectando na rede...");  
  if (Ethernet.begin(mac) == 0)
  { 
    Serial.println("Não foi possível receber config de rede via DHCP");
    setup();
  }
  printIPAddress();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}
void desliga_led(){
  Serial.println("\ndesligando led");
  digitalWrite(LED_BUILTIN, LOW);//desliga o led
  led = false;//atualiza a variavel de controle
  estado_led();//envia a informação para o broker
}
void liga_led(){
  Serial.println("\nligando led");
  digitalWrite(LED_BUILTIN, HIGH);//liga o led
  led = true;//atualiza a variavel de controle
  estado_led();//envia a informação para o broker
}
void estado_led(){//envia o estado do led para o broker
  Serial.println("enviando informações sobre o led: ");
  if(led){
    Serial.println("o led está ligado!");
    sprintf(MENSAGEM_BROKER, "ligado");
  }else{
    Serial.println("o led está desligado!");
    sprintf(MENSAGEM_BROKER, "desligado");
  }
  client.publish(TOPICO_LED_ESTADO, MENSAGEM_BROKER);  
}
