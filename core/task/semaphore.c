#include "semaphore.h"
#include "mm.h"
#include "task.h"

semaphore *sem_create() 
{
    semaphore *sem = (semaphore*)kmalloc(sizeof(semaphore));
    INIT_LIST_HEAD(&sem->wait_list);
    sem->count = 0;
    return sem;
}

void sem_down(semaphore *sem)
{
    if(!sem->count) 
    {
        sem->count++;
    } 
    else 
    {
        //this sema has been locked,
        //sechedule my sele
        task_struct *current = GET_CURRENT_TASK();
        list_add(&current->lock_ll,&sem->wait_list);
        yield_current();
    }
}

void sem_up(semaphore *sem)
{
    sem->count--;
    if(!list_empty(&sem->wait_list))
    {
        struct list_head *p = sem->wait_list.next;
        if(p->next != NULL && p->next != &sem->wait_list)
        {
            task_struct *task = list_entry(p,task_struct,lock_ll);
            list_del(p);
            wake_up_task(task);
        }
    }
}

void sem_up_all(semaphore *sem)
{
    sem->count = 0;
    if(!sem->count) 
    {
        //we should start to notify all the thread.
        if(!list_empty(&sem->wait_list))
        {
            struct list_head *p = sem->wait_list.next;
            while(p != NULL && p != &sem->wait_list)
            {
                task_struct *task = list_entry(p,task_struct,lock_ll);
                list_del(p);
                wake_up_task(task);
                p = p->next;
            }
        }
    }
}

void sem_destroy(semaphore *sem)
{
    if(!list_empty(&sem->wait_list))
    {
        kprintf("sem free while other thread hold sem \n");
    }

    free(sem);
}
