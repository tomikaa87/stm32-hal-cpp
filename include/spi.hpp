//
// C++ Wrapper for STM32 HAL SPI routines
//
// Created on 2018-09-16 by Tamas Karpati <tomikaa87@gmail.com>
//

#ifndef SPI_HPP_
#define SPI_HPP_

// Minimum necessary version is C++11
#if __cplusplus >= 201103l

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

#include "utils.hpp"

namespace spi
{
    namespace peripheral
    {
        #define PERIPH_STRUCT(_NUM)                     \
        struct spi##_NUM                                \
        {                                               \
            static inline SPI_TypeDef* base()           \
            {                                           \
                return SPI##_NUM;                       \
            }                                           \
                                                        \
            static inline void enable_clock()           \
            {                                           \
                __HAL_RCC_SPI##_NUM##_CLK_ENABLE();     \
            }                                           \
                                                        \
            static inline void disable_clock()          \
            {                                           \
                __HAL_RCC_SPI##_NUM##_CLK_DISABLE();    \
            }                                           \
        };                                              \

#ifdef SPI1
        PERIPH_STRUCT(1)
#endif

#ifdef SPI2
        PERIPH_STRUCT(2)
#endif

#ifdef SPI3
        PERIPH_STRUCT(3)
#endif

#ifdef SPI4
        PERIPH_STRUCT(4)
#endif
    }

    namespace mode
    {
        enum mode
        {
            master = SPI_MODE_MASTER,
            slave = SPI_MODE_SLAVE
        };
    }

    namespace data_size
    {
        enum data_size
        {
            eight_bits = SPI_DATASIZE_8BIT,
            sixteen_bits = SPI_DATASIZE_16BIT
        };
    }

    namespace direction
    {
        enum direction
        {
            one_line = SPI_DIRECTION_1LINE,
            two_lines = SPI_DIRECTION_2LINES,
            two_lines_rx_only = SPI_DIRECTION_2LINES_RXONLY
        };
    }

    namespace first_bit
    {
        enum first_bit
        {
            lsb = SPI_FIRSTBIT_LSB,
            msb = SPI_FIRSTBIT_MSB
        };
    }

    namespace nss
    {
        enum nss
        {
            hard_input = SPI_NSS_HARD_INPUT,
            hard_output = SPI_NSS_HARD_OUTPUT,
            soft = SPI_NSS_SOFT
        };
    }

    namespace frame_format
    {
        enum frame_format
        {
            motorola = SPI_TIMODE_DISABLE,
            ti
        };
    }

    namespace clock_phase
    {
        enum clock_phase
        {
            one_edge = SPI_PHASE_1EDGE,
            two_edge = SPI_PHASE_2EDGE
        };
    }

    namespace clock_polarity
    {
        enum clock_polarity
        {
            low = SPI_POLARITY_LOW,
            high = SPI_POLARITY_HIGH
        };
    }

    template <typename _peripheral,
              mode::mode _mode = mode::master,
              uint16_t _baud_prescaler = 16,
              frame_format::frame_format _frame_format = frame_format::motorola,
              first_bit::first_bit _first_bit = first_bit::msb,
              data_size::data_size _data_size = data_size::eight_bits,
              direction::direction _direction = direction::two_lines,
              nss::nss _nss = nss::soft,
              clock_phase::clock_phase _clock_phase = clock_phase::one_edge,
              clock_polarity::clock_polarity _clock_polarity = clock_polarity::low,
              bool _crc_calc_enabled = false,
              uint32_t _crc_polynomial = 0>
    struct device
    {
        using peripheral = _peripheral;
        static const auto mode = _mode;
        static const auto data_size = _data_size;
        static const auto direction = _direction;
        static const auto first_bit = _first_bit;
        static const uint16_t baud_prescaler = _baud_prescaler;
        static const auto nss = _nss;
        static const auto frame_format = _frame_format;
        static const auto clock_phase = _clock_phase;
        static const auto clock_polarity = _clock_polarity;
        static const bool crc_calc_enabled = _crc_calc_enabled;
        static const auto crc_polynomial =_crc_polynomial;

        inline static SPI_HandleTypeDef* handle()
        {
            static SPI_HandleTypeDef s_handle;
            return &s_handle;
        }

        inline static bool init()
        {
            static_assert(utils::bit_weight<baud_prescaler>::value == 1,
                    "baud rate prescaler value must be a power of 2");

            static_assert(baud_prescaler <= 256,
                    "baud rate prescaler value must be <= 256");

            handle()->Instance = peripheral::base();
            handle()->Init.Mode = mode;
            handle()->Init.DataSize = data_size;
            handle()->Init.Direction = direction;
            handle()->Init.FirstBit = first_bit;
            handle()->Init.BaudRatePrescaler = baud_prescaler;
            handle()->Init.NSS = nss;
            handle()->Init.TIMode = frame_format;
            handle()->Init.CLKPhase = clock_phase;
            handle()->Init.CLKPolarity = clock_polarity;
            handle()->Init.CRCCalculation = crc_calc_enabled ? SPI_CRCCALCULATION_ENABLE : SPI_CRCCALCULATION_DISABLE;
            handle()->Init.CRCPolynomial = crc_polynomial;

            peripheral::enable_clock();

            return HAL_SPI_Init(handle()) == HAL_OK;
        }

        inline static bool reinit()
        {
            return HAL_SPI_Init(handle()) == HAL_OK;
        }

        inline static HAL_StatusTypeDef transmit(uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY)
        {
            return HAL_SPI_Transmit(handle(), data, size, timeout);
        }

        inline static HAL_StatusTypeDef receive(uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY)
        {
            return HAL_SPI_Receive(handle(), data, size, timeout);
        }

        inline static HAL_StatusTypeDef transfer(uint8_t* txdata, uint8_t* rxdata, uint16_t size, uint32_t timeout = HAL_MAX_DELAY)
        {
            return HAL_SPI_TransmitReceive(handle(), txdata, rxdata, size, timeout);
        }
    };
}

#else
    #error utils library needs at least C++11 compiler support
#endif // __cplusplus

#endif /* SPI_HPP_ */
