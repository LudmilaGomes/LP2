#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int id1 = fork();

    int id2 = fork();

    /*
    fork1, fork2
    
        main1
        |   |
    child2   child1
            |
            child-child2

    main:           id1 = x
                    id2 = z
    child1:         id1 = 0
                    id2 = y
    child2:         id1 = x
                    id2 = 0
    child-child2:   id1 = 0
                    id2 = 0
    
    */

    return 0;
}