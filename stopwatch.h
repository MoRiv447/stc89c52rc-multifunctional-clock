#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

extern bit stopwatch_running;
extern unsigned int stopwatch_time;
extern unsigned int stopwatch_ms;

void stopwatchshow(void);
void stopwatch_timer_handler(void);

#endif