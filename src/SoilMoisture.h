class SoilMoisture{
public:
  int getSoilMoisture()
  {   
    return analogRead(34);
  }
}; 