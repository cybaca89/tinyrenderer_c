NAME = aa
CC = gcc
FLAGS = -std=gnu99 -g -pedantic
PPFLAGS = -Wl,--build-id=sha1
WARNINGS := -Wall -Wshadow -Wextra -Werror -Wundef -Wpointer-arith\
-Wcast-align -Wwrite-strings -Wconversion -Wunreachable-code
LIBS = -lm
#LIBS = -lglut -lGL -lGLU -lfl -lGLEW \
#-lX11 -lxcb -lXrandr -lXi \
#-lpthread

GEO_DIR = ..

OBJS = testobj/mathtest.o

OBJS += testobj/vecMat4.o
OBJS += testobj/mat4array_copy.o testobj/mat4array_frustrum.o
OBJS += testobj/mat4array_look.o testobj/mat4array_multiply.o
OBJS += testobj/mat4array_perspective.o testobj/mat4array_print.o
OBJS += testobj/mat4array_rotate.o testobj/mat4array_scale.o
OBJS += testobj/mat4array_set.o testobj/mat4array_translate.o
OBJS += testobj/mat4array_transpose.o

$(NAME):$(OBJS)
	$(CC) -o $@ $^ $(LIBS) -Ltestobj
	@echo compilation successful af

testobj/mathtest.o:mathtest.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@


testobj/vecMat4.o:$(GEO_DIR)/vecMat4/vecMat4.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj


testobj/mat4array_copy.o:$(GEO_DIR)/mat4array_copy.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_frustrum.o:$(GEO_DIR)/mat4array_frustrum.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_look.o:$(GEO_DIR)/mat4array_look.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_multiply.o:$(GEO_DIR)/mat4array_multiply.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_perspective.o:$(GEO_DIR)/mat4array_perspective.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_print.o:$(GEO_DIR)/mat4array_print.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_rotate.o:$(GEO_DIR)/mat4array_rotate.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_scale.o:$(GEO_DIR)/mat4array_scale.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_set.o:$(GEO_DIR)/mat4array_set.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_translate.o:$(GEO_DIR)/mat4array_translate.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

testobj/mat4array_transpose.o:$(GEO_DIR)/mat4array_transpose.c
	$(CC) $(FLAGS) $(WARNINGS) -c $< -o $@ -Ltestobj

.PHONY: clean
clean:
	$(RM) $(NAME) *.o *.s testobj/*.o

#asm.s:main.c
#	$(CC) -S -fverbose-asm -o $@ $(FLAGS) $<

