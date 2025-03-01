# Library-Management-System
The Online Library Management System (OLMS) is implemented using system calls for various
operations, including process management, file handling, file locking, multithreading, and interprocess
communication. Socket programming is utilized for client-server communication, allowing multiple
clients to access the library database simultaneously.

# Functionalities
1. User Authentication: Members are required to pass through a login system to access their
accounts, ensuring data privacy and security.
2. Administrative Access: Password-protected administrative access is provided to librarians,
enabling them to manage book transactions and member information.
3. Book Management: Administrators can add, delete, modify, and search for specific book details,
ensuring efficient management of library resources.
4. File-Locking Mechanisms: Proper file-locking mechanisms are implemented using system calls
to protect critical data sections and ensure data consistency.
5. Concurrent Access: The system employs socket programming to service multiple clients
concurrently, facilitating seamless access to library resources.

# How To Run
- Set up the correct file path according to your system in header file.
-  To create admin and client's run the following command :
```bash
gcc setdata.c
```

- Run server side on one terminal screen with the following command:
```bash
gcc -pthread main_server.c server.c admin.c user.c
```

- And run the client side on another terminal screen with the command :
```bash
gcc client.c user.c admin.c
```
