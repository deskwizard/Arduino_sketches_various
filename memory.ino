void recall_settings(){
  
  if (!EEPROM.read(0)){
    // Address 0 used as flag for first power on / recall stored settings
    Serial.println(F("EEPROM blank"));
  }
  else {
  Serial.println(F("Recalling settings from EEPROM"));
  }
}

