#include "../A20-gpio.hpp"

#include <assert.h>
#include <iostream>


using namespace A20;

int main() {
    
    GPIO_input *test;
    GPIO_input *test2;

    GPIO::init();
    test  = GPIO::get_input(256);
    test2 = GPIO::get_input(256);
    assert(test==test2);
    
    std::cout << "GPIO 256 reads (no pull-resistor): " << test->get() << std::endl;

    GPIO::free(256);
    test = GPIO::get_input(256, PULL_UP);

    std::cout << "GPIO 256 reads (pulled UP): " << test->get() << std::endl;

    GPIO::free(256);
    test = GPIO::get_input(256, PULL_DOWN);

    std::cout << "GPIO 256 reads (pulled DOWN): " << test->get() << std::endl;

    GPIO::free(256);
    GPIO::set_periphery_mode(256);

    std::cout << "GPIO 256 now in periphery mode" << std::endl;

    GPIO_output *test_out;

    GPIO::free(256);    
    test_out = GPIO::get_output(256);

    test_out->set(true);
    std::cout << "GPIO 256 is now enabled as output and producing '1' (wait 5s)." << std::endl;
    
    sleep(5);
    
    test_out->set(false);
    std::cout << "GPIO 256 is now enabled as output and producing '0' (wait 5s)." << std::endl;
    
    sleep(5);
    GPIO::free(256);
    
    return 0;
}
