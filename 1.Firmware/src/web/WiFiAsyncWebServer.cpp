#include "WiFiAsyncWebServer.h"


extern lv_obj_t *img1;
const char *host = "dial";
const int default_webserverporthttp = 80;

AsyncWebServer *server;  // initialise webserver
bool close_server_flag = false;
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<head>
    <meta charset="utf-8">
    <title>wifi config</title>
</head>
<body>
  <div>
    <div>
      WIFI Config
    </div>
    <div>
      <span>WIFI Name:</span> <input type="text" id="wifi" value="%WIFI%">
    </div>
    <div>
      <span>WIFI Password:</span> <input type="password" id="code" value="%CODE%">
    </div>
    <input type="submit" class="btn" value="Save WIFI" onclick="send_wifi()">
    <div>
        MQTT Config
    </div>
    <div>
        <span>MQTT Host:</span> <input type="text" id="host" value="%HOST%">
    </div>
    <div>
        <span>MQTT port:</span> <input type="text" id="port" value="%PORT%">
    </div>
    <div>
        <span>MQTT Username:</span> <input type="text" id="username" value="%USERNAME%">
    </div>
    <div>
        <span>MQTT Password:</span> <input type="password" id="password" value="%PASSWORD%">
    </div>
    <div>
        <span>MQTT Topic:</span> <input type="text" id="topic" value="%TOPIC%">
    </div>
    <input type="submit" class="btn" value="Save Mqtt" onclick="send_mqtt()">

    <p>Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>
    <p>%FILELIST%</p>
    <form id="form1" enctype="multipart/form-data" method="post" action="/update">
        <input type="file" name="update" id="update" />
    <input type="button" onclick="uploadbin()" value="上傳bin文件"/>
    </form>
      <div class="percentage">upload progress 0</div>
  </div>
</body>
<script>
  function send_wifi() {
    var input_ssid = document.getElementById("wifi").value;
    var input_code = document.getElementById("code").value;
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "/wifi_data", true);
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4) {
        if (xhttp.status == 200) {
          alert("The WIFI is set successfully, WIFI will reconnect");
          console.log(xhttp.responseText);
        } else if (xhttp.status == 0) {
          alert("Server closed the connection abruptly!");
          location.reload()
        } else {
          alert(xhttp.status + " Error!\n" + xhttp.responseText);
          location.reload()
        }
      }
    };
    var data = {
      "wifi_name": input_ssid,
      "wifi_code": input_code
    }
    xhttp.send("wifi_name="+input_ssid+"&wifi_code="+input_code);
  }
  
  function send_mqtt() {
    var host = document.getElementById("host").value;
    var port = document.getElementById("port").value;
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;
    var topic = document.getElementById("topic").value;
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "/mqtt_data", true);
    xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4) {
        if (xhttp.status == 200) {
          alert("The MQTT set successfully");
          console.log(xhttp.responseText);
        } else if (xhttp.status == 0) {
          alert("Server closed the connection abruptly!");
          location.reload()
        } else {
          alert(xhttp.status + " Error!\n" + xhttp.responseText);
          location.reload()
        }
      }
    };
    var data = {
      "host": host,
      "port": port,
      "username": username,
      "password": password,
      "topic": topic
    }
    xhttp.send("host="+host+"&port="+port+"&username="+username+"&password="+password+"&topic="+topic);
  }
  
  function uploadbin() {
    var fd = new FormData();
    fd.append("update", document.getElementById("update").files[0]);
    var xhr = new XMLHttpRequest();
    xhr.upload.addEventListener("progress", function (e) {
        let percentage = e.loaded / e.total * 100
        document.querySelector(".percentage").innerHTML = "upload progress"+`${percentage.toFixed(2)}`
    })
    xhr.open("POST", "/update");
    xhr.send(fd);
    }
</script>
)rawliteral";

//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

bool exists(String path) {
  bool yes = false;
  File file = FILESYSTEM.open(path, "r");
  if (!file.isDirectory()) {
    yes = true;
  }
  file.close();
  return yes;
}

String listFiles(bool ishtml) {
  // server.sendHeader("Connection", "close");
  String path = "/";
  Serial.println("handleFileList: " + path);


  File root = FILESYSTEM.open(path);
  path = String();

  String output = "";
  if (root.isDirectory()) {
    File file = root.openNextFile();
    while (file) {
      if (output != "") {
        output += "<br/>";
      }
      output += "{\"type\":\"";
      output += (file.isDirectory()) ? "dir" : "file";
      output += "\",\"name\":\"";
      output += String(file.path()).substring(1);
      output += "\"}";
      file = root.openNextFile();
    }
  }
  return output;
}

