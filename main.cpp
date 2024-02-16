#include <Arduino.h>
#include <Notecard.h>

#define usbSerial Serial
#define productUID ""
Notecard notecard;

int total;

void setup()
{
  delay(2500);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13,LOW);

  usbSerial.begin(115200);
  notecard.begin();
  notecard.setDebugOutputStream(usbSerial);

  //Set Notecard Settings
  J *req = notecard.newRequest("hub.set");
  JAddStringToObject(req, "product", productUID);
  JAddStringToObject(req, "mode", "continuous");
  JAddBoolToObject(req, "sync", true);
  JAddIntToObject(req, "interval", 2);
  notecard.sendRequest(req);
}

void loop()
{
  //Get Notes
  if (J *req = notecard.newRequest("note.get"))
  {
    J *rsp = notecard.requestAndResponse(req);
    notecard.logDebug(JConvertToJSONString(rsp));
    //Get Message Value from Note
    J *data = JGetObject(rsp, "body");
    char *message_char = JGetString(data, "message");
    String message(message_char);
    Serial.println("  ");
    Serial.println("Message:");
    Serial.println(message);
    notecard.deleteResponse(rsp);

    //Turn Light On or Off
    if (message == "on")
    {
      digitalWrite(13, HIGH);
    }
    else if (message == "off")
    {
      digitalWrite(13, LOW);
    }
    else{
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
    }
  }

  // Determine How Many Notes are in Que.  If Over 1 Delete a Note
  if (J *req = notecard.newRequest("file.changes"))
  {
    JAddStringToObject(req, "file", "data.qi");
    J *rsp = notecard.requestAndResponse(req);
    total = JGetInt(rsp, "total");
    Serial.print("total messages: ");
    Serial.println(total);
    notecard.deleteResponse(rsp);
  }

  if (total > 1)
  {
    if (J *req = notecard.newRequest("note.get"))
    {
      JAddStringToObject(req, "file", "data.qi");
      JAddBoolToObject(req, "delete", true);
      notecard.sendRequest(req);
    }
  }

delay(10000);
}   