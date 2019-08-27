VERSION=1.1b
CC=cl
CFLAGS=-DPROGVER=$(VERSION) -MT -O2 -W3
LDFLAGS=
LDLIBS=user32.lib
RM=del /q

all: main

OBJS=main.obj

email-and-password.exe: $(OBJS)
	$(CC) -Feemail-and-password $(OBJS) -link $(LDFLAGS) $(LDLIBS)

main: email-and-password.exe

.c.obj:
	$(CC) $(CFLAGS) -c -Fo$*.obj $<

clean:
	$(RM) "*.exe"
	$(RM) "*.obj"
