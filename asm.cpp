/*****************************************************************************
TITLE: Assembler CPP Program
AUTHOR:   		KUSHAGRA YADAV
ROLL NUMBER:	2001CS41
Declaration of Authorship
This .cpp file, asm.cpp, is part of the assignment of CS322 at the
department of Computer Science and Engineering, IIT Patna .
*****************************************************************************/
#include <bits/stdc++.h>
using namespace std;
// mnemonics
// Mnemonic classification
// type = 0 : No Operand
// type = 1 : Value Operand if label provided then value of label is used
// type = 2 : 'data'
// type = 3 : 'set'
// type = 4 : Offset calculated as instruction is branch instruction
// first value is type, second is opcode
map<string, pair<int, int>> types;
void initialise()
{
    types["ldc"] = {1, 0};
    types["adc"] = {1, 1};
    types["ldl"] = {1, 2};
    types["stl"] = {1, 3};
    types["ldnl"] = {1, 4};
    types["stnl"] = {1, 5};
    types["add"] = {0, 6};
    types["sub"] = {0, 7};
    types["shl"] = {0, 8};
    types["shr"] = {0, 9};
    types["adj"] = {1, 10};
    types["a2sp"] = {0, 11};
    types["sp2a"] = {0, 12};
    types["call"] = {4, 13};
    types["return"] = {0, 14};
    types["brz"] = {4, 15};
    types["brlz"] = {4, 16};
    types["br"] = {4, 17};
    types["HALT"] = {0, 18};
    types["data"] = {2, -1};
    types["SET"] = {3, -1};
}
// error defn
vector<string>
    errors = {
        "No Error in Pass One",
        "Duplicate Label Defined",
        "Invalid Label",
        "Invalid Mnemonic",
        "Missing Operand",
        "Not A Number",
        "Unexpected Operand",
        "Extra on End of Line",
        "All Labels not defined",
        "Undefined Error"};
// symbol table
// string is the symbol,integer is its address in the map symboltable
map<string, int> symboltable;
// insert a label if its not present and return 1, return 0 if its already present
int insertsymbotable(string &label)
{
    if (symboltable.find(label) != symboltable.end())
        return 0;
    symboltable[label] = -1;
    return 1;
}
// assign address as PC, if its present or else insert and assign it, return 0
// return 1 if address already assigned
int assignsymboltable(string &label, int PC)
{
    if (symboltable.find(label) == symboltable.end())
        symboltable[label] = -1;
    if (symboltable[label] != -1)
        return 0;
    symboltable[label] = PC;
    return 1;
}
// Return the address of the given label
// Return -1 if the address does not exist
int getaddresssymboltable(string &label)
{
    if (symboltable.find(label) == symboltable.end())
        return -1;
    else
        return symboltable[label];
}
bool hasunassignedsymboltable()
{
    for (auto it : symboltable)
    {
        if (it.second == -1)
            return 1;
    }
    return 0;
}
bool updatesymboltable(string &label, int address)
{
    if (symboltable.find(label) == symboltable.end())
        return 0;
    symboltable[label] = address;
    return 1;
}
void printsymboltable(ofstream &fout)
{
    fout << "SYMBOL TABLE\n";
    for (auto it : symboltable)
        fout << it.first << "\t" << it.second << "\n";
}
// removes comments from a particular line so that it doesnot interfere with command parsing later in the string
void removecomment(string &line)
{
    int i;
    for (i = 0; i < line.length(); i++)
    {
        if (line[i] == ';')
            break;
    }
    line.resize(i);
}
// remove : and check if its present or not, if present return 1, else return 0
int haslabel(string &line)
{
    int i;
    for (i = 0; i < line.length(); i++)
    {
        if (line[i] == ':' && i!=0)
        {
            line[i] = ' ';
            return 1;
        }
    }
    return 0;
}
// check if label present is valid
bool isvalidlabel(string &label)
{
    if (label.size() == 0 || (!isalpha(label[0]) && label[0] != '_'))
        return 0;
    for (int i = 0; i < label.size(); i++)
    {
        if ((!isalnum(label[i]) && label[i] != '_'))
            return 0;
    }
    return 1;
}

