CC := gcc
LIBS := $(shell pkg-config --static --libs glfw3) -lGL -lX11 -lpthread -lm
CC_FLAGS := -Wall -Werror -std=c99
OUT := program
OBJ := $(patsubst %.c, %.o, $(wildcard *.c))
DEPS := $(wildcard ../include/*.h)


all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS) $(CC_FLAGS)

%.o: %.c $(DEPS)
	$(CC) $< -c -o $@ $(CC_FLAGS) 

clean:
	rm -fR $(OBJ)

run: $(OUT)
	./$(OUT)
