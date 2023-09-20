#ifndef __TIMER_H__
#define __TIMER_H__
#include "types.h"


#define   TMR_RUNNING     0       /*!< Timer State: Running             */
#define   TMR_STOPPED     1       /*!< Timer State: Stopped             */
#define   TMR_ONE_SHOT    0       /* Timer Type: one shot */       
#define   TMR_PERIOD      1       /* Timer Type: period */

typedef struct timerCB                  /* Timer Control Block Define.        */
{
    list_t            node;
    uint16_t          timerID;             /*!< Timer ID.                        */
    uint8_t           timerType;           /*!< Timer Type.                      */
    uint8_t           timerState;          /*!< Timer State.                     */
    uint32_t          timerCnt;            /*!< Timer Counter.                   */
    uint32_t          timerReload;         /*!< Timer Reload Counter Value.      */	
    void (*timerCallBack)(void); /*!< Call-back Function When Timer overrun. */	
}timerCB_t;


#endif /* __TIMER_H__ */