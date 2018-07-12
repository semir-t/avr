/*! \file delay.h 
 *  \brief If systimer is used than delay_ms and delay_us can't be
 *  used. Systimer is used for controlling multiple 
 *  time-critical process. Before using, systimer must be
 *  initialized. If systimer is used, then delay_ms & delay_us
 *  can't be used.
 *
 *  Systimer and delay functions are implemented with TIM2(8bit)
 *
 *  Stopwatch is implemented with TIM1 (16 bit).*  
 *  Detailed description of the function
 * 
 * \author Semir Tursunovic (semir-t)
 */

#ifndef _DELAY_H
#define _DELAY_H 

#include <avr/io.h>
#include <avr/interrupt.h>

#define SYSTIMER_TIMEOUT 0
#define SYSTIMER_KEEPALIVE 1

/*! \brief Initialize systimer
 *
 * \return No return value
 */
void systimer_init(void);

/*! \brief ISR for handling interrupt which occur on TIM2
 *  compare value match
 *
 * \param  TIMER2_COMPA_vect corresponding interrupt vector 
 */
ISR(TIMER2_COMPA_vect);

/*! \brief Get current value of the systimer
 *
 * 
 * \return Current value of the systimer 
 */
uint32_t systimer_get(void);

/*! \brief SYSTIMER timout flag 
 *
 * \return SYSTIMER_TIMEOUT || SYSTIMER_KEEP_ALIVE
 */
uint8_t systimer_timeout_flag(uint32_t start_time, uint32_t period);

/*! \brief Pause in the code execution. Resolution in milliseconds
 *
 * This function is based on TIM2.
 *
 * \param  delay Waiting time
 * \return No return value
 */
void delay_ms(uint32_t);

/*! \brief Pause in the code execution. Resolution in microseconds
 *
 * This function is based on TIM2.
 *
 * \param  delay Waiting time in us
 * \return No return value
 */
void delay_us(uint32_t);


/*! \brief Initialize stopwatch
 * 
 * Can be used for measuring time which is needed for code to be
 * executed.
 *
 * \return No return value
 */
void stopwatch_init(void);

/*! \brief Start stopwatch 
 * 
 * \return No return value
 */
void stopwatch_start(void);

/*! \brief Stop stopwatch 
 * 
 * \return Time passed since stopwatch started (in ms) 
 */
uint16_t stopwatch_stop(void);




#endif /* ifndef _DELAY_H */
