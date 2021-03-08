void handlePost(WiFiClient cli) {
  String fullBody = "";
  Serial.println("POST request");
  String requestData = cli.readStringUntil(';');
  Serial.println("Got this:" + requestData);

  if (requestData == "burst") {
    Serial.println("Burstingggg");
    fullBody += "Turning on for " + String(periodSeconds) + " seconds";
    lastBurstBegin = millis();
    valveOn = true;
    digitalWrite(valvePin, LOW);
  }
  else if (requestData.indexOf("setInterval:") > -1) {
    int delim1 = requestData.indexOf(':');
    int delim2 = requestData.indexOf('/', delim1);
    int delim3 = requestData.indexOf('#', delim2);
    long prelimPeriod = requestData.substring( delim1 + 1, delim2).toInt();
    long prelimInterval = requestData.substring( delim2 + 1, requestData.indexOf('#', delim2)).toInt(); 
    if (requestData.substring(delim3 + 1) != "KFC" || prelimPeriod == 0 || prelimInterval == 0) {
      fullBody += "Bad input!";
    }
    else {
      periodSeconds = prelimPeriod;
      intervalSeconds = prelimInterval;
      fullBody += "Set to spray for " + String(periodSeconds) + " seconds every " + String(intervalSeconds) +" second";
    }
  }
  else if (requestData == "setState:manual") {
    Serial.println("Manuela");
    fullBody += "Setting manual state";
    autoMode = 0;
  }
  else if (requestData == "setState:auto") {
    Serial.println("Manual");
    fullBody += "Setting auto state";
    autoMode = 1;
  }
  else if (requestData == "0" && autoMode == 0) {
    fullBody += "Turning off";
    valveOn = false;
    digitalWrite(valvePin, HIGH);
  }
  /*else if(requestData == "1" && autoMode == 0){
    cli.print("\r\n");
    cli.print("Turning on");
    valveOn = true;
    }*/
  else {
    fullBody += "Valve is currently turned " + String(valveOn) + " (Bad input)";
  }
  writeHeaders(cli, fullBody.length());
  cli.print(fullBody);
}
