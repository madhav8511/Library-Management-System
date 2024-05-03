#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>

#ifndef MYHEADER_H
#define MYHEADER_H

struct admin
{
    int id;
    char password[10];
};

struct client
{
    int id;
    char name[10];
    char password[10];
    int borrowed_books[5]; //A client can atmost borrowed 5 book's
};

struct book
{
    int id;
    char name[10];
    int quantity;    
};


//File path
#define ADMIN_FILE "/home/madhav/Desktop/OS-Project/data/admin.txt"
#define CLIENT_FILE "/home/madhav/Desktop/OS-Project/data/client.txt"
#define BOOK_FILE "/home/madhav/Desktop/OS-Project/data/book.txt"


//Server functions
void server(int new_sd);
void *connection(void *nsd);

//Admin functions
void menu();
int add_book(struct book *b);
int show_all_books();
int delete_book(int id);
int update_book(struct book *b);
int show_all_users();
struct book * book_detail(int id);

//User function
void show_menu();
int borrow_a_book(int client_id,int id);
int deposit_a_book(int client_id,int id);
struct client * user_details(int id);
#endif