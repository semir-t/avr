/*! \file avr_core.h
 *  \brief AVR custom macros
 *  
 *  
 * 
 * \author Semir Tursunovic (semir-t)
 */
#ifndef _AVR_CORE_H
#define _AVR_CORE_H

#define BIT_(bit)               (bit)
#define BIT_MASK_(bit)          (1 << bit)
#define PORT_(port)              (PORT ## port)
#define PIN_(port)               (PIN ## port)
#define DDR_(port)               (DDR ## port)
     
#define BIT(port,bit)               BIT_(bit) 
#define BIT_MASK(port,bit)          BIT_MASK_(bit)
#define PORT(port,bit)              PORT_(port)
#define PIN(port,bit)               PIN_(port)
#define DDR(port,bit)               DDR_(port)

#define GPIO_MODE_INPUT(io)        (DDR(io) &= ~BIT_MASK(io))
#define GPIO_MODE_OUTPUT(io)       (DDR(io) |= BIT_MASK(io)) 
#define GPIO_SET(io)               (PORT(io) |= BIT_MASK(io)) 
#define GPIO_CLEAR(io)             (PORT(io) &= ~BIT_MASK(io))
#define GPIO_TOGGLE(io)            (PORT(io) ^= ~BIT_MASK(io))
#define GPIO_READ(io)              ((PIN(io) >> BIT(io) ) & 0x01)


#endif /* end of include guard: _AVR_CORE_H */