String processor(const String &var) {
  if (var == "FILELIST") {
    return listFiles(true);
  }
  if (var == "FREESPIFFS") {
    return formatBytes((FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes()));
  }
  if (var == "USEDSPIFFS") {
    return formatBytes(FILESYSTEM.usedBytes());
  }
  if (var == "TOTALSPIFFS") {
    return formatBytes(FILESYSTEM.totalBytes());
  }
  String ssid,password,host,username,topic;
  uint16_t port;
  get_wifi_config(ssid,password);
  if (var == "WIFI") {
    return ssid;
  }
  if (var == "CODE") {
    return password;
  }
  get_mqtt_config(host,port,username,password,topic);
  if (var == "HOST") {
    return host;
  }
  if (var == "PORT") {
    return String(port);
  }
  if (var == "USERNAME") {
    return username;
  }
  if (var == "PASSWORD") {
    return password;
  }
  if (var == "TOPIC") {
    return topic;
  }
  return String();
}


void handleWiFiConfig(AsyncWebServerRequest *request) {
#if 0  //調試用
  Serial.println("post wifi config");
  Serial.println(request->args());
  Serial.println(request->version());
  Serial.println(request->method());
  Serial.println(request->url());
  Serial.println(request->host());
  Serial.println(request->contentType());
  Serial.println(request->contentLength());
  Serial.println(request->multipart());

  int headers = request->headers();
  int i;
  for(i=0;i<headers;i++){
    AsyncWebHeader* h = request->getHeader(i);
    Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    // 下面行功能同上面兩行
    //Serial.printf("HEADER[%s]: %s\n", request->headerName(i).c_str(), request->header(i).c_str()); 
  }
  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isFile()){ //p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
    } else if(p->isPost()){
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    } else { //GET
      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
  }
#endif
  //  Serial.println(request->argName(0));     // get request argument name by number
  if (request->hasArg("wifi_name")) {
    const String wifi_name = request->arg("wifi_name");
    const String wifi_pass = request->arg("wifi_code");
    Serial.println(wifi_name);
    Serial.println(wifi_pass);
    set_wifi_config(wifi_name,wifi_pass);
    if (wifi_name.length() > 0) {
      ESP.restart();
    }
    request->send_P(200, "text/html", "seccess");
  }
}


void handleMqttConfig(AsyncWebServerRequest *request) {
  if (request->hasArg("host")) {
    const String host = request->arg("host");
    const String port = request->arg("port");
    const String username = request->arg("username");
    const String password = request->arg("password");
    const String topic = request->arg("topic");
    Serial.println(host);
    Serial.println(port);
    set_mqtt_config(host,port.toInt(),username,password,topic);
    request->send_P(200, "text/html", "seccess");
  }
}

// handles updata   用於ota
void handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);
  if (!index) {  //開始上傳
    logmessage = "Upload Start: " + String(filename);
    if (!Update.begin()) {  //start with max available size
      Update.printError(Serial);
    }
    Serial.println(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }
  if (final) {  //bin文件上傳完成  重啟esp32
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    if (Update.end(true)) {  //true to set the size to the current progress
      Serial.println(logmessage);
    } else {
      Update.printError(Serial);
    }
    Serial.printf("Rebooting...\n");
    request->send_P(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }
}
void configureWebServer() {
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + +" " + request->url();
    Serial.println(logmessage);
    request->send_P(200, "text/html", index_html, processor);
  });
  // run handleUpdate function when bin file is uploaded
  server->on(
    "/update", HTTP_POST, [](AsyncWebServerRequest *request) {
      request->send(200);
    },
    handleUpdate
  );
  server->on("/wifi_data", HTTP_POST, handleWiFiConfig);
  server->on("/mqtt_data", HTTP_POST, handleMqttConfig);
}


void ffat_init() {
  //第一次燒錄程序 初始化FFat
  if (get_init_ffat())  // 獲取當前命名空間中的鍵名為"ffat"的值   如果沒有該元素則返回默認值0
  {
    if (!FILESYSTEM.begin(FORMAT_FILESYSTEM_IF_FAILED, "/LFS")) {
      Serial.println("FFat Mount Failed");
      return;
    }
    Serial.printf("FFAT啟動\n");
    Serial.printf("Total space: %10u\n", FILESYSTEM.totalBytes());
    Serial.printf("Free space: %10u\n", FILESYSTEM.freeBytes());
    File root = FILESYSTEM.open("/");
    File file = root.openNextFile();
    while (file) {
      String fileName = file.name();
      size_t fileSize = file.size();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
      file = root.openNextFile();
    }
    file.close();
    Serial.printf("\n");
  } else {
    FILESYSTEM.format();
    Serial.printf("ffat初始化成功\n");
    set_init_ffat(1);  // 將數據保存到當前命名空間的"count"鍵中
    Serial.printf("重啟ESP32\n");
    ESP.restart();
  }
}


