#include<ESP8266WiFi.h> //Biblioteca para funcionamento do WiFi do ESP
#include<ESP8266WebServer.h> //Biblioteca para o ESP funcionar como servidor


const char* ssid = "--Nome da rede WiFi--";  // Rede WiFi
const char* password = "--Senha do WiFi--";  //Senha da Rede WiFi

ESP8266WebServer server(80); //server na porta 80


const int pinoLed = D5; //PINO DIGITAL UTILIZADO PELO LED
const int pinoSensor = D2; //PINO DIGITAL UTILIZADO PELO SENSOR
const int analogSensor = A0;
String condicao;


void setup(){
  
  pinMode(pinoSensor, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed, LOW); //LED INICIA DESLIGADO
  Serial.begin(9600);

   WiFi.begin(ssid, password); //Inicialização da comunicação Wi-Fi

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  server.begin(); //Inicializa o servidor
  Serial.println("Servidor HTTP inicializado");
  delay(3000);
}

void loop() {

  server.handleClient(); //Chama o método handleClient()
}

void handle_OnConnect() {
  
  int valor = analogRead(analogSensor);  //Realiza a leitura da temperatura
  
  if(valor >= 500){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      digitalWrite(pinoLed, HIGH); //ACENDE O LED
      condicao = "Risco detectado!";
      Serial.println("Risco detectado");
  }else{ //SENÃO, FAZ
    digitalWrite(pinoLed, LOW); //APAGA O LED
      condicao = "Ambiente estável";
      Serial.println("Ambiente estável");
  }

  Serial.println(valor);
  delay(2000);
  
  Serial.print("Concentração de gás: ");
  Serial.print(valor); //Imprime no monitor serial o valor da temperatura lida
  Serial.println(" ppm");
  server.send(200, "text/html", EnvioHTML(valor)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML
}

void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"
}

String EnvioHTML(int valor) { //Exibindo a página da web em HTML
String ptr = "<!DOCTYPE html> <html>\n"; //Indica o envio do código HTML
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; //Torna a página da Web responsiva em qualquer navegador Web
  ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 2 segundos
  ptr +=  "<meta charset='UTF-8'>";
  ptr +=  "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
  ptr +=  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Detector de vazamento de gases inflamáveis</title>\n"; //Define o título da página

  //Configurações de fonte do título e do corpo do texto da página web
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
  ptr += ".cor {background-color:red;}\n";
  ptr += ".cordois {background-color:green;}\n";
  ptr += "<style>body {font-family: 'Open Sans', sans-serif; background-color: #f2f2f2;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Detector de vazamento de gases inflamáveis</h1>\n";
  ptr += "<h2>NODEMCU ESP8266 Web Server</h2>\n";

  //Exibe as informações de temperatura e umidade na página web
  ptr += "<p><b>Concentração de gás: </b>";
  ptr += (int)valor;
  ptr += " ppm(partes por milhão)</p>";
  
  ptr += "<p><b>Status: </b>";
  ptr += (String)condicao;
  ptr += "</p>\n";
  

  if(valor <= 500){
     ptr += "<div class =\"cordois\">\n";
     ptr += "<h1> STATUS </h1>\n";
     ptr+= "</div>\n";
  }else{
    ptr += "<div class =\"cor\">\n";
    ptr += "<h1> STATUS </h1>\n";
    ptr+= "</div>\n";
  }
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
