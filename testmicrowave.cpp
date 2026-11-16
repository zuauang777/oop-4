#include "microwave.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

using namespace std;


class TeeBuf : public streambuf
{
    streambuf *A, *B;
public:
    TeeBuf(streambuf* a, streambuf* b) : A(a), B(b) {}
protected:
    int overflow(int c) override
    {
        if (c == EOF) return !EOF;
        bool ok1 = A->sputc((char)c) != EOF;
        bool ok2 = B->sputc((char)c) != EOF;
        return (ok1 && ok2) ? c : EOF;
    }
    int sync() override
    {
        return (A->pubsync() == 0 && B->pubsync() == 0) ? 0 : -1;
    }
};

static const char* YesNo(bool V) { return V ? "OK" : "REJECTED"; }


void DemoOperators(ostream& out)
{
    out << "=========== Частина 1. Демонстрація операторів ===========\n\n";

    MicrowaveOven Oven;
    out << "Піч створена конструктором за замовчуванням:\n";
    Oven.Print(out);

    out << "\nOven = true;   (вмикання живлення)\n";
    Oven = true;
    Oven.Print(out);

    out << "\nOven <<= 320;  -> " << YesNo(Oven <<= 320) << "\n";
    out << "Oven <<= 500;  -> " << YesNo(Oven <<= 500) << "  (500 Вт немає у ряді 160/320/640/800)\n";
    out << "Oven ^= 120;   -> " << YesNo(Oven ^= 120) << "\n";
    out << "Oven ^= 5;     -> " << YesNo(Oven ^= 5) << "  (менше 10 с)\n";
    out << "Oven ^= 4000;  -> " << YesNo(Oven ^= 4000) << "  (більше 60 хв)\n";
    Oven.Print(out);

    out << "\nЗапуск розігріву (метод StartHeating):\n";
    out << "StartHeating() -> " << YesNo(Oven.StartHeating()) << "\n";
    Oven.Print(out);

    out << "\nСпроба змінити потужність під час роботи випромінювача:\n";
    out << "Oven <<= 160;  -> " << YesNo(Oven <<= 160) << "\n";

    out << "\nОстанній крок: вимикання живлення оператором Oven = false;\n";
    Oven = false;
    Oven.Print(out);

    out << "\nПеревірка вимкненої печі (оператори не діють):\n";
    out << "Oven <<= 640;  -> " << YesNo(Oven <<= 640) << "\n";
    out << "Oven ^= 60;    -> " << YesNo(Oven ^= 60) << "\n\n";
}


void RunMenu(MicrowaveOven& Oven, istream& in, ostream& out, bool Interactive)
{
    out << "=========== Частина 2. Керування піччю (команди з "
        << (Interactive ? "клавіатури" : "файлу input.txt") << ") ===========\n";

    string Cmd;
    while (true)
    {
        if (Interactive)
        {
            Oven.Print(out);
            out << "\n [1] Power on          [0] Power off\n"
                   " [2] Select mode       [3] Set microwave power\n"
                   " [4] Set timer         [5] Start heating\n"
                   " [6] Open door         [7] Close door\n"
                   " [8] Finish heating    [e] Exit\n> ";
        }
        if (!(in >> Cmd)) break;
        if (!Interactive) out << "\n> " << Cmd;

        if (Cmd == "e") { out << "\n"; break; }
        else if (Cmd == "1") { Oven = true;  out << "   (Oven = true)\n"; }  
        else if (Cmd == "0") { Oven = false; out << "   (Oven = false)\n"; } 
        else if (Cmd == "2")
        {
            int M; in >> M;
            if (!Interactive) out << " " << M;
            out << "   SelectMode -> " << YesNo(Oven.SelectMode(M)) << "\n";
        }
        else if (Cmd == "3")
        {
            int W; in >> W;
            if (!Interactive) out << " " << W;
            out << "   (Oven <<= " << W << ") -> " << YesNo(Oven <<= W) << "\n"; 
        }
        else if (Cmd == "4")
        {
            int S; in >> S;
            if (!Interactive) out << " " << S;
            out << "   (Oven ^= " << S << ") -> " << YesNo(Oven ^= S) << "\n";   
        }
        else if (Cmd == "5") out << "   StartHeating -> " << YesNo(Oven.StartHeating()) << "\n";
        else if (Cmd == "6") { Oven.OpenDoor();  out << "   Door opened\n"; }
        else if (Cmd == "7") { Oven.CloseDoor(); out << "   Door closed\n"; }
        else if (Cmd == "8")
        {
            double kWh = Oven.FinishHeating();
            out << "   FinishHeating -> energy used: " << kWh << " kWh\n";
        }
        else out << "   Unknown command\n";

        if (!Interactive) Oven.Print(out);
    }
}

int main()
{
    ofstream File("output.txt");
    TeeBuf Tee(cout.rdbuf(), File.rdbuf());
    ostream out(&Tee);

    DemoOperators(out);

    MicrowaveOven Oven(GRILL, 320, 60);   
    out << "Об'єкт для частини 2 створено конструктором з трьома параметрами:\n";
    Oven.Print(out);
    out << "\n";

    ifstream In("input.txt");
    if (In) RunMenu(Oven, In, out, false);
    else    RunMenu(Oven, cin, out, true); 

    out << "Роботу завершено. Результати збережено у файлі output.txt\n";
    return 0;
}
