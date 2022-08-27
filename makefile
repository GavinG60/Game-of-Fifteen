NAME = fifteen
FLAGS = -Wall -pedantic-errors
cl: $(NAME).c
	gcc -lm -o $(NAME) $(FLAGS) $(NAME).c
5 run: cl
	./$(NAME)
7 clean:
	rm -f $(NAME)
