#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <arpa/inet.h>

#include <erl_interface.h>
#include "modbus_connection.h"

typedef std::unique_ptr<ETERM, void(*)(ETERM *)> ErlTerm;

using namespace std;

void help(const char *binary)
{
    cerr << binary << " <device> <baud>" << endl;
}


ErlTerm ErlDecode(char *bufp)
{
    ETERM * term = erl_decode(reinterpret_cast<unsigned char *>(bufp));
    if (NULL == term)
    {
        throw runtime_error("Failed to decode term");
    }
    return ErlTerm(term, erl_free_compound);
}

int ParseBaud(const char * baud_str)
{
    istringstream str_speed(baud_str);
    int baud = 0;
    str_speed >> baud;

    if (0 == baud || !str_speed.eof())
    {
        throw invalid_argument("Invalid baud");
    }

    return baud;
}

void ReadCmd(std::istream &s)
{
    uint16_t network_length = 0;
    s.read(reinterpret_cast<char *>(&network_length), sizeof(network_length));
    uint16_t length = ntohs(network_length);
    vector<char> command(length);
    s.read(&command[0], length);
    ErlDecode(&command[0]);
}

int main(int argc, const char *argv[])
{
    cerr << "Starting modbus port" << endl;

    int return_value = 0;
    try
    {
        if(argc < 3)
        {
            help(argv[0]);
            throw invalid_argument("Number or cmd-line arugments");
        }
        const char * device(argv[1]);
        int baud = ParseBaud(argv[2]);

        ModbusConnection connection(device, baud);

        cin.exceptions(istream::failbit | istream::badbit | istream::eofbit);
        cout.exceptions(ostream::failbit | ostream::badbit | ostream::eofbit);

        erl_init(NULL, 0);
        ReadCmd(cin);
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        return_value = 1;
    }
    return return_value;
}