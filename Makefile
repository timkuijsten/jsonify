test: jsmn.c jsonify.c test.c
	cc -Wall -Wextra -pedantic jsmn.c jsonify.c test.c && ./a.out; echo $?

.PHONY: clean 
clean:
	rm -f *.o a.out
