

#include "ft_ping.h"

void    freeDoubleChar(char **ptr) {
    int i = -1;

    if (ptr) {
        while(ptr[++i]) {
            free(ptr[i]);
        }
        free(ptr);
    }
}

void freePing(t_ping *ping) {
    if (ping) {}
}