bool isvalidnumber(string &num)
{
    istringstream a(num);
    int x;
    if (a.peek() == '0' && a.rdbuf()->in_avail() > 1)
    {
        a.get();
        if (a.peek() == 'x' || a.peek() == 'X')
        {
            a.get();
            a >> hex >> x;
        }
        else
            a >> oct >> x;
    }
    else
    {
        a >> x;
    }
    if (a.rdbuf()->in_avail() == 0)
        return 1;
    else
        return 0;
}
int tonumber(string &num)
{
    istringstream a(num);
    int x;
    if (a.peek() == '0' && a.rdbuf()->in_avail() > 1)
    {
        a.get();
        if (a.peek() == 'x' || a.peek() == 'X')
        {
            a.get();
            a >> hex >> x;
        }
        else
            a >> oct >> x;
    }
    else
    {
        a >> x;
    }
    return x;
}
// funtion to check if the Mnemonic is valid,
// return -1
//  return 0 : No Operand
//  return = 1 : Value Operand
//  return = 2 : 'data'
//  return = 3 : 'set'
//  return = 4 : Offset Operand
int isvalidmnemonic(string &word)
{
    if (types.find(word) != types.end())
        return types[word].first;
    else
        return -1;
}
bool requiresoperand(int type)
{
    return type > 0;
}
int puterror(int code, int linenum, ofstream &log)
{
    if (code && code < 8)
    {
        log << "ERROR:\t" << errors[code] << " at " << linenum << "\n";
    }
    else if (code == 8)
    {
        log << "ERROR:\t" << errors[code] << "\n";
    }
    else if (code)
    {
        log << "ERROR:\t" << errors[code] << "\n";
    }
    else
    {
        log << errors[code] << "\n";
    }
    return code;
}
void whitesapceconsume(istringstream &a)
{
    while (isspace(a.peek()))
        a.get();
}
//      0   "No Error in Pass One",
//      1   "Duplicate Label Defined",
//      2   "Invalid Label",
//      3   "Invalid Mnemonic",
//      4   "Missing Operand",
//      5   "Not A Number",
//      6   "Unexpected Operand",
//      7   "Extra on End of Line",
//      8   "All Labels not defined",
//      9   "Undefined Error"

