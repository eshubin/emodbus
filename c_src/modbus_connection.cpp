#include <stdexcept>
#include <iostream>

#include "modbus_connection.h"

using namespace std;

ModbusCtx ModbusNewRtu(const char * device, int baud)
{
    modbus_t *ctx = modbus_new_rtu(device, baud, 'N', 8, 1);
    if (NULL == ctx)
    {
        throw runtime_error("Failed to init to the device");
    }
    return ModbusCtx(ctx, modbus_free);
}

void ModbusSetSlave(modbus_t *ctx)
{
    if (0 != modbus_set_slave(ctx, 1))
    {
        throw runtime_error("Failed to set slave");
    }
}


ModbusConnection::ModbusConnection(const char *device, int baud) :
    ctx_(ModbusNewRtu(device, baud))
{
    ModbusSetSlave(ctx_.get());
    cerr << "Connecting to " << device << " on speed " << baud << endl;
    if (-1 == modbus_connect(ctx_.get()))
    {
        throw runtime_error("Failed to connect to the device");
    }
}

ModbusConnection::~ModbusConnection()
{
    modbus_close(ctx_.get());
}