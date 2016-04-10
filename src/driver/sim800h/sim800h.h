#ifndef UBIRCH_SIM800_H
#define UBIRCH_SIM800_H

#include <board.h>

size_t sim800h_readline(char *buffer, size_t max);
void sim800h_power_enable();
void sim800h_power_disable();
void sim800h_enable();


#endif
