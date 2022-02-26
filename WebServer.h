WiFiServer server(80);
WiFiClient client;

String httpd_get_value(String uri, String key) {
  int index = uri.indexOf("&");
  String dict = "";

  if (index == -1) { dict = uri; }
  else { dict = uri.substring(0, index); }

  if (dict.substring(0, dict.indexOf("=")) == key) { return dict.substring(dict.indexOf("=") + 1); }
  else if (index != -1) { return httpd_get_value(uri.substring(index + 1), key); }
  return "";
}

void httpd_client(WiFiClient client) {
  String buffer = "";
  char c = '\0';
  char cbuffer[64];
  bool isFirst = true;
  String method = "";
  String uri = "";
  
  while (client.connected()) {
    if (client.available()) {
      c = client.read();

      if (c == '\n') {
        if (buffer.length() == 0) {
          if (method != "GET") { break; }
          
          if ((uri != "/") && !uri.startsWith("/?")) {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Connection: close");
            client.println();
            #ifdef DEBUG
              logger("404 Response - URI: " + uri);
            #endif
            break;
          }
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type: text/html");
          client.println("Connection: close");
          client.println();

          if (uri.length() > 2) { uri = uri.substring(2); }
          if (httpd_get_value(uri, "brightness") != "") { MatrixSetBrightness(httpd_get_value(uri, "brightness").toInt()); }
          if (httpd_get_value(uri, "speed") != "") { MatrixSetSpeed(httpd_get_value(uri, "speed").toInt()); }
          if (httpd_get_value(uri, "text") != "") { MatrixSetText(httpd_get_value(uri, "text").toInt()); }
          if (httpd_get_value(uri, "mode") != "") { MatrixSetMode(httpd_get_value(uri, "mode").toInt()); }
          if (httpd_get_value(uri, "ison") != "") { MatrixSetIsOn((bool)httpd_get_value(uri, "ison").toInt()); }

          client.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
          client.println("<html>\n<head>\n<title>Daft Helmet</title>");
          client.println("<style>body { color: red; background-color: black; font-size: 2.5em; } h2 { font-size: 2em; margin: 1em; 0px 1em; 0px; }");
          client.println("div { margin-left: 1em; text-align: center; }");
          client.println("select { font-size: 2em; color: green; background-color: black; }");
          client.println("option { color: red; }");
          client.println("option:checked { display: none; }");
          client.println("input[type=\"radio\"] { display: none; }");
          client.println("input[type=\"radio\"] + span { font-size: 1.3em; display: inline-block; padding: 1em; text-transform: uppercase; border: 2px solid red; border-radius: 3px; color: red; }");
          client.println("input[type=\"radio\"]:checked + span { color: white; background-color: green; border: 2px solid green; }");
          client.println("</style></head>");
          client.println("<body><form method=\"GET\" action=\"/\">");

          /* isOn & Update */
          client.println("<div>");
          client.println("<label><input type=\"radio\" name=\"ison_hdr\" value=\"On\"");
          if (isOn) { client.print(" checked"); };
          client.println(" onClick=\"javascript:window.location='/?ison=1';\"><span>On</span></label>");
          client.println("<label><input type=\"radio\" name=\"ison_hdr\" value=\"Off\"");
          if (!isOn) { client.print(" checked"); };
          client.println(" onClick=\"javascript:window.location='/?ison=0';\"><span>Off</span></label>");
          client.println("<br/><label><input type=\"radio\" name=\"manage_hdr\" value=\"Update\" onClick=\"javascript:document.forms[0].submit();\"><span>Update</span></label>");
          client.println("<label><input type=\"radio\" name=\"manage_hdr\" value=\"Refresh\" onClick=\"javascript:window.location='/';\"><span>Refresh</span></label>");
          client.println("</div>");
                    
          /* Mode */
          client.println("<h2>Mode</h2><div>");
          for (int i = 0; i < ModeCount; i++) {
            sprintf(cbuffer, "<label><input type=\"radio\" name=\"mode\" value=\"%d\"", i); client.print(cbuffer);
            if (my_mode == i) { client.print(" checked"); };
            sprintf(cbuffer, "><span>%s</span></label>", my_modes[i]); client.print(cbuffer);
          }
          client.println("</div>");
          
          /* Text */
          client.println("<h2>Text</h2><div>");
          for (int i = 0; i < TextCount; i++) {
            sprintf(cbuffer, "<label><input type=\"radio\" name=\"text\" value=\"%d\"", i); client.print(cbuffer);
            if (my_text == i) { client.print(" checked"); };
            sprintf(cbuffer, "><span>%s</span></label>", TextName[i]); client.print(cbuffer);
          }
          client.println("</div>");

          /* Brightness */
          client.println("<h2>Brightness</h2><div><select name=\"brightness\">");
          for (int i = 1; i < 16; i++) {
            sprintf(cbuffer, "<option value=\"%d\"", i); client.print(cbuffer);
            if (my_brightness == i) { client.print(" selected"); };
            sprintf(cbuffer, ">%d</option>", i); client.print(cbuffer);
          }
          client.println("</select></div>");

          /* Speed */
          client.println("<h2>Speed</h2><div><select name=\"speed\">");
          sprintf(cbuffer, "<option value=\"%d\"", SpeedSlow); client.print(cbuffer);
          if (my_speed == SpeedSlow) { client.print(" selected"); };
          sprintf(cbuffer, ">%s</option>", "Slow"); client.print(cbuffer);
          sprintf(cbuffer, "<option value=\"%d\"", SpeedMedium); client.print(cbuffer);
          if (my_speed == SpeedMedium) { client.print(" selected"); };
          sprintf(cbuffer, ">%s</option>", "Medium"); client.print(cbuffer);
          sprintf(cbuffer, "<option value=\"%d\"", SpeedFast); client.print(cbuffer);
          if (my_speed == SpeedFast) { client.print(" selected"); };
          sprintf(cbuffer, ">%s</option>", "Fast"); client.print(cbuffer);
          client.println("</select></div>");

          /* Update & isOn */
          client.println("<div><br/>");
          client.println("<label><input type=\"radio\" name=\"manage_ftr\" value=\"Update\" onClick=\"javascript:document.forms[0].submit();\"><span>Update</span></label>");
          client.println("<label><input type=\"radio\" name=\"manage_ftr\" value=\"Refresh\" onClick=\"javascript:window.location='/';\"><span>Refresh</span></label>");
          client.println("<br/><label><input type=\"radio\" name=\"ison_ftr\" value=\"On\"");
          if (isOn) { client.print(" checked"); };
          client.println(" onClick=\"javascript:window.location='/?ison=1';\"><span>On</span></label>");
          client.println("<label><input type=\"radio\" name=\"ison_ftr\" value=\"Off\"");
          if (!isOn) { client.print(" checked"); };
          client.println(" onClick=\"javascript:window.location='/?ison=0';\"><span>Off</span></label>");
          client.println("</div>");
          
          client.println("</form>\n</body>\n</html>");
          break;
        }
        else {
          if (isFirst) {
            isFirst = false;
            method = buffer.substring(0, buffer.indexOf(" "));
            uri = buffer.substring(buffer.indexOf(" ") + 1, buffer.lastIndexOf(" "));
          }
          #ifdef DEBUG
            logger(buffer);
          #endif
          buffer = "";
        }
      }
      else if (c != '\r') { buffer += c; }
      else { }
    }
  }
  client.stop();
}
