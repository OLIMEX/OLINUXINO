/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 * Author: Federico Reghenzani <federico[DOT]dev[AT]reghe[DOT]net>
 */

#ifndef A20_GPIO_HPP
#define A20_GPIO_HPP

#include <vector>
#include <stdexcept>
#include <string>

#if __cplusplus > 199711L
    #include <cstdint>
#else
    #ifndef uint8_t
        typedef unsigned char uint8_t;
    #endif
    #ifndef uint16_t
        typedef unsigned short uint16_t;
    #endif
#endif

/*!
 * The namespace for A20
 */
namespace A20 {

    /*!
     * \brief The enumeration for pullup/pulldown resistors. The values match
     *        sunxi values for pull resistors.
     */
    typedef enum {
        NONE      =0,
        PULL_UP   =1,
        PULL_DOWN =2
    } pull_resistor_t;

    class GPIO;
    
    /*!
     * \brief A generic exception for all module.
     */
    class GPIO_exception : public std::runtime_error {
        public:
            GPIO_exception(std::string s) : runtime_error(std::string("GPIO exception: ")+s) {};
    };

    /*!
     * \brief An abstract class for GPIO_input and GPIO_output. Probably, you 
     *        don't have to use this (internal library use).
     */
    class GPIO_common {
        public:
            /*!
             *  \brief It returns the type of GPIO (input, output or periphery).  Probably, you 
             *        don't have to use this (internal library use).
             *  \return 0 if input, 1 if output, 2 if periphery 
             */
            virtual uint8_t  get_type() const = 0;    // 0 - input, 1 - output, 2 - per
            
            /*!
             *  \brief It returns the current port number as requested in costructor
             *  \return the port number according to linux enumeration (see datasheet) 
             */
            virtual uint16_t get_port() const = 0;
    };
    
    /*!
     * \brief The class that represents a GPIO input. You cannot instantiate this class
     *        directly, but you need to call GPIO::get_input to have a pointer of this 
     *        class. Also, you cannot free the pointer, but you need to use GPIO::free.
     */
    class GPIO_input : GPIO_common {
        friend class GPIO;
        public:
            uint8_t  get_type() const { return 0; }
            uint16_t get_port() const { return port; }
            
            /*!
             *  \brief It returns the current value of the GPIO.
             *  \return true if it reads a logical '1' on the GPIO, false otherwise.
             */
            bool get() const;
        private:
            GPIO_input(uint16_t port, pull_resistor_t pull_resistor);
            ~GPIO_input() {};
            uint16_t port;
    };
    
    /*!
     * \brief The class that represents a GPIO output. You cannot instantiate this class
     *        directly, but you need to call GPIO::get_output to have a pointer of an 
     *        object of this type. Also, you cannot free the pointer, but you need 
     *        to use GPIO::free.
     */
    class GPIO_output : GPIO_common {
        friend class GPIO;
        public:
            uint8_t  get_type() const { return 1; }
            uint16_t get_port() const { return port; }
            
            /*!
             *  \brief Set the logical output of a GPIO.
             *  \param value if true it puts logical '1' on the GPIO, '0' otherwise.
             */
            void set(bool) const;
        private:
            GPIO_output(uint16_t port);
            ~GPIO_output();
            uint16_t port;
    };
    
    /*!
     * \brief The class that represents a GPIO periphery. You cannot instantiate this class
     *        directly, but you need to call GPIO::set_periphery_mode to have a pointer of an 
     *        object of this type. Also, you cannot free the pointer, but you need 
     *        to use GPIO::free.
     *        Currently, you cannot do anything with this type of GPIO.
     */
    class GPIO_periphery : GPIO_common {
        friend class GPIO;
        public:
            uint8_t get_type() const { return 2; }
            uint16_t get_port() const { return port; }
        private:
            GPIO_periphery(uint16_t port);
            ~GPIO_periphery();
            uint16_t port;
    };

