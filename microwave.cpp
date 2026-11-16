#include "microwave.h"
#include <iomanip>

using namespace std;

bool MicrowaveOven::IsValidMicroPower(int P)
{
    return P == 160 || P == 320 || P == 640 || P == 800;
}



MicrowaveOven::MicrowaveOven()
{
    Reset();
}

MicrowaveOven::MicrowaveOven(int NewMode)
{
    Reset();
    TurnOn();
    SelectMode(NewMode);
}

MicrowaveOven::MicrowaveOven(int NewMode, int NewMicroPower)
{
    Reset();
    TurnOn();
    SelectMode(NewMode);
    SetMicroPower(NewMicroPower);
}

MicrowaveOven::MicrowaveOven(int NewMode, int NewMicroPower, int NewTimer)
{
    Reset();
    TurnOn();
    SelectMode(NewMode);
    SetMicroPower(NewMicroPower);
    SetTimer(NewTimer);
}

MicrowaveOven::MicrowaveOven(const MicrowaveOven& Other)
    : Power(Other.Power), Mode(Other.Mode), MicroPower(Other.MicroPower),
      Timer(Other.Timer), Emitter(Other.Emitter), Heater(Other.Heater),
      ResumeEmit(Other.ResumeEmit), ResumeHeat(Other.ResumeHeat),
      DoorOpen(Other.DoorOpen)
{
}

MicrowaveOven::~MicrowaveOven()
{
  
    Emitter = Heater = false;
}



void MicrowaveOven::Reset()
{
    Power = false;
    Mode = MICROWAVE;
    MicroPower = 800;
    Timer = 0;
    Emitter = Heater = false;
    ResumeEmit = ResumeHeat = false;
    DoorOpen = false;
}

void MicrowaveOven::TurnOn()
{
    Power = true;
    Mode = MICROWAVE;     
    MicroPower = 800;     
    Timer = 0;          
}

void MicrowaveOven::TurnOff()
{
    Power = false;
    Emitter = Heater = false;
    ResumeEmit = ResumeHeat = false;
}

bool MicrowaveOven::SelectMode(int NewMode)
{
    if (!Power || IsWorking()) return false;
    if (NewMode != MICROWAVE && NewMode != GRILL) return false;
    Mode = NewMode;
    return true;
}

bool MicrowaveOven::SetMicroPower(int Watts)
{
    if (!Power || IsWorking()) return false;
    if (!IsValidMicroPower(Watts)) return false;
    MicroPower = Watts;
    return true;
}

bool MicrowaveOven::SetTimer(int Seconds)
{
    if (!Power) return false;
    if (Seconds < MIN_TIMER || Seconds > MAX_TIMER) return false;
    Timer = Seconds;
    return true;
}

bool MicrowaveOven::StartHeating()
{
  
    if (!Power || DoorOpen || Timer == 0 || IsWorking()) return false;
    if (Mode == MICROWAVE) Emitter = true;
    else                   Heater = true;
    return true;
}

void MicrowaveOven::OpenDoor()
{
  
    if (Emitter) { ResumeEmit = true; Emitter = false; }
    if (Heater)  { ResumeHeat = true; Heater = false; }
    DoorOpen = true;
}

void MicrowaveOven::CloseDoor()
{
    DoorOpen = false;
   
    if (ResumeEmit) { Emitter = true; ResumeEmit = false; }
    if (ResumeHeat) { Heater = true;  ResumeHeat = false; }
}

double MicrowaveOven::FinishHeating()
{
    bool Was = IsWorking() || ResumeEmit || ResumeHeat;
    if (!Was) return 0.0;

    bool UsedEmitter = Emitter || ResumeEmit;
    int  Watts = UsedEmitter ? MicroPower : GRILL_POWER;
    double kWh = Watts * (double)Timer / 3600.0 / 1000.0;   

    Emitter = Heater = false;
    ResumeEmit = ResumeHeat = false;
    Timer = 0;
    return kWh;
}



void MicrowaveOven::operator = (bool State)
{
    State ? TurnOn() : TurnOff();
}


bool MicrowaveOven::operator <<= (int Watts)
{
    return SetMicroPower(Watts);
}


bool MicrowaveOven::operator ^= (int Seconds)
{
    return SetTimer(Seconds);
}


void MicrowaveOven::Print(ostream& Out) const
{
    Out << "---------------- STATE ----------------\n"
        << left
        << setw(12) << "Power"       << ": " << (Power ? "ON" : "OFF") << "\n"
        << setw(12) << "Mode"        << ": " << (Mode == MICROWAVE ? "MICROWAVE" : "GRILL") << "\n"
        << setw(12) << "MicroPower"  << ": " << MicroPower << " W\n"
        << setw(12) << "Timer"       << ": " << Timer / 60 << " min " << Timer % 60 << " s\n"
        << setw(12) << "Emitter"     << ": " << (Emitter ? "ON" : "OFF") << "\n"
        << setw(12) << "Heater"      << ": " << (Heater ? "ON" : "OFF") << "\n"
        << setw(12) << "Resume Emit" << ": " << (ResumeEmit ? "YES" : "NO") << "\n"
        << setw(12) << "Resume Heat" << ": " << (ResumeHeat ? "YES" : "NO") << "\n"
        << setw(12) << "Door"        << ": " << (DoorOpen ? "OPEN" : "CLOSED") << "\n"
        << "---------------------------------------\n";
}
