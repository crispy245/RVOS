#include "os.h"

MutCB_t      MUTTbl[MAX_MUT];
static uint32_t     MutMap[MAX_MUT/MAP_SIZE];


err_t createMut()
{
    reg_t lock_status;
    lock_status = baseLock();
    for (int i=0; i<MAX_MUT;i++) {
        int mapIndex = i / MAP_SIZE;
        int mapOffset = i % MAP_SIZE;
        if ((MutMap[mapIndex] & (1<< mapOffset)) == 0)
        {
            MutMap[mapIndex] |= (1<< mapOffset);
            MUTTbl[i].id = i;
            list_init((list_t*)&MUTTbl[i].node);
            baseUnLock(lock_status);
            return i;
        }
    }
    baseUnLock(lock_status);
    return E_CREATE_FAIL;
}

 
void delMut(uint16_t mutID)
{   
    /* free mut control block */
    int mapIndex = mutID / MAP_SIZE;
    int mapOffset = mutID % MAP_SIZE;
    reg_t lock_status = baseLock();
    MutMap[mapIndex] &=~(1<<mapOffset);   
    /* wakeup the suspended thread */
    MutCB_t *pmutcb = &MUTTbl[mutID];
    if (AllWaitTaskToRdy((list_t*)pmutcb))//return 1: task_yield
    {
        baseUnLock(lock_status);  
        task_yield();                             
    }else
        baseUnLock(lock_status);  
}


err_t mut_take(uint16_t mutID, int timeout){
    MutCB_t *pmutcb = &MUTTbl[mutID];
    taskCB_t *ptcb;
    reg_t lock_status=baseLock();

    if (pmutcb->mutCounter > 0) {
        /* mut is available */
        pmutcb->mutCounter --;
        baseUnLock(lock_status);
    } else {
        /* no waiting, return with timeout */
        if (timeout == 0) {
            baseUnLock(lock_status);
            return E_TIMEOUT;
        }
        //return OK
        ptcb = getCurrentTask();
        ptcb->returnMsg = E_OK;
        TaskToWait((list_t*)pmutcb, FIFO, ptcb);
        /* has waiting time, start thread timer */      
        if (timeout > 0) {
            //waiting in delayList
            setCurTimerCnt(ptcb->timer->timerID,timeout, timeout);
            startTimer(ptcb->timer->timerID);
        }  
        baseUnLock(lock_status);
        task_yield();

        if (ptcb->returnMsg != E_OK) { 
        } else { 
            stopTimer(ptcb->timer->timerID); //remove timer from delayList
            return E_OK;
        }
    }
    return E_OK;
}

err_t mut_trytake(uint16_t mutID) {
    return mut_take(mutID, 0);
}


err_t mut_release(uint16_t mutID)
{
    MutCB_t *psemcb=&MUTTbl[mutID];
    reg_t  need_schedule_sem;
    
    need_schedule_sem = 0;
    reg_t lock_status = baseLock();

    if (!list_isempty((list_t*)psemcb)) {
        WaitTaskToRdy((list_t*)psemcb);
        need_schedule_sem = 1;
    }
    baseUnLock(lock_status);
    if (need_schedule_sem) 
        task_yield();

    return E_OK;
    
}

