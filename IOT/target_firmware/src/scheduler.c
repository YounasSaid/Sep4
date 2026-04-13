#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "scheduler.h"

static task_t* task_list;
static uint8_t task_count;
static uint8_t _task_index = 0;

void scheduler_init(task_t tasks[], uint8_t count)
{
    task_list = tasks;
    task_count = count;
    for(uint8_t i=0; i<task_count; i++)
    {
        task_list[i].ticks = task_list[i].period/MS_PER_TICK;
    }
    uint16_t frequency = 1000/MS_PER_TICK; // Hz
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10); //Prescaler: F_CPU/64, CTC mode
    OCR1A  = F_CPU/(2*64*frequency);
    TIMSK1 = (1<<OCIE1A);  // Enable Timer Compare match interrupt
}

void dispatcher()
{
    // Execute tasks in the order given by task_list
    for(uint8_t i=0; i<task_count; i++)
    {
      if(task_list[i].ticks == 0)
      {
        task_list[i].task_p();
        task_list[i].ticks = task_list[i].period/MS_PER_TICK;
      }
    }
}

ISR(TIMER1_COMPA_vect)
{
    for(uint8_t i=0; i<task_count; i++)
    {
        if(task_list[i].ticks > 0)
        {
            task_list[i].ticks--;
        }
    }
}
