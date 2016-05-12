#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int argc, char const *argv[]) {

    if(argc <2)
    {
        printf("Please insert process id\n");
        return 0;
    }

    int process_id = atoi(argv[1]);

    char part1[1000], part2[1000];
    strcpy(part1, "ps --ppid ");
    strcpy(part2, " --format '%P %p'");

    strcat(part1, argv[1]);
    strcat(part1, part2);

    FILE *fp = popen(part1 , "r");

    if (fp == NULL)
    {
        printf("ERROR!\n");
    }

    char parentID[256];
    char processID[256];
    int childProcess[1000], count=0;
    char firstTime = 0;
    while (fscanf(fp, "%s %s", parentID, processID) != EOF)
    {
        printf("PID: %s  Parent: %s\n", processID, parentID);
        if(firstTime==0)
            firstTime=1;
        else
            childProcess[count++] = atoi(processID);
    }
    pclose(fp);

    printf("Are you sure to kill %d and its children:\n", process_id);
    int f1;
    for(f1=0; f1<count;f1++)
    {
        printf("%d\n", childProcess[f1]);
    }
    printf("(y/n)\n");
    char ans;
    scanf("%c", &ans);

    if(ans !='y')
        return 0;

    // Kill Child
    int killRes;
    for(f1=0; f1<count; f1++)
    {
        killRes = kill(childProcess[f1], SIGHUP);
        if(killRes!=0)
            printf("Problem killing %d\n", childProcess[f1]);
    }
    // Kill Parent
    killRes = kill(process_id, SIGHUP);
    if(killRes!=0)
        printf("Problem killing %d\n", process_id);

    printf("%d and its children ", process_id);
    for(f1=0; f1<count; f1++)
    {
        if(f1 == 0)
            printf("%d", childProcess[f1]);
        else
            printf(", %d", childProcess[f1]);
    }
    printf(" are killed.\n");

    return 0;
}
