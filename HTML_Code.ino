String HTML_Code(int activation_counter, String loseInd, int highscore) 
 {
  String page = "<html>\n";
  page += "<!DOCTYPE HTML><html><head>"
          "<html><head>"
          "<title>DAn ESP32</title>"
          "<meta charset='utf-8'>"
          "<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'></meta>\n";
  page += CSS_Code();
  page += "</head><body id='inputoutput'>"          
          " <h1>Matrix Evade Game</h1>"
          "<section>"
          "<br>"
          "<p>   Score: " + String(activation_counter) + "</p>"
          "<br>"
          "<p>   Status: " + loseInd + "</p>"
          "<br>"
          "<p>   HighScore: " + highscore + "</p>"
          "<br>"
          "<button id='TasterOn' onclick='Button_TimerOn()' value='=R'> Resume</button>"
          "<button id='TasterOff' onclick='Button_TimerOff()' value='=P'> Pause</button>"
          "</section>";

          //https://stackoverflow.com/questions/55940670/how-to-get-slider-range-value-and-store-it-in-a-variable-within-javascript fix damit bitte
   page += JS_Code();       
   page +="</body></html>\n";
          
  return page;
 }
