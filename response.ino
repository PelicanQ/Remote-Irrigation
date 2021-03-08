void responseHandler(WiFiClient cli){
    Serial.println("Found, waiting for newline");
    
    cli.setTimeout(400);
    const int timeoutAfter = 1000;
    unsigned long timeBegin = millis();
    String reqHeader = "";
    /*if(millis() - timeBegin > timeoutAfter){
        Serial.print("Stopping");
        String msg = "Didn't recieve a newline in time";
        writeHeaders(cli, msg.length());
        cli.print(msg);
        cli.stop();
        return;
    }*/
    while (cli.connected() && millis() - timeBegin <= timeoutAfter) {
      Serial.println("<<<watiing");
      String line = cli.readStringUntil('\n');
      if(line.length() == 0){
          Serial.print("line = 0len");
        break;
      }
      Serial.print(" " + line);
      
      reqHeader += line;
      if (line.length() != 1) {
       // Serial.println("Skipping line");
        continue;
      }
      Serial.println("--Whole request header read--");
      //When client has sent whole request header
      
      if(reqHeader.indexOf("GET") > -1){
        handleGet(cli, reqHeader);
      }
      else if(reqHeader.indexOf("POST") > -1){
        handlePost(cli);
      }
      else {
        String msg = "not post nor get";
        writeHeaders(cli, msg.length());
        cli.print(msg);
      }
      break;
    }
    
    //Now we have sent response
    cli.stop();
    Serial.println("end o func");
}
