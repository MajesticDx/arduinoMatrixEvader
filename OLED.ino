void OLED_LCD (String StringOledText)
{
  OLED.clear();
  OLED.setCursor(0,0);
  OLED.setFontSize(FONT_SIZE_SMALL);

  OLED.println(ssid);
  OLED.println(WiFi.macAddress());
  OLED.println("IP-Webside:");
  OLED.println(WiFi.localIP());
  OLED.println("Status:");
  OLED.println(StringOledText);
  }
