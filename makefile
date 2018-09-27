TARGET = mapred
OBJECTS = io_api.o multi_core.o word_count.o map_reduce.o core_utility.o
FLAGS = -Wall -fsanitize=address -fsanitize-recover=address -g

$(TARGET): io_api.o multi_core.o map_reduce.o word_count.o core_utility.o
		gcc $(FLAGS) -o $@ $^


clean:
	rm -f $(TARGET) $(OBJECTS)

%.o: %.c
	gcc $(FLAGS) -c $<

 core_utility.c:  core_utility.h core_utility.h
 io_api.c: io_api.h
 multi_core.c: io_api.h map_reduce.h word_count.h
 word_count.c: word_count.h core_utility.h
 map_reduce.c: map_reduce.h
