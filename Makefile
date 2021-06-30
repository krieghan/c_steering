all: main.c game_element.c vector.c game_element.c linkedlist.c steering.c steeringbehaviors.c world.c
	     gcc -o main main.c game_element.c linkedlist.c steeringbehaviors.c steering.c vector.c world.c -lGL -lGLU -lglut -lm 
