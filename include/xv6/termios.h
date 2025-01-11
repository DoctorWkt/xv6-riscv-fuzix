// Borrowed from FUZIX

#define ECHO            0x0001     /* enable echoing */
#define ICANON          0x0010     /* canonicalize input lines */

#define NCCS            12		/* Size of the buffer below */

#define TIOCGETA	1		/* Arbitrary ioctl() numbers */
#define TIOCSETA	2

typedef uint16 tcflag_t;
typedef uint16 speed_t;
typedef uint8  cc_t;

struct termios {
        tcflag_t        c_iflag;        /* input flags */
        tcflag_t        c_oflag;        /* output flags */
        tcflag_t        c_cflag;        /* control flags */
        tcflag_t        c_lflag;        /* local flags */
        cc_t            c_cc[NCCS];     /* control chars */
};
