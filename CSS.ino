
String CSS_Code(void) 
{
  String css = "<style>\n";   
  css += " body {"
         "background-color: #cccccc;"
         "font-family: Arial, Helvetica, Sans-Serif;"
         "Color: #000088;"
         "}\n";
  css +=  "@media only screen and ( min-width: 950px )"
          "{    /* breites Browserfenster */"
          "header, nav, section, footer  {"
          "display: block;"
          "}"
          "header  {"
          "background-color: blue;}"
          "nav {"
          "background-color: orange;}"
          "section {"
          "background-color: white;"
          ".bild {    width: 100%;     height: auto; }}"
          "footer  {"
          "background-color: deepskyblue;}"
          "}\n";

  css +=  "@media only screen and ( max-width: 440px )"
          "{        /* mobile Geräte */"
          "header, nav, section, footer" 
          "{"
          "display: block;"
          "}"
          "header  {"
          "background-color: green;}"
          "nav {"
          "background-color: orange;}"
          "section {"
          "background-color: white;"
          ".bild {    width: 100%;     height: auto; }}"
          "footer  {"
          "background-color: deepskyblue;}}\n";

    css +="@media only screen and ( min-width: 440px ) and ( max-width: 950px )"
          "{    /* Tablets und mittlere Bildschirme */"
          "header, nav, section, footer"
          "{    display: block;}"
          "header  {"
          "background-color: yellow;}"
          "nav {"
          "background-color: orange;}"
          "section {"
          "background-color: white;"
          ".bild {    width: 100%;     height: auto; }}"
          "footer  {"
          "background-color: deepskyblue;}}\n";

    css +="</style>\n";
    return css;
}
   
