#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

//declare struct for address record
struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

//declare struct that represents the database
struct Database {
	struct Address rows[MAX_ROWS];
};

//declare a struct that represents the DB connection to a file pointer and Database struct
struct Connection {
	FILE *file;
	struct Database *db;
};

//function for error handling
//void die(const char *message, Connection *conn) {
void die(const char *message) {
	//check if the errno variable is set (system variable)
	if(errno) {
		//if the system variable errno is set print the corresponding error message with perror
		perror(message);
	} else {
		//otherwise print the string passed to the function
		printf("ERROR: %s\n", message);
	}

	//free(conn);
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

	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("Failed to load database.");
}

//open database and return a connection
struct Connection *Database_open(const char *filename, char mode) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error!");

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) die ("Memory error!");

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

	if(!conn->file) die("Failed to open the file");

	return conn;
}

//free the Database and Connection structs from the heap
void Database_close(struct Connection *conn) {
	if(conn){
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1 ) die("Failed to write database.");

	rc = fflush(conn->file);
	if (rc == -1) die ("Cannot flush database.");
}

void Database_create(struct Connection *conn) {
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++) {
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first.");

	addr->set = 1;
	char *res = strncpy(addr->name, name, MAX_DATA);
	//Demonstrate the strncpy bug
	if (!res) die("Name copy failed");

	res = strncpy(addr->email, email, MAX_DATA);
	if (!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set");
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

int main(int argc, char** argv){
	if (argc <3) die("USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	if(id >= MAX_ROWS) die("There's not that many records");

	switch (action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if (argc != 4) die("Need an id to get");

			Database_get(conn, id);
			break;
		case 's':
			if(argc != 6) die("Need id, name, email to set");

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if(argc != 4) die("Need id to delete");

			Database_delete(conn, id);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
	}

	Database_close(conn);
	return 0;
}
