#include "../src/config.h"
#include "../src/log.h"

MyFrame::ConfigVar<int>::ptr g_int_value_config = 
    MyFrame::Config::Lookup("system.port", (int)8080, "system port");

MyFrame::ConfigVar<float>::ptr g_float_value_config = 
    MyFrame::Config::Lookup("system.value", (float)10.2f, "system value");
int main(int args, char** argv) {
    MYFRAME_LOG_INFO(MYFRAME_LOG_ROOT()) << g_int_value_config->getValue();
    MYFRAME_LOG_INFO(MYFRAME_LOG_ROOT()) << g_float_value_config->toString();
    return 0;
}

