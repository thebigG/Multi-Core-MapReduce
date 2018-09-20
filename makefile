TARGET = mapred
OBJECTS = io_api.o multi_core.o
FLAGS = -Wall -fsanitize=address -fsanitize-recover=address -g

$(TARGET): io_api.o multi_core.o
		gcc $(FLAGS) -o $@ $^


clean:
	rm -f $(TARGET) $(OBJECTS)

%.o: %.c
	gcc $(FLAGS) -c $<

io_api.o: io_api.h
 multi_core.o: io_api.h
