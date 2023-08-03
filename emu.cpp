/*****************************************************************************
TITLE: EMULATOR CPP Program
AUTHOR:   		KUSHAGRA YADAV
ROLL NUMBER:	2001CS41
Declaration of Authorship
This .cpp file, emu.cpp, is part of the assignment of CS322 at the
department of Computer Science and Engineering, IIT Patna .
*****************************************************************************/
#include <bits/stdc++.h>
using namespace std;
int A, B, sp = 0xFFFFFF, PC;
uint32_t memory[0x1000000];
vector<string> mnemonic = {"ldc",
                           "adc",
                           "ldl",
                           "stl",
                           "ldnl",
                           "stnl",
                           "add",
                           "sub",
                           "shl",
                           "shr",
                           "adj",
                           "a2sp",
                           "sp2a",
                           "call",
                           "return",
                           "brz",
                           "brlz",
                           "br",
                           "HALT"};

int mnemonictype(uint32_t x)
{
    return x % 256;
}
int operandvalue(uint32_t x)
{
    x = x / 256;
    if (x >= 0x800000)
        x = x - 0x1000000;
    return x;
}
void printOptions()
{
    cout << "usage: emu [options] file.o\n";
    cout << "-trace\tshow instruction trace\n";
    cout << "-before\tshow memory dump before execution\n";
    cout << "-after\tshow memory dump after execution\n";
    cout << "-isa\tdisplay ISA\n";
}
void printISA()
{
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printOptions();
        return 1;
    }
    bool trace = 0;
    bool isa = 0;
    bool memdumpbefore = 0;
    bool memdumpafter = 0;
    for (int i = 1; i < argc; i++)
    {
        string x = argv[i];
        if (x == "-trace")
            trace = 1;
        else if (x == "-before")
            memdumpbefore = 1;
        else if (x == "-after")
            memdumpafter = 1;
        else if (x == "-isa")
            isa = 1;
    }
    ifstream obj(argv[argc - 1], ios_base::binary);
    string name = argv[argc - 1];
    name.resize(name.size() - 2);
    ofstream out(name + "_trace.txt");
    int i = 0;
    if (isa)
    {
        printISA();
    }
    while (obj.read((char *)&memory[i], sizeof(memory[i])))
    {
        i++;
    }
    int count = 0;
    if (memdumpbefore)
    {
        ofstream memdumpbefore(name + "_membefore.txt");
        for (int j = 0; j < i; j++)
        {
            cout << hex << memory[j] << "\n";
            memdumpbefore << hex << memory[j] << "\n";
        }
        memdumpbefore.close();
    }
    while (1)
    {
        count++;
        int type = mnemonictype(memory[PC]);
        int value = operandvalue(memory[PC]);
        if (trace)
        {
            out << hex << "A=" << A << " B=" << B << " SP=" << sp << " PC=" << PC << " " << mnemonic[type] << " " << value << "\n";
            cout << hex << "A=" << A << " B=" << B << " SP=" << sp << " PC=" << PC << " " << mnemonic[type] << " " << value << "\n";
        }

        PC += 1;
        if (type == 0)
        {
            // ldc 0 value B=A, A=value
            B = A;
            A = value;
        }
        else if (type == 1)
        {
            // adc 1 value A=A+value
            A += value;
        }
        else if (type == 2)
        {
            // ldl 2 offset B=A,A=memory[sp+offset]
            B = A;
            A = memory[sp + value];
        }
        else if (type == 3)
        {
            // stl 3 offset memory[sp+offset]=A,A=B
            memory[sp + value] = A;
            A = B;
        }
        else if (type == 4)
        {
            // ldnl 4 offset A=memory[A+offset]
            A = memory[A + value];
        }
        else if (type == 5)
        {
            // stnl 5 offset memory[A+offset]=B
            memory[A + value] = B;
        }
        else if (type == 6)
        {
            // add 6 A=B+A
            A = B + A;
        }
        else if (type == 7)
        {
            // sub 7 A=B-A
            A = B - A;
        }
        else if (type == 8)
        {
            // shl 8 A=B<<A
            A = B << A;
        }
        else if (type == 9)
        {
            // shr 9 A=B>>A
            A = B >> A;
        }
        else if (type == 10)
        {
            // adj 10 value sp=sp+value
            sp = sp + value;
        }
        else if (type == 11)
        {
            // as2p 11 sp=A,A=B
            sp = A;
            A = B;
        }
        else if (type == 12)
        {
            // sp2a 12 B=A,A=sp
            B = A;
            A = sp;
        }
        else if (type == 13)
        {
            // call 13 offset B=A,A=PC,PC=PC+offset
            B = A;
            A = PC;
            PC = PC + value;
        }
        else if (type == 14)
        {
            // return 14 PC=A,A=B
            PC = A;
            A = B;
        }
        else if (type == 15)
        {
            // brz 15 offset if(A==0) PC=PC+offset
            if (A == 0)
                PC = PC + value;
        }
        else if (type == 16)
        {
            // brlz 16 offset if(A<0) PC=PC+offset
            if (A < 0)
                PC = PC + value;
        }
        else if (type == 17)
        {
            // br 17 offset PC=PC+offset
            PC = PC + value;
        }
        else if (type == 18)
        {
            // HALT 18
            // out << endl
            //     << "a " << A << "\tb " << B << "\tsp " << sp << "\tPC " << PC << endl;
            break;
        }

        // out << endl
        //    << "a " << A << "\tb " << B << "\tsp " << sp << "\tPC " << PC << endl;
    }
    // out << memory[21] << " " << memory[22];
    cout << dec << "NO OF INSTRUCTIONS EXECUTED: " << count << "\n";
    out << dec << "NO OF INSTRUCTIONS EXECUTED: " << count << "\n";
    if (memdumpafter)
    {
        ofstream memdumpafter(name + "_memdumpafter.txt");
        for (int j = 0; j < i; j++)
        {
            cout << hex <<j<<" "<< memory[j] << "\n";
            memdumpafter << hex <<j<<" "<< memory[j] << "\n";
        }
        memdumpafter.close();
    }
    out.close();
}