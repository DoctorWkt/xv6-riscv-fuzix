CC = riscv64-linux-gnu-gcc
AR = riscv64-linux-gnu-ar
#CC_OPT = -march=rv32ima -mabi=ilp32 -static -static-libgcc -mstrict-align
CC_OPT = -static -static-libgcc -mstrict-align
CC_OPT += -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin
CC_OPT += -fno-pie -Wall -g -Os -c -I../../include
CC_OPT += -DPREFER_STACK
SRC_C =  __argv.c abort.c accept.c aes256.c asctime.c asctime_r.c assert.c
SRC_C += atexit.c basename.c bcmp.c bcopy.c bind.c bsearch.c bzero.c
SRC_C += calloc.c cfree.c clock.c
SRC_C += clock_gettime.c clock_getres.c clock_settime.c
SRC_C += connect.c creat.c crypt.c ctermid.c ctime.c cuserid.c
SRC_C += dirname.c
SRC_C += err.c error.c
SRC_C += exit.c
SRC_C += fclose.c fdatasync.c fgetc.c fgetgrent.c fgetpwent.c
SRC_C += fgetpos.c fgets.c fopen.c fprintf.c fputc.c fputs.c fread.c
SRC_C += fsetpos.c fsync.c ftell.c ftruncate.c fwrite.c getcwd.c
SRC_C += getenv.c __getgrent.c getgrgid.c getgrnam.c getloadavg.c getlogin.c
SRC_C += getopt.c
SRC_C += getpw.c __getpwent.c getpwnam.c getpwuid.c gets.c gettimeofday.c
SRC_C += getw.c gmtime.c gmtime_r.c grent.c
SRC_C += htons.c htonl.c
SRC_C += inet_addr.c inet_aton.c inet_network.c inet_ntoa.c inet_ntop.c
SRC_C += inet_pton.c index.c initgroups.c isatty.c killpg.c
SRC_C += libintl.c
SRC_C += listen.c localtim.c localtim_r.c lsearch.c ltoa.c
SRC_C += ltostr.c malloc.c mkfifo.c mkstemps.c mntent.c nanosleep.c
SRC_C += pause.c perror.c
SRC_C += popen.c printf.c putenv.c putchar.c putpwent.c putw.c pwent.c qsort.c
SRC_C += raise.c rand.c readlink.c realloc.c regerror.c
SRC_C += regsub.c remount.c remove.c rewind.c rindex.c rootmntent.c
SRC_C += setbuffer.c setenv.c
SRC_C += setlocale.c setvbuf.c settimeofday.c sgetl.c sigdisp.c
SRC_C += socket.c sprintf.c
SRC_C += sputl.c stat.c statvfs.c stdio0.c stime.c
SRC_C += strcasecmp.c strcasestr.c strdup.c stricmp.c strlcpy.c strncasecmp.c
SRC_C += strnlen.c strnicmp.c strsep.c strxfrm.c strcoll.c strsignal.c
SRC_C += strtod.c strtol.c swab.c swapon.c system.c
SRC_C += tmpfile.c tmpnam.c ttyname.c
SRC_C += tzset.c umount.c ungetc.c usleep.c utent.c utimes.c utsname.c
SRC_C += vfork.c vfprintf.c vprintf.c xitoa.c pathconf.c
SRC_C += gethostname.c sysconf.c confstr.c memccpy.c getpass.c
# ctype
SRC_C += isalnum.c isalpha.c isascii.c isblank.c iscntrl.c isdigit.c
SRC_C += isgraph.c islower.c isprint.c ispunct.c isspace.c isupper.c
SRC_C += isxdigit.c toupper.c tolower.c
# tty layer
SRC_C += tcgetattr.c tcsetattr.c tcdrain.c tcflow.c tcflush.c
SRC_C += cfmakeraw.c cfspeed.c revoke.c
# scanf
SRC_C += fscanf.c scanf.c sscanf.c vfscanf.c vscanf.c vsscanf.c
# Seems to give the compiler a hard time
SRC_C += regexp.c
# Pieces we inherit in other cases from the compiler library instead
SRC_C += strcpy.c strlen.c abs.c atof.c atoi.c atol.c labs.c
SRC_C += strcat.c strchr.c strcmp.c strcspn.c strncat.c strncmp.c
SRC_C += strncpy.c strpbrk.c strrchr.c strspn.c strstr.c strtok.c strtok_r.c
SRC_C += memchr.c memcmp.c memcpy.c memset.c memmove.c

SRC_CT += termcap.c tgetent.c

SRC_CURS = $(shell find curses -name '*.c')

