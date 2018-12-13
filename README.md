# Multi-Core-MapReduce
Contributors:
Anton Ryjov (https://github.com/AnPlusOn),
Lorenzo Gomez (https://github.com/thebigG),
Michelle Dry-Moran (https://github.com/rossidiva150)


To Compile: Make
To Run:
./mapred -[wordcount or sort] -[procs or threads] -[# of maps] -[# of reduces] -input_file -output_file


main files are in multi_core.c
This takes in 7 arguments.  Then it calls reader, which reads all the data from the file and
stores this in one malloc'd space called data.

The program then sends the data to a delimeter in order to break the data up based on the given delimeters
from the posted project.

Next - it calls map, followed by reduce.

Both call out to the file map_reduce.c which uses linked lists in order to word count/sort.

