SYSCONF_LINK = gcc
CPPFLAGS     = -std=gnu99
LDFLAGS      = -Werror -Wshader -Wconversion -Wunreachable-code -Wundef -pedantic
LIBS         = -lm

DESTDIR = ./
TARGET  = aa

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga

