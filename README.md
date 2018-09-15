# stm32-hal-cpp
Convenient, zero-overhead C++ wrapper for the STM32 HAL library routines

# Usage
- Clone this repository into your project's 3rd-party directory or add it as a git sub-module:
```
git submodule add 'https://github.com/tomikaa87/stm32-hal-cpp.git' 3rdparty/stm32-hal-cpp
```
- Add the `stm32-hal-cpp/include` directory to your project's include search path
- Include the necessary headers, e.g. `gpio.hpp` for the GPIO API

Please keep in mind this library needs a C++11 compatible compiler.

# GPIO examples

## Setup pin A0 as a digital push-pull output and do stuff with it
```cpp
#include "gpio.hpp"

void gpio_test()
{
    using pin_a0 = gpio::output_pin<gpio::gpio_pin<
            gpio::port::a,
            gpio::pins<0>,
            gpio::output_pp_mode,
            gpio::speed::low,
            gpio::pull::nopull>>;

    pin_a0::init();
    pin_a0::set();
}
```
In this example, `gpio::gpio_pin` defines a generic structure for the GPIO pin and `gpio::output_pin` defines the necessary functions (`set()`, `reset()`, `toggle()` etc.) for controlling the output pin.

## Setup pins B5, B6, B7 and B8 simultaneously as digital push-pull outputs and do stuff with them
```cpp
#include "gpio.hpp"

void gpio_test()
{
    using test_pins = gpio::output_pin<gpio::gpio_pin<
            gpio::port::b,
            gpio::pins<5, 6, 7, 8>,
            gpio::output_pp_mode,
            gpio::speed::low,
            gpio::pull::nopull>>;

    test_pins::init();
    test_pins::set();
}
```
In this example we use the same way to define the output pins, but as you can see, we pass multiple pin indices to `gpio::pins<>`. This way the initialization and all the operations are done in a single step without any overhead.
Calling `set()` will output a logical 1 on all the pins.

## Setup pin B4 as a digital input with internal pull-up and read it
```cpp
void gpio_test()
{
    using input_b4 = gpio::input_pin<gpio::gpio_pin<
            gpio::port::b,
            gpio::pins<4>,
            gpio::input_mode,
            gpio::speed::high,
            gpio::pull::up>>;

    input_b4::init();
    bool high = input_b4::read();
}
```
In this example we used `gpio::input_mode` to instruct the API to setup the pin as an input. `gpio::pull::up` enables the internal pull-up resistor. If we `read()` the port's value when it's unconnected or connected to VCC, we will get a `true` value, but if we connect the pin to GND, we will get `false`.
