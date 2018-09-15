//
// C++ Wrapper for STM32 HAL GPIO routines
//
// Created on 2018-09-15 by Tamas Karpati <tomikaa87@gmail.com>
//

#ifndef GPIO_HPP_
#define GPIO_HPP_

// Minimum necessary version is C++11
#if __cplusplus >= 201103l

#include <stdint.h>
#include <initializer_list>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include "utils.hpp"

namespace gpio
{

    namespace port
    {
        struct a
        {
            static inline GPIO_TypeDef* get()
            {
                return GPIOA;
            }

            static inline void enable_clock()
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
            }
        };

        struct b
        {
            static inline GPIO_TypeDef* get()
            {
                return GPIOB;
            }

            static inline void enable_clock()
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
            }
        };

        struct c
        {
            static inline GPIO_TypeDef* get()
            {
                return GPIOC;
            }

            static inline void enable_clock()
            {
                __HAL_RCC_GPIOC_CLK_ENABLE();
            }
        };

        struct d
        {
            static inline GPIO_TypeDef* get()
            {
                return GPIOD;
            }

            static inline void enable_clock()
            {
                __HAL_RCC_GPIOD_CLK_ENABLE();
            }
        };

        struct e
        {
            static inline GPIO_TypeDef* get()
            {
                return GPIOE;
            }

            static inline void enable_clock()
            {
                __HAL_RCC_GPIOE_CLK_ENABLE();
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////

    template <uint32_t... _pins>
    struct pins
    {
        static const uint32_t value = utils::flag_disjunction<_pins...>::value;
    };

    struct all_pins
    {
        static const uint32_t value = GPIO_PIN_All;
    };

    //////////////////////////////////////////////////////////////////////////

    namespace modes
    {
        enum io_dir
        {
            input = 0,
            output,
            alternate,
            analog,
        };

        enum out_type
        {
            push_pull = 0,
            open_drain = 0x10
        };

        enum io_config
        {
            ext_interrupt = 0x10000,
            ext_event = 0x20000
        };

        enum trig_detect
        {
            rising_edge = 0x100000,
            falling_edge = 0x200000,
            both_edges = 0x300000
        };

        enum gpio_exti
        {
            gpio_mode = 0x10000000,
            exti_mode = 0x20000000
        };
    }

    template <uint32_t... _modes>
    struct mode
    {
        static const uint32_t value = utils::disjunction<_modes...>::value;
    };

    using input_mode = mode<modes::input>;
    using output_pp_mode = mode<modes::output, modes::push_pull>;
    using output_od_mode = mode<modes::output, modes::open_drain>;
    using af_pp_mode = mode<modes::alternate, modes::push_pull>;
    using af_od_mode = mode<modes::alternate, modes::open_drain>;
    using af_input_mode = input_mode;
    using analog_mode = mode<modes::analog, modes::input>;
    using it_rising_mode = mode<modes::input, modes::ext_interrupt, modes::rising_edge>;
    using it_falling_mode = mode<modes::input, modes::ext_interrupt, modes::falling_edge>;
    using it_both_edges_mode = mode<modes::input, modes::ext_interrupt, modes::both_edges>;
    using evt_rising_mode = mode<modes::input, modes::ext_event, modes::rising_edge>;
    using evt_falling_mode = mode<modes::input, modes::ext_event, modes::falling_edge>;
    using evt_both_edges_mode = mode<modes::input, modes::ext_event, modes::both_edges>;

    //////////////////////////////////////////////////////////////////////////

    namespace pull
    {
        enum pull_mode
        {
            nopull = GPIO_NOPULL,
            up = GPIO_PULLUP,
            down = GPIO_PULLDOWN
        };
    }

    //////////////////////////////////////////////////////////////////////////

    namespace speed
    {
        enum speed_mode
        {
            low = GPIO_SPEED_FREQ_LOW,
            medium = GPIO_SPEED_FREQ_MEDIUM,
            high = GPIO_SPEED_FREQ_HIGH
        };
    }

    //////////////////////////////////////////////////////////////////////////

    template <typename _gpio,
              typename _pins,
              typename _mode,
              speed::speed_mode _speed,
              pull::pull_mode _pull>
    struct gpio_pin
    {
        static const auto pin = _pins::value;
        static const auto mode = _mode::value;
        static const auto speed = _speed;
        static const auto pull = _pull;
        using gpio = _gpio;

        inline static void init()
        {
            GPIO_InitTypeDef gpio_init;
            gpio_init.Mode = mode;
            gpio_init.Pin = pin;
            gpio_init.Speed = speed;
            gpio_init.Pull = pull;

            gpio::enable_clock();
            HAL_GPIO_Init(gpio::get(), &gpio_init);
        }

        inline static void deinit()
        {
            HAL_GPIO_DeInit(gpio::get(), pin);
        }

        inline static bool lock()
        {
            return HAL_GPIO_LockPin(gpio::get(), pin) == HAL_OK;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    template <typename _pin>
    struct input_pin
    {
        using pin = _pin;

        inline static void init()
        {
            static_assert(pin::mode == GPIO_MODE_INPUT,
                    "pin must be configured as input");

            pin::init();
        }

        inline static bool read()
        {
            return HAL_GPIO_ReadPin(pin::gpio::get(), pin::pin) == GPIO_PIN_SET;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    template <typename _pin>
    struct output_pin
    {
        using pin = _pin;

        inline static void init()
        {
            static_assert(pin::mode == GPIO_MODE_OUTPUT_PP || pin::mode == GPIO_MODE_OUTPUT_OD,
                    "pin must be configured as output");

            pin::init();
        }

        inline static void set()
        {
            HAL_GPIO_WritePin(pin::gpio::get(), pin::pin, GPIO_PIN_SET);
        }

        inline static void reset()
        {
            HAL_GPIO_WritePin(pin::gpio::get(), pin::pin, GPIO_PIN_RESET);
        }

        inline static void toggle()
        {
            HAL_GPIO_TogglePin(pin::gpio::get(), pin::pin);
        }
    };
}

#else
    #error gpio library needs at least C++11 compiler support
#endif // __cplusplus

#endif /* GPIO_HPP_ */
