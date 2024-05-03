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
1. Set up the correct file path according to your system in header file.
2. Run gcc setdata.c to create admin and client's
3. Now on one terminal run server side : gcc -pthread main_server.c server.c admin.c user.c
4. And on another terminal run client side : gcc client.c user.c admin.c
