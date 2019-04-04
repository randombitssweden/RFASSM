String myFingerprint() {
  String p_data; //= DSID;
  p_data += timeClient.getEpochTime();
  p_data += "00";
  p_data += millis();
  //uint8_t data[p_data.length()] = ;
  //strcpy(data, p_data);
  //data.toCharArray(p_data, p_data.length());
  //  p_data.toCharArray(data, p_data.length());
  // aes256_encrypt_ecb(&ctxt, p_data);
  return p_data;
}

void handleRoot() {
  String htmlReply = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>RFASSM:";
  htmlReply +=DSID;
  htmlReply +="</title>\n<meta charset=\"UTF-8\">\n<meta name=\"keywords\" content=\"HTML,JavaScript\">\n<meta name=\"author\" content=\"(C) 2018 Random Bits AB\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n</head>\n<body>RFASSM ID: ";
  htmlReply += DSID;
  htmlReply += "</br>\nVERSION:";
  htmlReply +=DVERSION;
  htmlReply +="</br>\nSTATUS:";
  htmlReply +=myStatus;
  htmlReply +=myFingerprint();
  htmlReply +="</br>\nGMT TIME:";
  htmlReply +=timeClient.getFormattedTime();
  htmlReply +="\n</body></html>";
  server.send(200, "text/html", htmlReply);
}

void handleAlarm() {
  String htmlReply = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>RFASSM:";
  htmlReply +=DSID;
  htmlReply +="</title>\n<meta charset=\"UTF-8\">\n<meta name=\"keywords\" content=\"HTML,JavaScript\">\n<meta name=\"author\" content=\"(C) 2018 Random Bits AB\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n</head>\n<body>ALARM:";
  if(myStatus == 200) {
    htmlReply += "OK";
  } else if (myStatus == 423) {
    htmlReply += "LOCKDOWN";
  } else if (myStatus == 409) {
    htmlReply += "CONFLICT";
  } else if (myStatus == 104) {
    htmlReply += "TRIGGERED";
  } else {
    htmlReply += "PROCESSING";
  }
  htmlReply +="<hr>";
  htmlReply +="</br>\n(c) 2018 Random Bits AB - <a href=\"";
  htmlReply +=DURL;
  htmlReply +="\">";
  htmlReply +=DURL_ALT;
  htmlReply +="\n</body></html>";
  server.send(200, "text/html", htmlReply);
}
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void handleStatus(){
    String htmlReply = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>RFASSM:";
    htmlReply +=DSID;
    htmlReply +="</title>\n<meta charset=\"UTF-8\">\n<meta name=\"keywords\" content=\"HTML,JavaScript\">\n<meta name=\"author\" content=\"(C) 2018 Random Bits AB\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n</head>\n<body>RFASSM ID: ";
    htmlReply += DSID;
    htmlReply += "</br>\nVERSION:";
    htmlReply +=DVERSION;
    htmlReply +="</br>\nTEMP:";
    if (useDHT == 1) {
      // DO DHT STUFF - FIXME
    }
    htmlReply +="</br>\nSTATUS:";
    htmlReply +=myStatus;
    htmlReply +=myFingerprint();
    htmlReply +="</br>\nGMT TIME:";
    htmlReply +=timeClient.getFormattedTime();
    htmlReply +="\n</body></html>";
    server.send(200, "text/html", htmlReply);
}
