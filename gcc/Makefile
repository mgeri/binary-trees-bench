all: binarytrees binarytrees2 binarytrees3 binarytrees4

binarytrees: binarytrees.c
	gcc -pipe -Wall -O3 -fomit-frame-pointer -march=core2  binarytrees.c -o binarytrees.gcc_run -lm

binarytrees2: binarytrees2.c
	gcc -pipe -Wall -O3 -fomit-frame-pointer -march=core2 -pthread binarytrees2.c -o binarytrees2.gcc_run -lm

binarytrees3: binarytrees3.c
	gcc -pipe -Wall -O3 -fomit-frame-pointer -march=core2 binarytrees3.c -o binarytrees3.gcc_run -lm

binarytrees4: binarytrees4.c
	gcc -pipe -Wall -O3 -fomit-frame-pointer -march=core2 -pthread binarytrees4.c -o binarytrees4.gcc_run -lm
