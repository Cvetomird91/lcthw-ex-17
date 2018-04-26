#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "db_operations.h"

#define MAX_DATA 512
#define MAX_ROWS 100

int main(int argc, char** argv){

	char *filename = argv[1];
	char action = argv[2][0];

	struct Connection *conn = Database_open(filename, action);

	if (argc <3) die("USAGE: ex17 <dbfile> <action> [action params]", conn);

	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	if(id >= MAX_ROWS && action != 'c') die("There's not that many records", conn);

	int max_rows;
	int max_data;

	switch (action) {
		case 'c':

            max_rows = atoi(argv[3]);
            max_data = atoi(argv[4]);

			Database_create(conn, max_data, max_rows);
			Database_write(conn);

			break;
		case 'g':
			if (argc != 4) die("Need an id to get", conn);

			Database_get(conn, id);
			break;
		case 's':
			if(argc != 6) die("Need id, name, email to set", conn);

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if(argc != 4) die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}

	Database_close(conn);
	return 0;
}
