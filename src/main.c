#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


DBusConnection *conn;

void ending_conn(int signo)
{
	printf("closing conn...");
	//dbus_connection_close(conn);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	DBusError err;
	dbus_error_init(&err);


	if(argc>1)
	{
		if(strcmp(argv[1],"session")==0)
			conn = dbus_bus_get(DBUS_BUS_SESSION,&err);
		else if(strcmp(argv[1],"address")==0)
		{
			conn= dbus_connection_open(
					"unix:path=/dev/shm/dbus_service_socket",&err);
			if(!dbus_bus_register(conn, &err))
			{
				printf("Connection Error (%s)\n", err.message);
				dbus_error_free(&err);
				exit(EXIT_FAILURE);
			}
		}
		else {exit(EXIT_FAILURE);}
	}
	else {exit(EXIT_FAILURE);}


	if(dbus_error_is_set(&err))
	{
		printf("Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn)
			exit(EXIT_FAILURE);

	if(DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER !=
			dbus_bus_request_name(conn, "com.test",
					DBUS_NAME_FLAG_REPLACE_EXISTING, &err))
	{
		fprintf(stderr, "Name Error (%s)\n", err.message);
		dbus_error_free(&err);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, ending_conn);
	signal(SIGTERM, ending_conn);

	dbus_bus_add_match(conn,
		"type='signal', path='/',"
		"interface='com.test.Interface', "
		"member='TestSignal'",
		&err
	);
	dbus_connection_flush(conn);
	if(dbus_error_is_set(&err))
	{
		printf( "Match Error (%s)\n", err.message);
		dbus_error_free(&err);
		exit(EXIT_FAILURE);
	}
	DBusMessageIter args;
	char *sigvalue;
	for(;;)
	{
		dbus_connection_read_write(conn, 0);
		DBusMessage *msg=dbus_connection_pop_message(conn);

		if(NULL==msg)
		{
			sleep(1);
			continue;
		}

		if(dbus_message_is_signal(msg,"com.test.Interface",
				"TestSignal"))
		{
			if (!dbus_message_iter_init(msg, &args))
				printf("Message has no arguments!\n");
			 else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
				printf( "Argument is not string!\n");
			 else {
				dbus_message_iter_get_basic(&args, &sigvalue);
				printf("client: %s\n", sigvalue);
			 }
		}
	}



	return 0;
}
