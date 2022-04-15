#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");

const char* ssid = "ASUS";
const char* password = "9974702102";

uint8_t echo = 5;
uint8_t trig = 4;
int dist = 100;
String formattedTime = "";
String logs[500];
int log_pt = 0;
bool isLog = false;
int clients = 0;
String currentDate ="";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  #logs {
    font-size: 1.8rem;
    color: #000;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 100%;
    margin: 0 auto;
  }
  .card {
    width: max-content;
    display: none;
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding:20px;
    margin: 10px auto;
    text-align: center;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    margin: 10px;
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
  .logs {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .logs:hover {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }

    .download {
      padding: 10px;
      font-size: 16px;
      text-align: center;
      outline: none;
      color: #fff;
      background-color: #0f8b8d;
      border: none;
      border-radius: 5px;
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      -khtml-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
      -webkit-tap-highlight-color: rgba(0,0,0,0);
    }
   
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
           
   ol {
     list-style: none;
     counter-reset: item;
   }
   li {
     counter-increment: item;
     margin-bottom: 5px;
     text-align: left;
   }
   li:before {
     margin-right: 5px;
     content: counter(item);
     background: lightblue;
     border-radius: 100%;
     color: white;
     width: 1.2em;
     text-align: center;
     display: inline-block;
     padding: 2px;
   }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card" id="card1">
      <h2>Object Detected</h2>
      <p class="state">Last Detected distance: <span id="distance"></span> cm</p>
      <p class="state">Last Detection at: <span id="time"></span></p>
    </div>
    <div class="card" id="card2">
      <h2>No Object Detected</h2>
    </div>
    <div class="buttons">
      <button id="connect" class="button">Connect</button>
      
      <button id="check" class="button">Check</button>
      
      <button id="disconnect" class="button">Disconnect</button>
      
      <button id="log" class="logs">Logs</button>
     </div>
      <div class="card" id="loglist">
        <h1 id="logs">Logs</h1>
        <button class="download" onclick="download()">Download Logs</button>
      </div>
      <div class="card" id="nologs">No Logs Available</div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var logtxt = [];
  window.addEventListener('load', onLoad);
  let interval = setInterval(function() {
    document.getElementById("check").click();
    }, 2000);

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    str = event.data;
    arr = str.split(".");

    arrn = arr[0].split(" ");
    arrt = arrn[1].split("/");

    distance = parseInt(arr[0]);
    if (distance < 80){
      document.getElementById('distance').innerHTML = distance;
      document.getElementById('time').innerHTML = arrt[0]+ ", " +arrt[1];
      document.getElementById("card1").style.display = "block";
      document.getElementById("card2").style.display = "none";
      if (navigator.vibrate) {
          window.navigator.vibrate(1000);
      }
    }
    else{
      document.getElementById("card2").style.display = "block";
      document.getElementById("card1").style.display = "none";
      if (navigator.vibrate) {
          window.navigator.vibrate(0);
      }
    }

    logsdata = arr[1];
    logsarr = logsdata.split(",");
    
    console.log("logsarr");
    console.log(logsarr);
    
    var logsarrn = logsarr.filter(function (el) {
      return el != "";
    });
    
    if(logsdata === "" ){
//      document.getElementById("nologs").style.display = "block";
      document.getElementById("loglist").style.display = "none"
    }else{
      document.getElementById("nologs").style.display = "none";
      document.getElementById("loglist").style.display = "block";
      if(document.getElementById('logData')!== null)
      document.getElementById("loglist").removeChild(document.getElementById('logData'));
      document.getElementById("loglist").appendChild(makeOL(logsarrn));
    }
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('check').addEventListener('click', distance);
    document.getElementById('log').addEventListener('click', openLog);
    document.getElementById('disconnect').addEventListener('click', disconn);
    document.getElementById('connect').addEventListener('click', conn);
  }
  function distance(){
    console.log("click");
    websocket.send('distance');
  }
  function openLog(){
    console.log("click");
    websocket.send('distance');
    window.setTimeout(noLogsAlert(), 200);
  }
  function conn(){
    console.log("click");
    interval = setInterval(function() {
    document.getElementById("check").click();
    }, 2000);
  }
  function disconn(){
    console.log("click");
    clearInterval(interval);
  }
  function makeOL(array) {
    logtxt = [];
    var list = document.createElement('ol');
    list.setAttribute("id", "logData");
    for (var i = 0; i < array.length; i++) {
        var item = document.createElement('li');
        itemarr = array[i].split(" ");
        console.log("itemarr");
        console.log(itemarr);
        itemarrt = itemarr[1].split("/");
        console.log("itemarrt");
        console.log(itemarrt);
        item.appendChild(document.createTextNode("Object Detected at " + itemarr[0] +" cm at "+ itemarrt[0] + " on "+itemarrt[1]));
        list.appendChild(item);
        logtxt[i] = "Object Detected at " + itemarr[0] +" cm at "+ itemarrt[0] + " on "+itemarrt[1] + "\n"
    }
    return list;  
  }  
  function noLogsAlert(){
    if(logsdata === ""){
      window.alert("No Logs Available");      
    }
  }
  function download() {
    var a = document.createElement('a');
    a.href = "data:text/html," + logtxt;
    a.target = "_blank";
    a.download = "newfile.txt";
    a.click();
   }
</script>
</body>
</html>
)rawliteral";

void notifyClients(String data) {
  ws.textAll(data);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "distance") == 0) {
      notifyClients(check_reading() + "." + send_logs());
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        clients +=1;
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        clients -=1;
        clear_logs();
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket(){
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup(){
//  initializeUS();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Start server
  server.begin();
  
  
  timeClient.begin();
  timeClient.setTimeOffset(19800);
//  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  
  Serial.println(monthDay);
  Serial.println(currentMonth);
  Serial.println(currentYear);
  
  currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
  
  Serial.println(currentDate);
}

void loop(){
  ws.cleanupClients();
  get_time();
  if (clients == 0) {
    String s = check_reading();
    if(dist < 80){
      generate_log(s);
    }
  }
  Serial.println(WiFi.localIP());
  delay(500);
}

void get_time(){
  timeClient.update();
  formattedTime = timeClient.getFormattedTime()+ "/" + currentDate;
}

void initializeUS(){
  pinMode(echo, OUTPUT); 
  pinMode(trig, INPUT);
  Serial.begin(115200);
}

int readUS(){
  digitalWrite(echo, LOW);
  delayMicroseconds(20);
  digitalWrite(echo, HIGH);
  delayMicroseconds(100);
  digitalWrite(echo, LOW);
  int distance = microsecondsToCentimeters(pulseIn(trig, HIGH));
  Serial.println(distance);
  return distance;
}

long microsecondsToCentimeters(long microseconds){
   return microseconds / 29 / 2;
}

String check_reading(){
//  dist = readUS();
  dist = random(1,200);
  if(dist>1000){
    dist  = 0;
  }
  return String(dist) + String(" ") + String(formattedTime);
}

void generate_log(String data){
  isLog = true;
  if(log_pt != 199){
    logs[log_pt] = data;
    log_pt+=1;
    Serial.println(data);
    Serial.println("log PT");
    Serial.println(log_pt);
  }
}

String send_logs(){
  String logstr = "";
  for(int i =0;i<log_pt;i++){
    logstr += logs[i] + ",";
  }
  return logstr;
}

void clear_logs(){
  if (clients == 0){
    String logs[500] = {};
    log_pt = 0;
  }
}
