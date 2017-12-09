all:
        gcc -o control control.c
        gcc -o main main.c
clean:
        rm *~
        rm a.out
