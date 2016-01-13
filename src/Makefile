CC = gcc
CFLAGS = -I.
CFLAGS += $(shell pkg-config glfw3 --cflags)
LDFLAGS = -L/usr/X11R6/lib64
LIBS = -lz -lglut -lGLU -lGL -lGLEW
LIBS += $(shell pkg-config glfw3 --static --libs)

particles: main.o vector.o imageLoader.o scene.o io.o logic.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
