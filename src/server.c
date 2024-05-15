#include "../header/myheader.h"

void server(int new_sd)
{
	// Handling requests.
    int choice;
	while (1)
	{
        int k = read(new_sd,&choice,sizeof(int));
        printf("Chossed : %d\n",choice);
        if (choice == 1) {
            int admin_id;
            int flag = 0;
            int read_count = read(new_sd, &admin_id, sizeof(int));

            // printf("ID GET: %d\n", admin_id);
            char password[10];
            read_count = read(new_sd, password, sizeof(password));

            // printf("Password Get: %s\n", password);
            int fd = open(ADMIN_FILE, O_RDONLY, 0744);
            
            struct admin *a = (struct admin *)malloc(sizeof(struct admin));
            read_count = read(fd, a, sizeof(struct admin));

            if (a->id == admin_id && strcmp(a->password, password) == 0)
            {
                printf("Admin Logged In\n");
                write(new_sd, "YES", sizeof("YES"));
                flag = 1;
            }
            else 
            {
                write(new_sd, "NO", sizeof("NO"));
            }

            if(flag == 1){
                int admin_choice;
                while(1)
                {
                    read_count = read(new_sd,&admin_choice,sizeof(int));
                    printf("Recived Admin Choice: %d\n",admin_choice);
                    //Add a book...
                    if(admin_choice == 1)
                    {
                        int id; read(new_sd,&id,sizeof(int));
                        //printf("%d\n",id);
                        char name[10]; read(new_sd,name,sizeof(name));
                        int quantity; read(new_sd,&quantity,(sizeof(int)));
                        struct book *b = (struct book *)malloc(sizeof(struct book));
                        b->id = id;
                        strcpy(b->name,name);
                        b->quantity = quantity;
                        printf("Book details : %d, %s, %d\n",b->id,b->name,b->quantity);
                        int status = add_book(b);
                        if(status == 1) write(new_sd,"S",sizeof("S"));
                        else write(new_sd,"F",sizeof("F"));
                    }
                    //Delete a book
                    else if(admin_choice == 2)
                    {
                        int id;
                        read_count = read(new_sd,&id,sizeof(int));
                        int status = delete_book(id);
                        if(status == 1) write(new_sd,"S",sizeof("S"));
                        else write(new_sd,"F",sizeof("F"));
                    }
                    //Update a book....
                    else if(admin_choice == 3)
                    {
                        int id; read(new_sd,&id,sizeof(int));
                        char name[10]; read(new_sd,name,sizeof(name));
                        int quantity; read(new_sd,&quantity,(sizeof(int)));
                        struct book *b = (struct book *)malloc(sizeof(struct book));
                        b->id = id;
                        strcpy(b->name,name);
                        b->quantity = quantity;
                        printf("Updated Book details : %d, %s, %d\n",b->id,b->name,b->quantity);
                        int status = update_book(b);
                        if(status == 1) write(new_sd,"S",sizeof("S"));
                        else write(new_sd,"F",sizeof("F"));
                    }
                    //Show all books present in library or not...
                    else if(admin_choice == 4)
                    {
                        int st = show_all_books();
                        int bd = open(BOOK_FILE,O_RDONLY,0744);
                        struct flock readlk;
                        readlk.l_type=F_RDLCK; 
                        readlk.l_whence=SEEK_SET; 
                        readlk.l_start=0; 
                        readlk.l_len=0; 
                        readlk.l_pid=getpid(); 

                        // Set a read lock on file
                        int status = fcntl(bd,F_SETLKW,&readlk);
                        write(new_sd,&st,sizeof(int));
                        for(int i=0;i<st;i++)
                        {
                            struct book *b = (struct book *)malloc(sizeof(struct book));
                            read(bd,b,sizeof(struct book));
                            write(new_sd,b,sizeof(struct book));
                        }
                        readlk.l_type=F_UNLCK;
                        status = fcntl(bd,F_SETLKW,&readlk);
                        close(bd);
                    }
                    //Show the list of user that are linked with library...
                    else if(admin_choice == 5)
                    {
                        int st = show_all_users();
                        int cd = open(CLIENT_FILE,O_RDONLY,0744);
                        struct flock readlk;
                        readlk.l_type=F_RDLCK; 
                        readlk.l_whence=SEEK_SET; 
                        readlk.l_start=0; 
                        readlk.l_len=0; 
                        readlk.l_pid=getpid(); 

                        // Set a read lock on file
                        int status = fcntl(cd,F_SETLKW,&readlk);
                        write(new_sd,&st,sizeof(int));
                        for(int i=0;i<st;i++)
                        {
                            struct client *b = (struct client *)malloc(sizeof(struct client));
                            read(cd,b,sizeof(struct client));
                            write(new_sd,b,sizeof(struct client));
                        }
                        readlk.l_type=F_UNLCK;
                        status = fcntl(fd,F_SETLKW,&readlk);
                        close(cd);
                    }
                    //Add/Link a new user with library...
                    else if(admin_choice == 6)
                    {
                        int st = show_all_users();
                        char name[10]; read(new_sd,name,sizeof(name));
                        char password[10]; read(new_sd,password,sizeof(password));

                        struct client *c = (struct client *)malloc(sizeof(struct client));
                        int got_id = st+1;
                        c->id = got_id;
                        strcpy(c->name,name);
                        strcpy(c->password,password);
                        for(int i=0;i<5;i++) c->borrowed_books[i] = -1;
                        
                        add_user(c);
                        write(new_sd,&got_id,sizeof(got_id));
                    }
                    //Choice to logout....
                    else if(admin_choice == 0)
                    {
                        printf("Admin Logout\n");
                        break;
                    }
                }
            }
            close(fd);
            free(a);
        }
        else if(choice == 2)
        {
            int user_id;
            int flag = 0;
            int read_count = read(new_sd,&user_id,sizeof(int));
            //printf("ID get: %d\n",user_id);
            char password[10];
            read_count = read(new_sd,password,sizeof(password));
            //printf("Pass get: %s\n",password);

            int num_structs = show_all_users();
            int fd = open(CLIENT_FILE,O_RDONLY,0744);
            struct flock readlk;
            readlk.l_type=F_RDLCK; 
            readlk.l_whence=SEEK_SET; 
            readlk.l_start=0; 
            readlk.l_len=0; 
            readlk.l_pid=getpid(); 

            // Set a write lock on file
            int status = fcntl(fd,F_SETLKW,&readlk);
            int client_id;
            int count = 0;
            for(int i=0;i<num_structs;i++)
            {
                struct client *c = (struct client *)malloc(sizeof(struct client));
                read_count = read(fd,c,sizeof(struct client));
                if(c->id == user_id && (strcmp(c->password,password) == 0))
                {
                    write(new_sd,"YES",sizeof("YES"));
                    flag = 1;
                    client_id = c->id;
                    printf("User Logged In\n");
                    break;
                }
                else
                {
                    count++;
                }
            }
            if(count == num_structs) write(new_sd,"NO",sizeof("NO"));
            readlk.l_type=F_UNLCK;
            status = fcntl(fd,F_SETLKW,&readlk);
            if(flag == 1){
                int user_choice;
                while(1)
                {
                    read_count = read(new_sd,&user_choice,sizeof(int));
                    printf("Recived User Choice: %d\n",user_choice);
                    //Show all books present in library...
                    if(user_choice == 1)
                    {
                        int st = show_all_books();
                        int bd = open(BOOK_FILE,O_RDONLY,0744);
                        struct flock readlk;
                        readlk.l_type=F_RDLCK; 
                        readlk.l_whence=SEEK_SET; 
                        readlk.l_start=0; 
                        readlk.l_len=0; 
                        readlk.l_pid=getpid(); 

                        // Set a write lock on file
                        int status = fcntl(bd,F_SETLKW,&readlk);
                        write(new_sd,&st,sizeof(int));
                        for(int i=0;i<st;i++)
                        {
                            struct book *b = (struct book *)malloc(sizeof(struct book));
                            read(bd,b,sizeof(struct book));
                            write(new_sd,b,sizeof(struct book));
                        }
                        readlk.l_type=F_UNLCK;
                        status = fcntl(fd,F_SETLKW,&readlk);
                        close(bd);
                    }
                    //Borrow a book
                    else if(user_choice == 2)
                    {
                        int id;
                        read_count = read(new_sd,&id,sizeof(int));
                        int status = borrow_a_book(client_id,id);
                        if(status == 1) write(new_sd,"S",sizeof("S"));
                        else if(status == 0) write(new_sd,"F",sizeof("F"));
                        else write(new_sd,"M",sizeof("M"));
                    }
                    //Deposit / return a book to library...
                    else if(user_choice == 3)
                    {
                        int id;
                        read_count = read(new_sd,&id,sizeof(int));
                        int status = deposit_a_book(client_id,id);
                        if(status == 1) write(new_sd,"S",sizeof("S"));
                        else if(status == 0) write(new_sd,"F",sizeof("F"));
                        else write(new_sd,"M",sizeof("M"));
                    }
                    //User detail's.....
                    else if(user_choice == 4)
                    {
                        struct client *c = (struct client *)malloc(sizeof(struct client));
                        c = user_details(client_id);
                        write(new_sd,c,sizeof(struct client));
                    }
                    //Search_book ....
                    else if(user_choice == 5)
                    {
                        char title[10];
                        read(new_sd,title,sizeof(title));
                        int status = search_book(title);
                        write(new_sd,&status,sizeof(int));
                        
                        if(status == 0) write(new_sd,"F",sizeof("F"));
                        else write(new_sd,"S",sizeof("S"));
                    }
                    //For logout...
                    else if(user_choice == 0)
                    {
                        printf("User Logout\n");
                        break;
                    }
                }
            }
        }
        else if(choice == 0) 
        {
            // printf("Application Shut-Down\n");
            break;
        }
	}
	close(new_sd);
	write(1, "Ended client session.....\n", sizeof("Ended client session.....\n"));
	return;
}

void *connection(void *nsd)
{
	int nsfd = *(int *)nsd;
	server(nsfd);
}