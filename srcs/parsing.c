



#include "ft_ping.h"

void    parsingOption(int argc, char **argv, int *i, t_ping *ping) {
    int j = 0;
    char finish = 0;
    double value;

    while (!finish && argv[*i][++j]) {
        char        c = argv[*i][j];

        if (c == 'c') {
            if (argv[*i][j + 1]) {
                printf("ping: bad number of packets to transmit.\n");
                exit(EXIT_FAILURE);
            } else {
                *i += 1;
                finish = 1;
                if (*i >= argc)
                {
                    printHelp();
                }
                if (!ft_atof_strict(argv[*i], &value) || value < 1) {
                    printf("ping: bad number of packets to transmit.\n");
                    printHelp();
                } else {
                    ping->options.c.actived = 1;
                    ping->options.c.value = (int)value;
                }
            }
        }
        else if (c == 'D')
        {
            ping->options.d = 1;
        }
        else if (c == 'f')
        {
            ping->options.f = 1;
            if (ping->options.i.actived == 0)
            {
                ping->options.i.actived = 1;
                ping->options.i.value = 0;
            }
        }
        else if (c == 'i')
        {
            if (argv[*i][j + 1]) {
                // free all
                printf("ping: bad timing interval.\n");
                exit(EXIT_FAILURE);
            } else {

                *i += 1;
                finish = 1;
                if (*i >= argc)
                {
                    // free all
                    printHelp();
                }
                if (!ft_atof_strict(argv[*i], &value) || value < 0) {
                    // free all
                    printf("ping: bad timing interval.\n");
                    printHelp();
                } else {
                    ping->options.i.actived = 1;
                    ping->options.i.value = value;
                }
            }
        }
        else if (c == 'h') {
            // free all
            printHelp();
        } else if (c == 'v') {
            ping->options.v = 1;
        }
        else if (c == 'w')
        {
            if (argv[*i][j + 1]) {
                // free all
                printf("ping: bad wait time.\n");
                exit(EXIT_FAILURE);
            } else {
                int value;

                *i += 1;
                finish = 1;
                if (*i >= argc)
                {
                    // free all
                    printHelp();
                }
                if (!ft_atoi_strict(argv[*i], &value)
                    || (value > -1 && value < 1))
                {
                    ping->options.w.actived = 0;
                }
                else if (value <= -1)
                {
                    // free all
                    printf("ping: bad wait time.\n");
                    printHelp();
                }
                else {
                    ping->options.w.actived = 1;
                    ping->options.w.value = value;
                }
            }
        }
        else {
            // free all
            printf("ping : option invalide -- '%c'\n", c);
            printHelp();
        }
    }
}

/*
int     checkIP(char *ip) {
    char    **arrayIP = ft_strsplit(ip, '.');
    int     i = -1;
    int     arrayLen = ft_arraystrlen(arrayIP);
    int     octet;

    while (++i < arrayLen) {
        octet = -1;
        if (!ft_atoi_strict(arrayIP[i], &octet) || octet < 0) {
            freeDoubleChar(arrayIP);
            return 0;
        }
    }
    freeDoubleChar(arrayIP);
    return 1;
}
*/

void    parsing(int argc, char **argv, t_ping *ping) {
    int i = 0;

    while (++i < argc) {
        if (argv[i][0] == '-') {
            parsingOption(argc, argv, &i, ping);
        }
        else {
            ping->dst = argv[i];
        }
    }
    if (ping->dst == NULL)
        printHelp();
}