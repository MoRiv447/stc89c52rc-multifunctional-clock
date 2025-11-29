#ifndef __COUNTDOWN_H__
#define __COUNTDOWN_H__

extern bit countdown_running;
extern unsigned int countdown_time;
extern unsigned int countdown_ms;
extern bit countdown_finished;

void countdownshow(void);
void countdown_timer_handler(void);

#endif