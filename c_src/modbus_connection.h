#include <memory>

#include <modbus/modbus.h>

typedef std::unique_ptr<modbus_t, void(*)(modbus_t *)> ModbusCtx;


class ModbusConnection
{
public:
    ModbusConnection(const char *device, int baud);
    ~ModbusConnection();

private:
    ModbusConnection(const ModbusConnection& c);
//    ModbusConnection &operator
private:
    ModbusCtx ctx_;
};