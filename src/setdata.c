#include "../header/myheader.h"

int main() {
    int ad = open(ADMIN_FILE, O_CREAT | O_WRONLY, 0744);
    if (ad == -1) {
        perror("Failed to open file\n");
        return 1;
    }

    //Locking start...
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(ad,F_SETLKW,&writelk);
    if(status==-1)
        printf("could not lock the file\n");
    else
        printf("Admin File Locked\n");

    struct admin *a = (struct admin *)malloc(sizeof(struct admin)); 
    if (a == NULL) {
        perror("Failed to allocate memory\n");
        return 1;
    }

    a->id = 1;
    strcpy(a->password, "admin");
    int success = write(ad, a, sizeof(struct admin));
    if(success == -1) printf("Error in data written\n");
    else printf("Admin added successfully\n"); 

    writelk.l_type=F_UNLCK;
    status = fcntl(ad,F_SETLKW,&writelk);

    if(status == -1) printf("Error in unlocking");
    else printf("Admin file unlocked\n");
    close(ad);
    free(a);

    int clt = open(CLIENT_FILE,O_CREAT | O_WRONLY, 0744);
    if(clt == -1){
        perror("Failed to open file\n");
        return 1;
    }

    // Set a write lock on file
    status = fcntl(clt,F_SETLKW,&writelk);
    if(status==-1)
        printf("could not lock the file\n");
    else
        printf("Client File Locked\n");

    for(int i=1;i<=5;i++)
    {
        struct client *c = (struct client *)malloc(sizeof(struct client));
        c->id = i;
        strcpy(c->name,"client");
        strcpy(c->password,"enter");
        for(int i=0;i<5;i++)
        {
            c->borrowed_books[i] = -1;
        }
        write(clt,c,sizeof(struct client));
        free(c);
    }
    printf("5 Clients added successfully\n");
    writelk.l_type=F_UNLCK;
    status = fcntl(ad,F_SETLKW,&writelk);

    //Read to confirm data
    clt = open(CLIENT_FILE,O_RDONLY,0744);
    for(int i=0;i<5;i++)
    {
        struct client *c = (struct client *)malloc(sizeof(struct client));
        read(clt,c,sizeof(struct client));
        printf("ID: %d, Name: %s, Password: %s, Book Issue (ID's): ",c->id,c->name,c->password);
        for(int i=0;i<5;i++)
        {
            printf("%d",c->borrowed_books[i]);
        }
        printf("\n");
        free(c);
    }

    if(status == -1) printf("Error in unlocking\n");
    else printf("Client file unlocked\n");
    close(clt);
    return 0;
}
