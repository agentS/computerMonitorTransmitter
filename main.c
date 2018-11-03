#include "./stats.h"

#include <libserialport.h>

/* // TODO: include once glibc 2.28 is widely available
#include <threads.h>
#include <time.h>
*/
#include <signal.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const int BAUDRATE = 9600;

static const size_t BUFFER_SIZE = 42;
static const unsigned int SLEEP_TIME = 5;

/* // TODO: include once glibc 2.28 is widely available
static struct timespec SLEEP_TIME { .tv_sec = 5 };
*/

void print_usage(const char *program_name);

void init_signal_handler(void);
void handle_signal(int signal);

void send_status(struct sp_port *port);

static struct sp_port *port = NULL;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	init_statistics_retriever();
	init_signal_handler();

	const char *port_name = argv[1];
	if (sp_get_port_by_name(port_name, &port) == SP_OK)
	{
		if (sp_open(port, SP_MODE_WRITE) == SP_OK)
		{
			if (sp_set_baudrate(port, BAUDRATE) == SP_OK)
			{
				fprintf(stdout, "Opened port %s.\n", port_name);

				/* // TODO: include once glibc 2.28 is widely available
				do
				{
					send_status(port);
				} while(thrd_sleep(SLEEP_TIME, NULL) == 0);
				*/
				do
				{
					send_status(port);
				} while (sleep(SLEEP_TIME) == 0);
				
				return EXIT_SUCCESS;
			}
			else
			{
				fprintf(stderr, "Could not apply the configuration for port %s\n", port_name);
				sp_close(port);
				return EXIT_FAILURE;
			}
		}
		else
		{
			fprintf(stderr, "Could not open serial port %s\n", port_name);
			sp_close(port);
			return EXIT_FAILURE;
		}
	}
	else
	{
		fprintf(stderr, "Could not open serial port %s\n", port_name);
		return EXIT_FAILURE;
	}
}

void send_status(struct sp_port *port)
{
	char *buffer = ((char*) malloc(BUFFER_SIZE));
	snprintf
	(
		buffer, BUFFER_SIZE,
		"CPU usage: %3.2f%%\nMemory usage: %3.2f%%\n",
		get_cpu_usage(), get_memory_usage()
	);
	fprintf(stdout, "%s", buffer);

	if (sp_nonblocking_write(port, buffer, BUFFER_SIZE) == SP_OK)
	{
	}

	free(buffer);
}

void init_signal_handler(void)
{
	signal(SIGTERM, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGKILL, handle_signal);
}

void handle_signal(int signal)
{
	sp_close(port);
	fprintf(stdout, "Closed port.\n");
	
	switch (signal)
	{
	case SIGTERM:
		fprintf(stdout, "Terminating with SIGTERM.\n");
		break;
	case SIGKILL:
		fprintf(stdout, "Terminating with SIGKILL.\n");
		break;
	case SIGINT:
		fprintf(stdout, "Terminating with SIGINT.\n");
		break;
	}
}

void print_usage(const char *program_name)
{
	fprintf(stderr, "USAGE: %s <serialPortName>\n", program_name);
}