// first pass to detect labels, assign them adress and check for errors
int firstpass(ifstream &assembly, ofstream &log)
{
    string line;
    int linenum = 0, error = 0, PC = 0;
    while (!getline(assembly, line).fail() && (++linenum))
    {
        removecomment(line);
        bool labelled = haslabel(line);
        // cout<<linenum<<" "<<line<<"eol\n";
        istringstream a(line);
        whitesapceconsume(a);
        if (a.rdbuf()->in_avail() == 0)
            continue;
        // cout<<linenum<<" "<<line<<"eol\n";
        string word, label;
        // if : is present check for label
        if (labelled)
        {
            whitesapceconsume(a);
            a >> label;
            // cout<<"/"<<label<<"/\n";
            if (isvalidlabel(label))
            {
                if (!assignsymboltable(label, PC))
                {
                    puterror(1, linenum, log);
                    error++;
                    continue;
                }
            }
            else
            {
                puterror(2, linenum, log);
                error++;
                continue;
            }
        }
        // if nothing left on line after label continue
        whitesapceconsume(a);
        // cout<<"stream"<<(char)a.peek()<<"\n";
        if (a.rdbuf()->in_avail() == 0)
        {
            // cout<<"contd\n";
            continue;
        }
        // check for mnemomic after label
        a >> word;
        // cout<<"/"<<word<<"/\n";
        int type = isvalidmnemonic(word);
        // funtion to check if the Mnemonic is valid,
        // return -1
        //  return 0 : No Operand
        //  return = 1 : Value Operand
        //  return = 2 : 'data'
        //  return = 3 : 'set'
        //  return = 4 : Offset Operand
        if (type == -1)
        {
            puterror(3, linenum, log);
            error++;
            continue;
        }
        whitesapceconsume(a);
        // if it requires operand and nothing is present, puterror and continue to next line
        if (requiresoperand(type))
        {
            if (a.rdbuf()->in_avail() == 0)
            {
                puterror(4, linenum, log);
                error++;
                continue;
            }
            // if operand is present, it can be either label or number, if label store in symbol table, if number and not set or data, store in literal table,
            // FOR SET change address of current label to value specified
            // if niether number nor label is provided correctly then error 5(unexpected operand)
            else
            {
                a >> word;
                // cout<<"/"<<word<<"/\n";
                if (isvalidlabel(word))
                {
                    insertsymbotable(word);
                }
                else if (isvalidnumber(word))
                {
                    // FOR SET
                    if (type == 3)
                    {
                        int x = updatesymboltable(label, tonumber(word));
                        if (!x)
                        {
                            puterror(9, linenum, log);
                            error++;
                            continue;
                        }
                    }
                    // else if (type < 3)
                    //     insertliteraltable(tonumber(word));
                }
                else
                {
                    puterror(5, linenum, log);
                    error++;
                    continue;
                }
            }
        }
        // if operand not required and it is present
        else if (a.rdbuf()->in_avail() != 0 && (type == 0))
        {
            puterror(6, linenum, log);
            error++;
            continue;
        }
        whitesapceconsume(a);
        if (a.rdbuf()->in_avail() != 0)
        {
            puterror(7, linenum, log);
            error++;
            continue;
        }
        PC++;
    }
    if (hasunassignedsymboltable())
    {
        error++;
        puterror(8, linenum, log);
    }
    if (error)
        return 0;
    else
        puterror(0, linenum, log);
    return 1;
}
// print given integer to hex string with 4 digits
void printPC(ofstream &lst, int PC)
{
    char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char ans[9];
    ans[8] = '\0';
    // lst << PC << ' ';
    if (PC < 0)
    {
        int x = 0x1000;
        PC = x - PC;
    }
    // lst << PC % 16 << " ";
    for (int i = 7; i >= 0; i--)
    {
        ans[i] = digits[PC % 16];
        PC /= 16;
    }
    // cout << ans << "\n";
    // printf("%s\n", ans);
    lst << ans << ' ';
}
void printlbldataset(ofstream &lst, ofstream &obj, int PC)
{
    char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char ans[9];
    ans[8] = '\0';
    // lst << PC << ' ';
    if (PC < 0)
    {
        int x = 0x1000000;
        PC = x + PC;
    }
    // lst << PC % 16 << " ";
    for (int i = 7; i >= 0; i--)
    {
        ans[i] = digits[PC % 16];
        PC /= 16;
    }
    // cout << ans << "\n";
    // printf("%s\n", ans);
    lst << ans << ' ';
     string y=ans;
     istringstream l(y);
     uint fin;
     l >> hex >> fin;
     //lst << hex << fin << ' ';
    obj.write((char *)&fin, sizeof(uint));
}
void encoder(ofstream &lst, ofstream &obj, int opcode, int operand)
{
    char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char ans[9];
    ans[8] = '\0';
    if (operand < 0)
    {
        int x = 0x1000000;
        operand = x + operand;
    }
    if (opcode == -1)
        opcode = 0;
    for (int i = 7; i >= 6; i--)
    {
        ans[i] = digits[opcode % 16];
        opcode /= 16;
    }
    for (int i = 5; i >= 0; i--)
    {
        ans[i] = digits[operand % 16];
        operand /= 16;
    }
    // cout << ans << "\n";
    // printf("%s\n", ans);
    lst << ans << ' ';
     string y=ans;
     istringstream l(y);
     uint fin;
     l >> hex >> fin;
     //lst << hex << fin << ' ';
    obj.write((char *)&fin, sizeof(uint));
}
int secondpass(ifstream &assembly, ofstream &obj, ofstream &lst)
{
    string line;
    int linenum = 0, PC = 0;
    while (!getline(assembly, line).fail() && (++linenum))
    {
        string l = line;
        removecomment(l);
        bool labelled = haslabel(l);
        istringstream a(l);
        whitesapceconsume(a);
        if (a.rdbuf()->in_avail() == 0)
        {
            lst << "              " << line << "\n";
            continue;
        }
        printPC(lst, PC);
        string label, word;
        if (labelled)
        {
            a >> label;
            whitesapceconsume(a);
        }
        if (a.rdbuf()->in_avail() == 0)
        {
            lst << "              " << line << "\n";
            continue;
        }
        a >> word;
        int type = isvalidmnemonic(word);
        int opcode = types[word].second;
        if (type == 0)
        {
            encoder(lst, obj, opcode, 0);
        }
        else if (type == 2 || type == 3)
        {
            whitesapceconsume(a);
            a >> word;
            if (isvalidnumber(word))
                printlbldataset(lst, obj, tonumber(word));
            else
                printlbldataset(lst, obj, symboltable[word]);
        }
        else if (type == 1)
        {
            whitesapceconsume(a);
            a >> word;
            if (isvalidnumber(word))
            {
                encoder(lst, obj, opcode, tonumber(word));
            }
            else
            {
                encoder(lst, obj, opcode, symboltable[word]);
            }
        }
        else if (type == 4)
        {
            whitesapceconsume(a);
            a >> word;
            if (isvalidnumber(word))
            {
                encoder(lst, obj, opcode, tonumber(word));
            }
            else
            {
                encoder(lst, obj, opcode, symboltable[word] - PC - 1);
            }
        }
        lst << line << "\n";
        PC++;
    }
    return 1;
}
int main(int argc, char *argv[])
{
    initialise();
    string filename = argv[1];
    ifstream assembly;
    ofstream log;
    assembly.open(filename.c_str());
    filename.resize(filename.size() - 4);
    string objname = filename + ".o";
    string listingname = filename + ".l";
    string logfile = filename + ".log";
    log.open(logfile.c_str());
    if (!firstpass(assembly, log))
        return 0;
    assembly.close();
    assembly.open((filename + ".asm").c_str());
    ofstream obj(objname,ios::out | ios::binary);
    ofstream lst;
    lst.open(listingname);
    secondpass(assembly, obj, lst);
    log << "No error in second Pass";
    assembly.close();
    obj.close();
    lst.close();
    log.close();
}
