#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "scheduler.h"
#include <stdio.h>

static volatile task_t* task_list;
static volatile uint8_t task_count;
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
    //Prescaler: F_CPU/1024, CTC mode
    TCCR2A = (1<<WGM21);
    TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);
    OCR2A  = F_CPU/(1024*frequency) - 1;
    TIMSK2 = (1<<OCIE2A);  // Enable Timer Compare match interrupt
}

void dispatcher()
{
    // Execute tasks in the order given by task_list
    for(uint8_t i=0; i<task_count; i++)
    {
      if(task_list[i].ticks <= 0)
      {
        task_list[i].task_p();
        task_list[i].ticks = task_list[i].period/MS_PER_TICK;
      }
    }
}

ISR(TIMER2_COMPA_vect)
{
    PORTB ^= (1<<5);
    for(uint8_t i=0; i<task_count; i++)
    {
        if(task_list[i].ticks > 0)
        {
            task_list[i].ticks--;
        }
    }
}