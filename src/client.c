#include "../header/myheader.h"

int main()
{
    struct sockaddr_in client;
    int sd;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr =  INADDR_ANY ;
    client.sin_port = htons(5555);

    connect(sd, (struct sockaddr *)&client, sizeof(client));
    int choice;
    
    while(1)
    {
        printf("\n");
        printf("LOG-IN AS: \n");
        printf("0. TO CLOSE THE APPLICATION\n");
        printf("1. ADMIN\n");
        printf("2. USER\n");
        printf("\n");
        printf("Enter a valid choice: ");
        scanf("%d",&choice);
        write(sd,&choice,sizeof(int)); //Send choice to server side
        if(choice == 1)
        {
            printf("\n");
            printf("--- Welcome to Admin Portal ---\n");
            printf("Enter Admin Id: ");
            int admin_id;
            scanf("%d",&admin_id);
            write(sd,&admin_id,sizeof(int));
            printf("Enter Password: ");
            char buffer[10];
            scanf("%s",buffer);
            write(sd,buffer,sizeof(buffer));

            char response[10];
            read(sd,response,sizeof(response));
            if(strcmp(response,"YES") == 0) 
            {
                printf("\n");
                printf("--- Authenticated Successfully ---\n");
                printf("\n");
                printf("--- Functionality ---\n");
                menu(); //Admin menu....
                int fnchoice;
                while(1)
                {
                    printf("\n");
                    printf("Enter a valid choice from Admin menu: ");
                    scanf("%d",&fnchoice);
                    write(sd,&fnchoice,sizeof(int));
                    //Add a book..
                    if(fnchoice == 1)
                    {
                        printf("Enter Book Id: ");
                        int id; scanf("%d",&id); write(sd,&id,sizeof(int));
                        printf("Enter Book Name: ");
                        char name[10]; scanf("%s",name); write(sd,name,sizeof(name));
                        printf("Enter Book Quantity: ");
                        int quantity; scanf("%d",&quantity); write(sd,&quantity,sizeof(int));

                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Book added successfully\n");
                        else printf("Book with same id already exist\n");
                        printf("\n");
                    }
                    //Delete a book...
                    else if(fnchoice == 2)
                    {
                        printf("Enter Book ID: ");
                        int id; scanf("%d",&id); write(sd,&id,sizeof(int));
                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Book deleted successfully\n");
                        else printf("Sorry book doesn't exist\n");
                        printf("\n");
                    }
                    //Update a book...
                    else if(fnchoice == 3)
                    {
                        printf("Enter Book Id: ");
                        int id; scanf("%d",&id); write(sd,&id,sizeof(int));
                        printf("Enter Book Name: ");
                        char name[10]; scanf("%s",name); write(sd,name,sizeof(name));
                        printf("Enter Book Quantity: ");
                        int quantity; scanf("%d",&quantity); write(sd,&quantity,sizeof(int));
                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Book Updated successfully\n");
                        else printf("Sorry book doesn't exist\n");
                        printf("\n");
                    }
                    // Show all books....
                    else if(fnchoice == 4)
                    {
                        int no_of_books;
                        read(sd,&no_of_books,sizeof(int));
                        if(no_of_books<=0) no_of_books=0;
                        printf("There are %d books present in Library\n",no_of_books);
                        printf("\n");
                        for(int i=0;i<no_of_books;i++)
                        {
                            struct book *b = (struct book *)malloc(sizeof(struct book));
                            read(sd,b,sizeof(struct book));
                            printf("Book Id: %d, Book Title: %s, Copies-Available: %d\n",b->id,b->name,b->quantity);
                        }
                        printf("\n");
                    }
                    //Show all user linked with library...
                    else if(fnchoice == 5)
                    {
                        int no_of_users;
                        read(sd,&no_of_users,sizeof(int));
                        if(no_of_users<=0) no_of_users=0;
                        printf("There are %d users linked with Library\n",no_of_users);
                        printf("\n");
                        for(int i=0;i<no_of_users;i++)
                        {
                            struct client *b = (struct client *)malloc(sizeof(struct client));
                            read(sd,b,sizeof(struct client));
                            printf("User Id: %d, User Name: %s, Book-Issues (ID's): ",b->id,b->name);
                            for(int i=0;i<5;i++)
                            {
                                if(b->borrowed_books[i] != -1)
                                {
                                    printf("%d, ",b->borrowed_books[i]);
                                }
                            }
                            printf("\n");
                        }
                        printf("\n");
                    }
                    //Add a user...
                    else if(fnchoice == 6)
                    {
                        printf("Enter user-name: ");
                        char name[10]; scanf("%s",name);
                        write(sd,name,sizeof(name));
                        printf("Enter password: ");
                        char pass[10]; scanf("%s",pass);
                        write(sd,pass,sizeof(pass));

                        int id;
                        read(sd,&id,sizeof(int));
                        printf("\n");
                        printf("User registered with ID: %d\n",id);
                        printf("Remember your id and password for authentication\n");
                        printf("\n");
                    }
                    else if(fnchoice == 0) 
                    {
                        printf("Successfully Logout\n");
                        printf("\n");
                        break;
                    }
                }
            }
            else 
            {
                printf("Not a valid id/password\n");   
                //break;
            }
        }
        else if(choice == 2)
        {
            printf("\n");
            printf("--- Welcome to User Portal ---\n");
            printf("Enter User Id: ");
            int user_id;
            scanf("%d",&user_id);
            write(sd,&user_id,sizeof(int));
            printf("Enter Password: ");
            char buffer[10];
            scanf("%s",buffer);
            write(sd,buffer,sizeof(buffer));

            char response[10];
            read(sd,response,sizeof(response));
            if(strcmp(response,"YES") == 0) 
            {
                printf("\n");
                printf("--- Authenticated Successfully ---\n");
                printf("\n");
                printf("--- Functionality ---\n");
                show_menu(); //user menu...
                int fnchoice;
                while(1)
                {
                    printf("\n");
                    printf("Enter a valid choice from User menu: ");
                    scanf("%d",&fnchoice);
                    write(sd,&fnchoice,sizeof(int));
                    //Show all books present in library...
                    if(fnchoice == 1)
                    {
                        int no_of_books;
                        read(sd,&no_of_books,sizeof(int));
                        if(no_of_books <= 0) no_of_books=0;
                        printf("There are %d books present in Library\n",no_of_books);
                        printf("\n");
                        for(int i=0;i<no_of_books;i++)
                        {
                            struct book *b = (struct book *)malloc(sizeof(struct book));
                            read(sd,b,sizeof(struct book));
                            printf("Book Id: %d, Book Title: %s, Copies-Available: %d\n",b->id,b->name,b->quantity);
                        }
                        printf("\n");
                    }
                    //Borrow a book...
                    else if(fnchoice == 2)
                    {
                        printf("Enter Book Id: ");
                        int id; scanf("%d",&id); write(sd,&id,sizeof(int));
                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Book Borrowed successfully\n");
                        else if(strcmp(response,"F") == 0) printf("Sorry This Book is currently not available\n");
                        else printf("You are on your limit for borrowing books.\n");
                        printf("\n");
                    }
                    //Deposit a book...
                    else if(fnchoice == 3)
                    {
                        printf("Enter Book Id: ");
                        int id; scanf("%d",&id); write(sd,&id,sizeof(int));
                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Book Deposit successfully\n");
                        else if(strcmp(response,"F") == 0) printf("Sorry This Book is not the part of our Library\n");
                        else printf("Sorry this book is not issued by You\n");
                        printf("\n");
                    }
                    //Show user it's detail....
                    else if(fnchoice == 4)
                    {
                        struct client *c = (struct client *)malloc(sizeof(struct client));
                        read(sd,c,sizeof(struct client));
                        printf("\n");
                        printf("User Id: %d\n",c->id);
                        printf("User Name: %s\n",c->name);
                        printf("Borrowed Book's: \n");
                        for(int i=0;i<5;i++) 
                        {
                            if(c->borrowed_books[i] != -1)
                            {
                                struct book *b = (struct book *)malloc(sizeof(struct book));
                                b = book_detail(c->borrowed_books[i]);
                                printf("Book Id: %d, Book Title: %s\n",b->id,b->name);
                                free(b);
                            }
                        }
                        printf("\n");
                    }
                    //Search a book by title....
                    else if(fnchoice == 5)
                    {
                        char title[10];
                        printf("Enter book title: "); scanf("%s",title);
                        write(sd,title,sizeof(title));

                        int id; read(sd,&id,sizeof(int));

                        char response[10];
                        read(sd,response,sizeof(response));
                        printf("\n");
                        if(strcmp(response,"S") == 0) printf("Yes, that book is present in the library with ID: %d\n",id);
                        else printf("Sorry that book is currently not available\n");
                        printf("\n");
                    }
                    //To logout....
                    else if(fnchoice == 0)
                    {
                        printf("Successfully Logout\n");
                        printf("\n");
                        break;
                    }
                }
            }
            else 
            {
                printf("Not a valid id/password\n");   
                //break;
            }
        }
        else break;
    }  
    close(sd);
    return (0);
}