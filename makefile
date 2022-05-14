flappy: flappy.c pipeheightgen.c
	gcc -o $@ $^ -lncurses -pthread

clean:
	rm flappy
