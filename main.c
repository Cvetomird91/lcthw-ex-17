#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DATA 512
#define MAX_ROWS 100

//declare struct for address record
struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

//declare struct for the database sizes
struct Sizes {
    int max_rows;
	int max_data;
};

//declare struct that represents the database
struct Database {
    struct Sizes *sizes;
    int max_rows;
	int max_data;
	struct Address rows[MAX_ROWS];
};

//declare a struct that represents the DB connection to a file pointer and Database struct
struct Connection {
	FILE *file;
	struct Database *db;
};

//function for error handling
void die(const char *message, struct Connection *conn) {
	//check if the errno variable is set (system variable)
	if(errno) {
		//if the system variable errno is set print the corresponding error message with perror
		perror(message);
	} else {
		//otherwise print the string passed to the function
		printf("ERROR: %s\n", message);
    }

    if (conn->db) {
        free(conn->db);
    }

    /*
    if (conn->db->sizes) {
        free(conn->db->sizes);
    }*/

    if (conn->file) {
        fclose(conn->file);
    }

	if (conn) {
        free(conn);
    }

	exit(1);
}

//output the info about an address
void Address_print(struct Address *addr) {
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {

	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	/*
		ptr − This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
		size − This is the size in bytes of each element to be read.
		nmemb − This is the number of elements, each one with a size of size bytes.
		stream − This is the pointer to a FILE object that specifies an input stream.
	*/

	if (!conn)
        die("Failed to load database.", conn);

	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("Failed to load database.", conn);
}

//open database and return a connection
struct Connection *Database_open(const char *filename, char mode) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error!", conn);

	conn->db = malloc(sizeof(struct Database));

	conn->db->sizes = malloc(sizeof(struct Sizes));
	if (!conn->db) {
        die ("Memory error!", conn);
	}

	//open the file pointer in a different mode depending on the command
	if (mode == 'c') {
		//if we choose the create mode open in "write"
		//if a file with the same name exists - truncate it
		conn->file = fopen(filename, "w");
	} else {
		//if we choose another option open in r+ mode (for update, both for reading and writing)
		conn->file = fopen(filename, "r+");

		if(conn->file)
			Database_load(conn);
	}

	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

//free the Database and Connection structs from the heap
void Database_close(struct Connection *conn) {
	if(conn){
		if(conn->file) fclose(conn->file);
		if(conn->db->sizes) free(conn->db->sizes);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1 ) die("Failed to write database.", conn);

	rc = fflush(conn->file);
	if (rc == -1) die ("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int max_data, int max_rows) {
	int i = 0;

	conn->db->sizes->max_data = max_data;
	conn->db->sizes->max_rows = max_rows;

	for(i = 0; i < conn->db->sizes->max_rows; i++) {
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first.", conn);

	addr->set = 1;
	char *res = strncpy(addr->name, name, MAX_DATA);
	//Demonstrate the strncpy bug
	if (!res) die("Name copy failed", conn);

	/*
        dest − This is the pointer to the destination array where the content is to be copied.
        src − This is the string to be copied.
        n − The number of characters to be copied from source.
    */
	res = strncpy(addr->email, email, MAX_DATA);
	if (!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id) {
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct  Connection *conn) {
	int i = 0;
	struct Database *db = conn->db;

	for (i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];

		if(cur->set){
			Address_print(cur);
		}
	}
}

void Database_set_size(struct Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db->sizes, sizeof(struct Sizes), 1, conn->file);
	if(rc != 1 ) die("Failed to write database.", conn);

	rc = fflush(conn->file);
	if (rc == -1) die ("Cannot flush database.", conn);

}

void Database_get_size(struct Connection *conn) {
	rewind(conn->file);
    struct Sizes *buff = (struct Sizes*)malloc(sizeof(struct Sizes));

	fread(buff, sizeof(struct Sizes), 1, conn->file);

    printf("Data from the file: %d %d", buff->max_rows, buff->max_data);
    exit(0);

}

int main(int argc, char** argv){

    struct Connection *conn = malloc(sizeof(struct Connection));

    if (argc <3)
    die("USAGE: ex17 <dbfile> <action> [action params]\nor ex17 <dbfile> c <max_rows> <max_data>", conn);

	char *filename = argv[1];
	char action = argv[2][0];

	free(conn);
	conn = Database_open(filename, action);

	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	//if(id >= MAX_ROWS) die("There's not that many records", conn);
	//if(id >= conn->db->max_rowss) die("There's not that many records", conn);

	int max_rows;
	int max_data;

	if (action == 'c' && argc != 5)
        die ("Please, specify maximum database rows and data size: ex17 <dbfile> c <max_rows> <max_data>", conn);

	switch (action) {
		case 'c':

			max_rows = atoi(argv[3]);
			max_data = atoi(argv[4]);

			Database_create(conn, max_data, max_rows);
			Database_set_size(conn);

			Database_get_size(conn);
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
