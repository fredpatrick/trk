#include <iostream>
#include "GPIOConfig.h"

using namespace trk;

int main() {

    std::cout << "BeagleBoneBlack utility clearing exported gpios" <<std::endl;

    GPIOConfig* gpio_config = GPIOConfig::instance();
    gpio_config->clear_gpios();

    return 0;
}

