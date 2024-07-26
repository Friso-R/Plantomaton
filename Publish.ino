void pubSensors(){
  broker.publish("tmp",   String(sensors.temperature));
  broker.publish("vocht", String(sensors.humidity));
  //broker.publish("vpd",   String(sensors.vpd));
  broker.publish("soil",  String(sensors.soilMoisture));

  broker.publish("F1", String(sensors.waves[0]));
  broker.publish("F2", String(sensors.waves[1]));
  broker.publish("F3", String(sensors.waves[2]));
  broker.publish("F4", String(sensors.waves[3]));
  broker.publish("F5", String(sensors.waves[6]));
  broker.publish("F6", String(sensors.waves[7]));
  broker.publish("F7", String(sensors.waves[8]));
  broker.publish("F8", String(sensors.waves[9]));
}

// This function is executed when some device publishes a message to a topic that the ESP32 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;

  for (int i = 0; i < length; i++)  
    messageTemp += (char)message[i];
    
  broker.publish(topic, "sent!!!");

  if(topic == "infob3it/student033/lamp"){
    if(messageTemp == "on"){
      lamp.on();  
      lampFans.on();
      broker.publish("gordijn", "updown"); 
      }
    if(messageTemp == "off"){
      lamp.off(); 
      lampFans.off();}
  }
  if(topic == "infob3it/student033/pomp"){
    if(messageTemp == "on")
      pomp.on();
    if(messageTemp == "off")
      pomp.off();    
  }
  if(topic == "infob3it/student033/humi"){
    if(messageTemp == "onoff")
      humi.toggle();
  }
  if(topic == "infob3it/student033/optimal"){
    sscanf(messageTemp.c_str(), "%f %f %f %f %f", &optimal[0], &optimal[1], &optimal[2], &optimal[3], &optimal[4]);

    int i;
    for (i=0; i<5; i++) {
      Serial.println(String(optimal[i]));
    }
  }
}