void clear_wifi_name()
{
  set_wifi_config("","");
}

void wifi_server_begin(void *parameter) {
  // put your setup code here, to run once:
  close_server_flag = false;
  char wifi_mode;
  wifi_mode = 0;  //沒有狀態
  TaskParams *params = (TaskParams *)parameter;
  lv_obj_t* ui_wifi_label = params->ui_wifi_label;

  //第一次燒錄程序 初始化FFat
  String ssid,password;
  get_wifi_config(ssid,password);
  if (ssid.length() > 1) 
  {
    const char *wifi_name = ssid.c_str();  //去掉const會顯示編譯錯誤
    const char *wifi_pass = password.c_str();  //去掉const會顯示編譯錯誤
  
    //WIFI INIT
    #if DEBUG_PRINT
      printf("already set wifi\n");
      printf("%s\n", wifi_name);
      printf("%s\n", wifi_pass);
      printf("Connecting to %s\n", wifi_name);
    #endif
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_name, wifi_pass);
    wifi_mode = 1;  //  STA模式
    int wifi_max_count = 0;

    String wifi_count = "";
    while (WiFi.status() != WL_CONNECTED) {
      wifi_max_count++;
      vTaskDelay(1000);
      #if DEBUG_PRINT
        printf(".");
      #endif
      wifi_count += ".";
      lv_label_set_text(ui_wifi_label, wifi_count.c_str());
      if (wifi_max_count == MAX_STA_COUNT) {
        wifi_mode = 2;
        set_wifi_config("","");
        #if DEBUG_PRINT
          printf("connect fail");
        #endif
        break;
      }
    }
  } else {
    #if DEBUG_PRINT
      printf("wifi not set\n");
    #endif
    wifi_mode = 2;
  }
  if (wifi_mode == 1) {
    #if DEBUG_PRINT
      printf("WIFI-STA連接成功\n");
      printf("\n\nNetwork Configuration:\n");
      printf("----------------------");
      printf("         SSID: ");
      printf("%s\n", WiFi.SSID().c_str() );
      printf("  WiFi Status: ");
      printf("%d\n", WiFi.status() );
      printf("WiFi Strength: ");
      printf("%d", WiFi.RSSI());
      printf(" dBm\n");
      printf("          MAC: ");
      printf("%s\n",WiFi.macAddress().c_str() );
      printf("           IP: ");
      printf("%s\n",WiFi.localIP().toString().c_str());
    #endif
    lv_label_set_text(ui_wifi_label, ("STA:" + WiFi.localIP().toString()).c_str());
  }
  if (wifi_mode == 2) {
    char mac_tmp[6];
    const char *ssid = mac_tmp;
    sprintf(mac_tmp, "%02X\r\n", (uint32_t)(ESP.getEfuseMac() >> (24)));
    sprintf(mac_tmp, "ESP32-%c%c%c%c%c%c", mac_tmp[4], mac_tmp[5], mac_tmp[2], mac_tmp[3], mac_tmp[0], mac_tmp[1]);
    WiFi.mode(WIFI_AP);
    while (!WiFi.softAP(ssid, "")) {};  //啟動AP
    #if DEBUG_PRINT
      printf("AP start success\n");
      printf("ip:");
      printf("%s\n",WiFi.softAPIP().toString().c_str() );
    #endif
    lv_label_set_text(ui_wifi_label, ("AP:" + WiFi.softAPIP().toString()).c_str());
  }
  MDNS.begin(host);
  Serial.print("Open http://");
  Serial.print(host);
  Serial.println(".local to see the file browser");


  // configure web server
  #if DEBUG_PRINT
    printf("\nConfiguring Webserver ...\n");
  #endif
  server = new AsyncWebServer(default_webserverporthttp);
  configureWebServer();

  // startup web server
  #if DEBUG_PRINT
    printf("Starting Webserver ...\n");
  #endif
  server->begin();
  while (1) {
    vTaskDelay(1);
    if(close_server_flag){
      server->end();
      delete server;
      break;
    }
  }
  #if DEBUG_PRINT
    printf("Stop Webserver ...\n");
  #endif
  //WiFi.disconnect();
  //WiFi.mode(WIFI_OFF);
  while (1) {
    vTaskDelay(10);
  }
}

void close_server() {
  close_server_flag = true;
}
  

