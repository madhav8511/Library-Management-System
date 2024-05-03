#include "../header/myheader.h"

int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, clientLen;
	pthread_t threads;
	bool result;
	sd = socket(AF_INET, SOCK_STREAM, 0);

	// Keeping localhost as the server address , same as client for demo purpose
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	// server setup steps followed as shown in class
	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);

	write(1, "Waiting for the client.....\n", sizeof("Waiting for the client.....\n"));

	while (1)
	{
		clientLen = sizeof(client);
		nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

		write(1, "Connected to the client.....\n", sizeof("Connected to the client.....\n"));
		if (pthread_create(&threads, NULL, (void *)connection, (void *)&nsd) < 0)
		{
			perror("could not create thread");
			return 1;
		}
	}

	pthread_exit(NULL);
	close(sd);
	return (0);
}