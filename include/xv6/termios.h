// Borrowed from NetBSD

#define ECHO            0x0008     /* enable echoing */
#define ICANON          0x0100     /* canonicalize input lines */

#define NCCS            20		/* Size of the buffer below */

#define TIOCGETA	1		/* Arbitrary ioctl() numbers */
#define TIOCSETA	2

typedef unsigned int    tcflag_t;
typedef unsigned char   cc_t;
typedef unsigned int    speed_t;

struct termios {
        tcflag_t        c_iflag;        /* input flags */
        tcflag_t        c_oflag;        /* output flags */
        tcflag_t        c_cflag;        /* control flags */
        tcflag_t        c_lflag;        /* local flags */
        cc_t            c_cc[NCCS];     /* control chars */
        int             c_ispeed;       /* input speed */
        int             c_ospeed;       /* output speed */
};
