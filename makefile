TARGET = mapred
OBJECTS = io_api.o multi_core.o word_count.o
FLAGS = -Wall -fsanitize=address -fsanitize-recover=address -g

$(TARGET): io_api.o multi_core.o map_reduce.o word_count.o
		gcc $(FLAGS) -o $@ $^


clean:
	rm -f $(TARGET) $(OBJECTS)

%.o: %.c
	gcc $(FLAGS) -c $<

io_api.o: io_api.h
 multi_core.o: io_api.h map_reduce.h word_count.h
 word_count.c: word_count.h
