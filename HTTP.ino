void handleRoot() 
{ //auf die Clientanfrage wird folgende response gesendet:
  //HTTP-Status-Code 200 für OK, den Inhaltstyp als „text/html“ und der eigentliche Code
  String timerAsk = server.arg("Taste");

  if (timerAsk.equals("R")== true) {
    timerAlarmEnable(timer);
  }
  if (timerAsk.equals("P")== true) {
    timerAlarmDisable(timer);
  }

  if (lose) {
    loseInd = "Game Over";
  } else {
    loseInd = "Ingame";
  }
  server.send ( 200, "text/html", HTML_Code(activation_counter, loseInd, highscore) );  //Ausgabe an Webseite
}