SRC_RL = readline.c readline_fn.c

SRC_LM = vfscanf_m.c vfprintf_m.c
SRC_LM += acosf.c acoshf.c asinf.c asinhf.c atan2f.c atanf.c atanhf.c
SRC_LM += cbrtf.c ceilf.c copysignf.c erff.c expf.c expm1f.c
SRC_LM += fabsf.c fdimf.c floorf.c fmaxf.c fminf.c fmodf.c frexpf.c
SRC_LM += hypotf.c ilogbf.c j0f.c j1f.c jnf.c
SRC_LM += ldexpf.c lgammaf.c lgammaf_r.c logf.c log2f.c log10f.c logbf.c
SRC_LM += lrintf.c lroundf.c
SRC_LM += modff.c nearbyintf.c nextafterf.c powf.c
SRC_LM += remainderf.c remquof.c rintf.c roundf.c
SRC_LM += scalbnf.c scalbln.c
SRC_LM += sinf.c sincosf.c sinhf.c
SRC_LM += sqrtf.c tgammaf.c
SRC_LM += __expo2f.c __float_bits.c __fpclassifyf.c __log1pf.c __signgam.c
SRC_LM += mathhelper.c remainder.c
SRC_LM += acos.c acosh.c asin.c asinh.c atan2.c atan.c atanh.c
SRC_LM += cbrt.c ceil.c copysign.c erf.c exp.c expm1.c
SRC_LM += fabs.c fdim.c floor.c fmax.c fmin.c fmod.c frexp.c
SRC_LM += hypot.c j0.c j1.c jn.c
SRC_LM += ldexp.c lgamma.c lgamma_r.c log.c log2.c log10.c
SRC_LM += lrint.c lround.c
SRC_LM += modf.c nearbyint.c pow.c
SRC_LM += remquo.c rint.c round.c
SRC_LM += scalbn.c
SRC_LM += sin.c sincos.c sinh.c
SRC_LM += sqrt.c tgamma.c
SRC_LM += __expo2.c __double_bits.c __fpclassify.c __log1p.c
SRC_LM += __sin.c __cos.c __tan.c __sindf.c __cosdf.c __tandf.c

OBJ_C = $(SRC_C:.c=.o)
OBJ_CURS = $(SRC_CURS:.c=.o)
OBJ_CT = $(SRC_CT:.c=.o)
OBJ_RL = $(SRC_RL:.c=.o)
OBJ_LM = $(SRC_LM:.c=.o)
OBJ_HARD = $(SRC_HARD:.c=.o)
OBJ_ALL = $(OBJ_C) $(OBJ_LM) $(OBJ_HARD) $(OBJ_RL)

all: libc.a libcurses.a libreadline.a libtermcap.a libm.a

install: all
	cp libc.a libcurses.a libreadline.a libtermcap.a libm.a ..

#$(OBJ_ALL): $(KRN_HEADERS_COPY)

#$(KRN_HEADERS_COPY): $(KRN_HEADERS_SRC)
#	cp ../../Kernel/include/$(notdir $@) $@

libc.l:%.l:$(OBJ_ALL)
	ls $(OBJ_ALL) > libc.l

libc.a: libc.l
	$(AR) rc libc.a $$(cat libc.l)

libcurses.a: $(OBJ_CURS)
	$(AR) rc libcurses.a $(OBJ_CURS)

libtermcap.a: $(OBJ_CT)
	$(AR) rc libtermcap.a $(OBJ_CT)

libm.a: $(OBJ_LM)
	$(AR) rc libm.a $(OBJ_LM)

libreadline.a: $(OBJ_RL)
	$(AR) rc libreadline.a $(OBJ_RL)

$(OBJ_C):%.o: %.c
	$(CC) $(CC_OPT) $(@:.o=.c)

$(OBJ_CT):%.o: %.c
	$(CC) $(CC_OPT) $(@:.o=.c)

$(OBJ_CURS):%.o: %.c
	$(CC) $(CC_OPT) $(@:.o=.c) -o $@

$(OBJ_LM):%.o: %.c
	$(CC) $(CC_OPT) $(@:.o=.c)

$(OBJ_HARD):%.o: %.c
	$(CC) $(CC_NOOPT) $(@:.o=.c)

$(OBJ_RL):%.o: %.c
	$(CC) $(CC_OPT) $(@:.o=.c)

clean:
	rm -f *.o *.asm *.sym *.lst *.a *~ *.l \
	libc.a libm.a libtermcap.a \
	libcurses.a libreadline.a
	-cd curses && rm -rf *.o *.asm *.sym *.lst *~
