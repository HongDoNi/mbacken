#include <string>
#include <map>
#include "packetSegment/Data.h"

class Packet 
{
  
public:
    Packet()
    {
       
    }

    Packet(string command, Data data)            
    {
        this->command = command;
        this->data = data;
    }

    Packet(int line, string command, Data data)     
    {
        this->line = line;
        this->command = command;
        this->data = data;
    }

    int getLine() 
    {
        return line;
    }

    void setLine(int line) 
    {
        this->line = line;
    }

    string getTerminalAddress() 
    {
        return terminalAddress;
    }

    void setTerminalAddress(string terminalAddress) 
    {
        this->terminalAddress = terminalAddress;
    }

    string getCommand() 
    {
        return command;
    }

    void setCommand(string command) 
    {
        this->command = command;
    }

    Data getData()    
    {
        return data;
    }

    void setData(Data data)     
    {
        this->data = data;
    }

private:
    /**
     * 线路编号，0为终端本身，其它为测量点
     */
    int line;

    /**
     * 终端号
     */
    string terminalAddress;

    /**
     * 命令名
     */
    string command;

    /**
     * 数据
     */
    Data data;
    
};
