String JS_Code(void) 
{
    String js = "<script> var text = 0;\n";

    js += "setInterval(function() { location.reload(); }, 500);";

    js += "function Button_TimerOn()"
          "{"
          "var TastWert = document.getElementById('TasterOn').value;"
          "window.location.search ='Taste' +TastWert;"
          "}"
          "function Button_TimerOff()"
          "{"
          "var TastWert = document.getElementById('TasterOff').value;"
          "window.location.search ='Taste' +TastWert;"
          "}";

    js += "</script>\n";
    return js;
}
