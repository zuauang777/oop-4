#ifndef MICROWAVE_H
#define MICROWAVE_H

#include <ostream>


enum OvenMode { MICROWAVE = 0, GRILL = 1 };

class MicrowaveOven
{
private:
   
    bool Power;        
    int  Mode;       
    int  MicroPower; 
    int  Timer;       
    bool Emitter;      
    bool Heater;     
    bool ResumeEmit;  
    bool ResumeHeat;   
    bool DoorOpen;     

    static const int GRILL_POWER = 1000;  
    static const int MIN_TIMER   = 10;   
    static const int MAX_TIMER   = 3600;  

    bool IsWorking() const { return Emitter || Heater; }
    static bool IsValidMicroPower(int P);

public:
    
    MicrowaveOven();                                    
    explicit MicrowaveOven(int NewMode);                
    MicrowaveOven(int NewMode, int NewMicroPower);      
    MicrowaveOven(int NewMode, int NewMicroPower, int NewTimer); 
    MicrowaveOven(const MicrowaveOven& Other);         
    ~MicrowaveOven();                                 

   
    void   Reset();                  
    void   TurnOn();                
    void   TurnOff();               
    bool   SelectMode(int NewMode); 
    bool   SetMicroPower(int Watts); 
    bool   SetTimer(int Seconds);    
    bool   StartHeating();          
    void   OpenDoor();               
    void   CloseDoor();              
    double FinishHeating();          

    
    void operator = (bool State);
    bool operator <<= (int Watts);
    bool operator ^= (int Seconds);


    bool GetPower()      const { return Power; }
    int  GetMode()       const { return Mode; }
    int  GetMicroPower() const { return MicroPower; }
    int  GetTimer()      const { return Timer; }
    bool GetEmitter()    const { return Emitter; }
    bool GetHeater()     const { return Heater; }
    bool GetDoorOpen()   const { return DoorOpen; }
    void Print(std::ostream& Out) const;  
};

#endif
