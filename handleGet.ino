void handleGet(WiFiClient cli, String reqHeader) {
  //Serial.println("GET request detected");
  if (reqHeader.indexOf("favicon") > -1) {
    cli.print("HTTP/1.1 204 No Content\r\n\r\n");
    return;
  }
  
  String fullBody = prePage();
  fullBody += "<p>Valve is turned : <strong id='espValve'>" + String(valveOn ? "On" : "Off") + "</strong></p>";
  fullBody += "<p>Running on <strong id='espMode'>"+ String(autoMode ? "Auto" : "Manual") +"</strong></p>";
  if(autoMode) fullBody += "<p>Next spray in <strong id='espTimeRemain'>" +String(abs((intervalSeconds+periodSeconds)*1000 - millis() + lastBurstBegin))+ " milliseconds</strong></p>";
  fullBody += "<p>Accepting charge from solar panel : <strong id='espCharging'>"+ String(charging ? "Yes" : "No") + "</strong></p>";
  fullBody += "<p id='espVoltage'>Battery Voltage: " + String(batteryVoltage) + " ("+String(readV)+"). Shutdown will occur below " + String(minBatVoltage) + " Volts</p>";
  fullBody += postPage();
  writeHeaders(cli, fullBody.length());
  cli.print(fullBody);
}