    /*!
     * \brief The main class of the module. First of all, you need to call the
     *        init() method, to initializate the sunxi driver. After that you
     *        can obtain GPIO via get_input, get_output and set_periphery_mode.
     *
     * \note  This is a static class, you cannot (and you don't need to)
     *        initializate an object of this type. 
     *
     */
    class GPIO {
        public:
            /*!
             * \brief  Initialize the submodule sunxi. You have to call this 
             *         before any other method.
             * \exception GPIO_exception In case of initialization error.
             */
            static void        init() throw(GPIO_exception);

            /*!
             * \brief  Set the GPIO indicated as input, enable the eventually
             *         specified pull resistor, and returns the corresponding
             *         object to read data. If you want to change the type of
             *         GPIO, you must free the previous object via GPIO::free.
             * \param  port  the number of GPIO to set as input according to 
             *               kernel enumeration (see datasheet)
             * \param  pull_resistor specify the type of pull resistor to 
             *                       enable in this GPIO. Refer to 
             *                       pull_resistor_t for available values. By
             *                       default no pull resistor is enabled.
             * \exception GPIO_exception In case of configuration error or if
             *                           you are trying to set as input a non
             *                           freed GPIO (for example because you 
             *                           create a GPIO_output on the same GPIO).
             * \note Before you can read from this gpio, you need to wait some
             *       microseconds. We already put in this method a sleep of
             *       100us. If you don't want this behaviour, define a
             *       costant named GPIO_NO_WAIT.
             */
            static GPIO_input  *get_input(uint16_t port, pull_resistor_t pull_resistor=NONE) throw(GPIO_exception);
            
            /*!
             * \brief  Set the GPIO indicated as output and returns the corresponding
             *         object to write data. If you want to change the type of
             *         GPIO, you must free the previous object via GPIO::free.
             * \param  port  the number of GPIO to set as output according to 
             *               kernel enumeration (see datasheet)

             * \exception GPIO_exception In case of configuration error or if
             *                           you are trying to set as output a non
             *                           freed GPIO (for example because you 
             *                           create a GPIO_input on the same GPIO).
             * \note Before you can write to  this gpio, you need to wait some
             *       microseconds. We already put in this method a sleep of
             *       100us. If you don't want this behaviour, define a
             *       costant named GPIO_NO_WAIT.
             */
            static GPIO_output *get_output(uint16_t port) throw(GPIO_exception);
            
            /*!
             * \brief  Set the GPIO indicated as periphery and returns the corresponding
             *         object. If you want to change the type of
             *         GPIO, you must free the previous object via GPIO::free.
             * \param  port  the number of GPIO to set as periphery according to 
             *               kernel enumeration (see datasheet)

             * \exception GPIO_exception In case of configuration error or if
             *                           you are trying to set as periphery a non
             *                           freed GPIO (for example because you 
             *                           create a GPIO_input on the same GPIO).
             * \note Before you can use this gpio, you need to wait some
             *       microseconds. We already put in this method a sleep of
             *       100us. If you don't want this behaviour, define a
             *       costant named GPIO_NO_WAIT.
             */
            static void        set_periphery_mode(uint16_t port) throw(GPIO_exception);
            
            /*!
             * \brief  Free the corresponding object. 
             * \param  port  the number of GPIO to free according to 
             *               kernel enumeration (see datasheet)
             * \note If the object you ask to free is an output GPIO, this
             *       method transform the output in input for precaution. If
             *       you don't want this behaviour, define a constant named 
             *       GPIO_NO_SAFE_RESET_TO_INPUT
             * \exception GPIO_exception In case of generic error or if
             *                           you are trying to free a non setted 
             *                           GPIO.
             */
            static void        free(uint16_t port) throw(GPIO_exception);
        private:
            GPIO();        // You cannot instantiate this object
    
            static std::vector<GPIO_common*> registered;
    
    };



}

#endif