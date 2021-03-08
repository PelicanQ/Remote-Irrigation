String prePage(){
  return 
  "<!DOCTYPE html>\\
  <html>\\
  <head>\\
    <title>ESP</title>\\
    <script>\\
    function send(e){var n=new XMLHttpRequest();n.open('POST','');n.setRequestHeader('Cache-Control','no-store');n.onload=()=>{alert(n.response);n.abort()};n.send(e)}\\
    </script>\\
  </head>\\
  <body>";
}
String postPage(){
  return 
  "<button onclick='send(\"burst;\")'>Burst</button>\\
  <button onclick='send(\"setState:auto;\")'>Auto mode</button>\\
  <button onclick='send(\"setState:manual;\")'>Manual mode</button>\\
  <button onclick='send(\"0;\")'>Off (only in manual)</button>\\
</body>\\
</html>";
}
