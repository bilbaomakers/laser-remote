// WifiManager.h

#ifndef WifiManager_h
#define WifiManager_h

#include <stdlib.h>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WifiManager {
  public:
    WifiManager(const char* ssid, const char* password);

    /// Function to be called on setup
    void setup(std::function<void(int speedX, int speedY)> setSpeed); 

    /// Function to be called in every loop to update motors
    void loop(); 

    void notifyClients();
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    String processor(const String& var);

    String processHtml();
  
  private:
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    const char* _ssid;
    const char* _password;

    std::function<void(int speedX, int speedY)> _setSpeed;    

    bool ledState = 0;
    const int ledPin = 2;
};

const char _index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head><title>Laser Control</title><meta name="viewport" content="width=device-width,initial-scale=1"><script>let StickStatus={xPosition:0,yPosition:0,x:0,y:0,cardinalDirection:"C"};var JoyStick=function(t,i,e){var o=void 0===(i=i||{}).title?"joystick":i.title,n=void 0===i.width?0:i.width,a=void 0===i.height?0:i.height,s=void 0===i.internalFillColor?"#00AA00":i.internalFillColor,r=void 0===i.internalLineWidth?2:i.internalLineWidth,c=void 0===i.internalStrokeColor?"#003300":i.internalStrokeColor,d=void 0===i.externalLineWidth?2:i.externalLineWidth,u=void 0===i.externalStrokeColor?"#008000":i.externalStrokeColor,h=void 0===i.autoReturnToCenter||i.autoReturnToCenter;axis=void 0!==i.axis&&i.axis,e=e||function(t){};var S=document.getElementById(t);S.style.touchAction="none";var l=document.createElement("canvas");l.id=o,0===n&&(n=S.clientWidth),0===a&&(a=S.clientHeight),l.width=n,l.height=a,S.appendChild(l);var f=l.getContext("2d"),x=0,k=2*Math.PI,g=(l.width-(l.width/2+10))/2,v=g+5,P=g+30,y=l.width/2,m=l.height/2,C=l.width/10,p=-1*C,w=l.height/10,L=-1*w,F=y,E=m;function W(){f.beginPath(),f.arc(y,m,P,0,k,!1),f.lineWidth=d,f.strokeStyle=u,f.stroke()}function T(){f.beginPath(),F<g&&(F=v),F+g>l.width&&(F=l.width-v),E<g&&(E=v),E+g>l.height&&(E=l.height-v),f.arc(F,E,g,0,k,!1);var t=f.createRadialGradient(y,m,5,y,m,200);t.addColorStop(0,s),t.addColorStop(1,c),f.fillStyle=t,f.fill(),f.lineWidth=r,f.strokeStyle=c,f.stroke()}function D(){let t="",i=F-y,e=E-m;return e>=L&&e<=w&&(t="C"),e<L&&(t="N"),e>w&&(t="S"),i<p&&("C"===t?t="W":t+="W"),i>C&&("C"===t?t="E":t+="E"),t}"ontouchstart"in document.documentElement?(l.addEventListener("touchstart",(function(t){x=1}),!1),document.addEventListener("touchmove",(function(t){1===x&&t.targetTouches[0].target===l&&(F=t.targetTouches[0].pageX,E=t.targetTouches[0].pageY,"BODY"===l.offsetParent.tagName.toUpperCase()?(F-=l.offsetLeft,E-=l.offsetTop):(F-=l.offsetParent.offsetLeft,E-=l.offsetParent.offsetTop),axis&&("y"===axis?F=y:"x"===axis&&(E=m)),f.clearRect(0,0,l.width,l.height),W(),T(),StickStatus.xPosition=F,StickStatus.yPosition=E,StickStatus.x=((F-y)/v*100).toFixed(),StickStatus.y=((E-m)/v*100*-1).toFixed(),StickStatus.cardinalDirection=D(),e(StickStatus))}),!1),document.addEventListener("touchend",(function(t){x=0,h&&(F=y,E=m);f.clearRect(0,0,l.width,l.height),W(),T(),StickStatus.xPosition=F,StickStatus.yPosition=E,StickStatus.x=((F-y)/v*100).toFixed(),StickStatus.y=((E-m)/v*100*-1).toFixed(),StickStatus.cardinalDirection=D(),e(StickStatus)}),!1)):(l.addEventListener("mousedown",(function(t){x=1}),!1),document.addEventListener("mousemove",(function(t){1===x&&(F=t.pageX,E=t.pageY,"BODY"===l.offsetParent.tagName.toUpperCase()?(F-=l.offsetLeft,E-=l.offsetTop):(F-=l.offsetParent.offsetLeft,E-=l.offsetParent.offsetTop),axis&&("y"===axis?F=y:"x"===axis&&(E=m)),f.clearRect(0,0,l.width,l.height),W(),T(),StickStatus.xPosition=F,StickStatus.yPosition=E,StickStatus.x=((F-y)/v*100).toFixed(),StickStatus.y=((E-m)/v*100*-1).toFixed(),StickStatus.cardinalDirection=D(),e(StickStatus))}),!1),document.addEventListener("mouseup",(function(t){x=0,h&&(F=y,E=m);f.clearRect(0,0,l.width,l.height),W(),T(),StickStatus.xPosition=F,StickStatus.yPosition=E,StickStatus.x=((F-y)/v*100).toFixed(),StickStatus.y=((E-m)/v*100*-1).toFixed(),StickStatus.cardinalDirection=D(),e(StickStatus)}),!1)),W(),T(),this.SetAxis=function(t){console.log("setAxis: "+t),axis=t},this.GetWidth=function(){return l.width},this.GetHeight=function(){return l.height},this.GetPosX=function(){return F},this.GetPosY=function(){return E},this.GetX=function(){return((F-y)/v*100).toFixed()},this.GetY=function(){return((E-m)/v*100*-1).toFixed()},this.GetDir=function(){return D()}};</script><style>body{background-color:#a91c13;color:#fff;margin:0}#radios{position:relative;background-color:#ffffffaa;z-index:5;width:303px}input{display:none}.labels{color:#a91c13}#bckgrnd,.labels{width:100px;height:25px;text-align:center;display:inline-block;padding-top:3px;margin-right:-3px;z-index:2;cursor:pointer;outline:1px solid #fff}@supports (-webkit-touch-callout:none){#radios{width:300px}#bckgrnd,.labels{margin-right:0}}#bckgrnd{background-color:#a91c13;position:absolute;left:0;top:0;z-index:-1}input[type=radio]:checked+label{color:#fff}#rad1:checked~#bckgrnd{transform:translateX(0);transition:transform .5s ease-in-out}#rad2:checked~#bckgrnd{transform:translateX(100px);transition:transform .5s ease-in-out}#rad3:checked~#bckgrnd{transform:translateX(201px);transition:transform .5s ease-in-out}</style></head><body><div style="display:flex;flex-direction:column;align-items:center"><h1>Remote</h1><div id="radios"><input id="rad1" type="radio" name="radioBtn" value="undefined"><label class="labels" for="rad1">Todo</label><input id="rad2" type="radio" name="radioBtn" value="x" checked="checked"><label class="labels" for="rad2">Horizontal</label><input id="rad3" type="radio" name="radioBtn" value="y"><label class="labels" for="rad3">Vertical</label><div id="bckgrnd"></div></div><div id="joyDiv" style="margin:0"></div></div><script type="text/javascript">const width=window.innerWidth,height=window.innerHeight;var size=width<height?width:height;document.getElementById("joyDiv").style.width=size+"px",document.getElementById("joyDiv").style.height=size+"px";const config={title:"Remote",internalFillColor:"#FFF",internalStrokeColor:"#CCC",externalStrokeColor:"#FFF",axis:"x"};var joy=new JoyStick("joyDiv",config,(function(e){console.log("x: ",e.x),1==websocket.readyState&&websocket.send(`x:${e.x}|y:${e.y}`)}));document.querySelectorAll("input[type='radio']").forEach((e=>{e.addEventListener("change",(()=>{console.log("axis: ",e.value),joy.SetAxis(e.value)}))}));var websocket,gateway=`ws://${window.location.hostname}/ws`;function initWebSocket(){console.log("Trying to open a WebSocket connection..."),(websocket=new WebSocket(gateway)).onopen=onOpen,websocket.onclose=onClose,websocket.onmessage=onMessage}function onOpen(e){console.log("Connection opened")}function onClose(e){console.log("Connection closed"),setTimeout(initWebSocket,2e3)}function onMessage(e){var o;o="1"==e.data?"ON":"OFF",document.getElementById("state").innerHTML=o}function onLoad(e){initWebSocket()}window.addEventListener("load",onLoad);</script></body></html>
)rawliteral";

#endif 