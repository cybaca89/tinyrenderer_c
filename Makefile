CC           = gcc
GDB          = gdb
CPPFLAGS     = -std=gnu99 -fomit-frame-pointer -c
#CPPFLAGS     = -std=gnu99 -g -gp
LIBS         = -lm -lxcb
WARNINGS     = -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code \
-Wundef -Wcast-align
LDFLAGS      = -Wl,--build-id=sha1
CFLAGS       = -I src/display/include -I src/math/include -I src/utils/include \
-I src/model/include

TARGET  = aa

OBJECTS = ./obj/main.o \
./obj/xdraw.o \
./obj/parser.o \
./obj/vec3.o \
./obj/model.o \
./obj/timer.o \
./obj/transform.o \
./obj/input.o \
./obj/display.o \
./obj/drm_write.o \
./obj/drm_util.o \
./obj/fb_write.o \
./obj/fb_util.o
#./obj/vec3array.o

all: ./$(TARGET)

$(TARGET):$(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

./obj/main.o:src/main.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)

./obj/xdraw.o:src/xdraw.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)

#./obj/vec3array.o:src/math/vec3/vec3array.c
#	$(CC)  $(CPPFLAGS) $(WARNINGS) -c $(CFLAGS) $< -o $@

./obj/vec3.o:src/math/vec3/vec3.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@


./obj/model.o: src/model/model.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)

./obj/transform.o: src/model/transform.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)

./obj/parser.o: src/model/parser.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)


./obj/timer.o: src/utils/timer.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)


./obj/display.o: src/display/xcb/display.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@


./obj/input.o: src/display/input/input.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@

./obj/drm_write.o: src/display/drm/drm_write.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@

./obj/drm_util.o: src/display/drm/drm_util.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@

./obj/fb_write.o: src/display/framebuf/fb_write.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@ $(CFLAGS)

./obj/fb_util.o: src/display/framebuf/fb_util.c
	$(CC) $(CPPFLAGS) $(WARNINGS) $< -o $@


clean:
	-rm -r ./obj/*.o
	-rm -f $(TARGET)
	-rm -f *.tga

