CC = gcc
CFLAGS = -I.
CFLAGS += $(shell pkg-config glfw3 --cflags)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  LDFLAGS = -L/opt/X11/lib/
else
  LDFLAGS = -L/usr/X11R6/lib64
endif
LIBS = -lz -lGLU -lGL -lGLEW
LIBS += $(shell pkg-config glfw3 --static --libs)

particles: main.o vector.o imageLoader.o scene.o io.o logic.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
