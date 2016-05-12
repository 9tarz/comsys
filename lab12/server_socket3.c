#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#define NSTRS       3           /* no. of strings  */
#define NUM_PROCESSES 3

/*
 * Strings we send to the client.
 */
char c;
FILE *fp;
int i;

char *strs[NSTRS] = {
    "This is the first string from the server.\n",
    "This is the second string from the server.\n",
    "This is the third string from the server.\n"
};
void doprocessing (int ns) {
    /*
     * We'll use stdio for reading the socket
     */

    fp = fdopen(ns, "r");

    /*
     * First we send some strings to the client.
     */
    for (i = 0; i < NSTRS; i++)
        send(ns, strs[i], strlen(strs[i]), 0);

    /*
     * Then we read some strings from the client and
     * print them out.
     */
    /*for (i = 0; i < NSTRS; i++) {
        while ((c = fgetc(fp)) != EOF) {
            putchar(c);

            if (c == '\n')
                break;
        }
    }*/
    char str[256];
        while ((fgets(str, 255, fp)) != NULL) {
            printf("recieve %s", str);
            if (strcmp(str, "end") == 0) break;
    }

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(ns);
    
}
main(int argc, char* const argv[])
{
    int fromlen;
    register int ns;
    struct sockaddr_un saun[3], fsaun;
    char* addr1 = argv[1];
    int client_socket[3];
    char* addr2 = argv[2];
    char* addr3 = argv[3];

    for (i = 0; i < NUM_PROCESSES; i++) 
    {
        client_socket[i] = socket(AF_UNIX, SOCK_STREAM, 0);
        saun[i].sun_family = AF_UNIX;
        strcpy(saun[i].sun_path, argv[i+1]);
        unlink(argv[i+1]);
        bind(client_socket[i], (struct sockaddr *)&saun[i], SUN_LEN(&saun[i]));
        printf("Listener on port %d \n", argv[i+1]);
        listen(client_socket[i], 1);
    }

    /*
     * Accept connections.  When we accept one, ns
     * will be connected to the client.  fsaun will
     * contain the address of the client.
     */

    fromlen = sizeof(fsaun);
    puts("Waiting for connections ...");
    pid_t pids[NUM_PROCESSES];


    for (i = 0; i < NUM_PROCESSES ; i++) {

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("ERROR on fork");
            exit(1);   
        }
      
        if (pids[i] == 0) {
            // This is the client process 
            int num = i;
            if (listen(client_socket[i], 1) < 0) {
                perror("server: listen");
                exit(1);
            }
            if ((ns = accept(client_socket[i], (struct sockaddr *) &fsaun, &fromlen)) < 0) {
                perror("server: accept");
                exit(1);
            }
            printf("server%d is spawn for port%d\n",i,i);
            doprocessing(ns);
            printf("server%d is exiting\n",num);

            exit(0);
        }
        else {
            close(ns);

        }
    }
    int status;
    pid_t pid;
    while (1) {
        sleep(1);
        pid = wait(&status);
        for (i = 0; i < NUM_PROCESSES ; i++) {
            if (pids[i] == pid) {
                pids[i] = fork();
                if (pids[i] < 0) {
                    perror("ERROR on fork");
                    exit(1);   
                }
              
                if (pids[i] == 0) {
                    // This is the client process 
                    int num = i;
                    if (listen(client_socket[i], 1) < 0) {
                        perror("server: listen");
                        exit(1);
                    }
                    if ((ns = accept(client_socket[i], (struct sockaddr *) &fsaun, &fromlen)) < 0) {
                        perror("server: accept");
                        exit(1);
                    }
                    printf("server%d is spawn for port%d\n",i,i);
                    doprocessing(ns);
                    printf("server%d is exiting\n",num);
                    exit(0);
                }
                else {
                    close(ns);
                }
                
            }
        }
    }

    exit(0);
}
