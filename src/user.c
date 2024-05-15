#include "../header/myheader.h"

void show_menu()
{
    //Menu and functionality in user...
    printf("\n");
    printf("0. To Logout\n");
    printf("1. Show All Books\n");
    printf("2. Borrow a Book\n");
    printf("3. Deposit a Book\n");
    printf("4. My Details's\n");
    printf("5. Search a Book\n");
    printf("\n");
}

//Borrow book function and file locking to prevent race condition in book quantity variable...
int borrow_a_book(int client_id, int id)
{
    int total_books = show_all_books();
    int total_user = show_all_users();
    int bd = open(BOOK_FILE,O_RDWR,0744);
    int cd = open(CLIENT_FILE,O_RDWR,0744);

    //File locking mechanism....
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(bd,F_SETLKW,&writelk);
    status = fcntl(cd,F_SETLKW,&writelk);

    for(int i=0;i<total_books;i++)
    {
        struct book *b1 = (struct book *)malloc(sizeof(struct book));
        int r = read(bd,b1,sizeof(struct book));
        if(b1->id == id)
        {
            if(b1->quantity > 0)
            {

                off_t current_offset = lseek(bd, 0, SEEK_CUR);
                b1->quantity--;
                lseek(bd,current_offset-sizeof(struct book),SEEK_SET);
                write(bd,b1,sizeof(struct book));

                //Add that in client record....
                lseek(cd,(client_id-1)*sizeof(struct client),SEEK_SET);
                struct client *c = (struct client *)malloc(sizeof(struct client));
                read(cd,c,sizeof(struct client));

                int flag = 0;
                for(int i=0;i<5;i++)
                {
                    if(c->borrowed_books[i] == -1)
                    {
                        c->borrowed_books[i] = b1->id;
                        flag = 0;
                        break;
                    }
                    else flag = 1;
                }
                if(flag == 1)
                {
                    writelk.l_type=F_UNLCK;
                    status = fcntl(bd,F_SETLKW,&writelk);
                    status = fcntl(cd,F_SETLKW,&writelk);
                    close(bd);
                    close(cd);
                    return -1;
                }
                else
                {
                    lseek(cd,(client_id-1)*sizeof(struct client),SEEK_SET);
                    write(cd,c,sizeof(struct client));
                    free(c);
                }
                writelk.l_type=F_UNLCK;
                status = fcntl(bd,F_SETLKW,&writelk);
                status = fcntl(cd,F_SETLKW,&writelk);
                return 1;
            }
        }
        free(b1);
    }
    writelk.l_type=F_UNLCK;
    status = fcntl(bd,F_SETLKW,&writelk);
    status = fcntl(cd,F_SETLKW,&writelk);
    close(bd);
    close(cd);
    return 0;
}

//Deposit book function only accept those books which you have issued....
int deposit_a_book(int client_id,int id)
{
    int total_books = show_all_books();
    int bd = open(BOOK_FILE,O_RDWR,0744);
    int cd = open(CLIENT_FILE,O_RDWR,0744);
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(bd,F_SETLKW,&writelk);
    status = fcntl(cd,F_SETLKW,&writelk);
    for(int i=0;i<total_books;i++)
    {
        struct book *b1 = (struct book *)malloc(sizeof(struct book));
        int r = read(bd,b1,sizeof(struct book));
        if(b1->id == id)
        {

            lseek(cd,(client_id-1)*sizeof(struct client),SEEK_SET);
            struct client *c = (struct client *)malloc(sizeof(struct client));
            read(cd,c,sizeof(struct client));
            int flag = 0;
            for(int i=0;i<5;i++)
            {
                if(c->borrowed_books[i] == id)
                {
                    flag = 1;
                    c->borrowed_books[i] = -1;
                    break;
                }
                else flag = 0;
            }
            if(flag == 0)
            {
                writelk.l_type=F_UNLCK;
                status = fcntl(bd,F_SETLKW,&writelk);
                status = fcntl(cd,F_SETLKW,&writelk);
                close(bd);
                close(cd);
                return -1;
            } 
            off_t current_offset = lseek(bd, 0, SEEK_CUR);
            b1->quantity++;
            lseek(bd,current_offset-sizeof(struct book),SEEK_SET);
            write(bd,b1,sizeof(struct book));
            lseek(cd,(client_id-1)*sizeof(struct client),SEEK_SET);
            write(cd,c,sizeof(struct client));
            writelk.l_type=F_UNLCK;
            status = fcntl(bd,F_SETLKW,&writelk);
            status = fcntl(cd,F_SETLKW,&writelk);
            close(bd);
            close(cd);
            return 1;
        }
    }
    writelk.l_type=F_UNLCK;
    status = fcntl(bd,F_SETLKW,&writelk);
    status = fcntl(cd,F_SETLKW,&writelk);
    close(bd);
    close(cd);
    return 0;
}

//Function to show user it's detail like name and id and which are the books he/she borrowed...
struct client * user_details(int id)
{
    int cd = open(CLIENT_FILE,O_RDONLY,0744);
    struct flock readlk;
    readlk.l_type=F_RDLCK; 
    readlk.l_whence=SEEK_SET; 
    readlk.l_start=0; 
    readlk.l_len=0; 
    readlk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(cd,F_SETLKW,&readlk);

    lseek(cd,(id-1)*sizeof(struct client),SEEK_SET);
    struct client *c = (struct client* )malloc(sizeof(struct client));
    read(cd,c,sizeof(struct client));

    readlk.l_type=F_UNLCK;
    status = fcntl(cd,F_SETLKW,&readlk);
    return c;
}

int search_book(char title[10])
{
    int fd = open(BOOK_FILE, O_CREAT | O_RDWR, 0744);
    int book_count = show_all_books();
    struct flock readlk;
    readlk.l_type=F_RDLCK; 
    readlk.l_whence=SEEK_SET; 
    readlk.l_start=0; 
    readlk.l_len=0; 
    readlk.l_pid=getpid(); 

    // Set a read lock on file
    int status = fcntl(fd,F_SETLKW,&readlk);

    for(int i=0;i<book_count;i++)
    {
        struct book *b = (struct book * )malloc(sizeof(struct book));
        int r = read(fd,b,sizeof(struct book));
        if(strcmp(b->name,title) == 0 && b->quantity > 0)
        {
            return b->id;
        }
        else free(b);
    }

    readlk.l_type=F_UNLCK;
    status = fcntl(fd,F_SETLKW,&readlk);
    return 0;
}