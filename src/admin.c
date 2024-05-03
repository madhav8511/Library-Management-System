#include "../header/myheader.h"

void menu()
{
    printf("\n");
    printf("0. To Logout\n");
    printf("1. Add a Book\n");
    printf("2. Delete a Book\n");
    printf("3. Update a Book\n");
    printf("4. Show All Books\n");
    printf("5. Show All User Linked\n");
    printf("\n");
}

int add_book(struct book *b)
{
    int fd = open(BOOK_FILE, O_CREAT | O_RDWR, 0744);
    int book_count = show_all_books();
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(fd,F_SETLKW,&writelk);

    for(int i=0;i<book_count;i++)
    {
        struct book *b1 = (struct book *)malloc(sizeof(struct book));
        int r = read(fd,b1,sizeof(struct book));
        if(b1->id == b->id)
        {
            writelk.l_type=F_UNLCK;
            status = fcntl(fd,F_SETLKW,&writelk);
            return 0;
        }
    }

    lseek(fd,0,SEEK_END);
    write(fd,b,sizeof(struct book));
    writelk.l_type=F_UNLCK;
    status = fcntl(fd,F_SETLKW,&writelk);
    return 1;
}

int show_all_books()
{
    int fd = open(BOOK_FILE,O_RDONLY,0744);
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd,0,SEEK_SET);
    size_t struct_size = sizeof(struct book);
    size_t num_structs = file_size / struct_size;
    if(num_structs <= 0) num_structs = 0;
    close(fd);
    return num_structs;
}

int show_all_users()
{
    int fd = open(CLIENT_FILE,O_RDONLY,0744);
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd,0,SEEK_SET);
    size_t struct_size = sizeof(struct client);
    size_t num_structs = file_size / struct_size;
    if(num_structs <= 0) num_structs = 0;
    close(fd);
    return num_structs;
}

int delete_book(int id)
{
    int fd = open(BOOK_FILE, O_CREAT | O_RDWR, 0744);
    int book_count = show_all_books();
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(fd,F_SETLKW,&writelk);

    for(int i=0;i<book_count;i++)
    {
        struct book *b1 = (struct book *)malloc(sizeof(struct book));
        int r = read(fd,b1,sizeof(struct book));
        if(b1->id == id)
        {
            b1->quantity = 0;
            off_t current_offset = lseek(fd, 0, SEEK_CUR);
            lseek(fd,current_offset-sizeof(struct book),SEEK_SET);
            write(fd,b1,sizeof(struct book));
            writelk.l_type=F_UNLCK;
            status = fcntl(fd,F_SETLKW,&writelk);
            return 1;
        }
    }
    writelk.l_type=F_UNLCK;
    status = fcntl(fd,F_SETLKW,&writelk);
    return 0;
}

int update_book(struct book *b)
{
    int fd = open(BOOK_FILE, O_CREAT | O_RDWR, 0744);
    int book_count = show_all_books();
    struct flock writelk;
    writelk.l_type=F_WRLCK; 
    writelk.l_whence=SEEK_SET; 
    writelk.l_start=0; 
    writelk.l_len=0; 
    writelk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(fd,F_SETLKW,&writelk);

    for(int i=0;i<book_count;i++)
    {
        struct book *b1 = (struct book *)malloc(sizeof(struct book));
        int r = read(fd,b1,sizeof(struct book));
        if(b1->id == b->id)
        {
            strcpy(b1->name,b->name);
            b1->quantity = b->quantity;
            off_t current_offset = lseek(fd, 0, SEEK_CUR);
            lseek(fd,current_offset-sizeof(struct book),SEEK_SET);
            write(fd,b1,sizeof(struct book));
            writelk.l_type=F_UNLCK;
            status = fcntl(fd,F_SETLKW,&writelk);
            return 1;
        }
    }
    writelk.l_type=F_UNLCK;
    status = fcntl(fd,F_SETLKW,&writelk);
    return 0;
}

struct book * book_detail(int id)
{
    int bd = open(BOOK_FILE,O_RDONLY,0744);
    struct flock readlk;
    readlk.l_type=F_RDLCK; 
    readlk.l_whence=SEEK_SET; 
    readlk.l_start=0; 
    readlk.l_len=0; 
    readlk.l_pid=getpid(); 

    // Set a write lock on file
    int status = fcntl(bd,F_SETLKW,&readlk);

    int total_books = show_all_books();
    for(int i=0;i<total_books;i++)
    {
        struct book *b = (struct book *)malloc(sizeof(struct book));
        read(bd,b,sizeof(struct book));
        if(b->id == id) return b;
        free(b);
    }
    readlk.l_type = F_UNLCK;
    status = fcntl(bd,F_SETLKW,&readlk);
    close(bd);
}