#define MAX_DATA 512
#define MAX_ROWS 100

//declare struct for address record
struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

//define struct for database size settings
struct Config {
    int max_data;
    int max_rows;
};

//declare struct that represents the database
struct Database {
	struct Address rows[MAX_ROWS];
};

//declare a struct that represents the DB connection to a file pointer and Database struct
struct Connection {
	FILE *file;
	struct Database *db;
	struct Config *config;
};

void die(const char *message, struct Connection *conn);
void Address_print(struct Address *addr);
void Database_load(struct Connection *conn);
struct Connection *Database_open(const char *filename, char mode);
void Database_close(struct Connection *conn);
void Database_write(struct Connection *conn);
void Database_create(struct Connection *conn, int max_data, int max_rows);
void Database_set(struct Connection *conn, int id, const char *name, const char *email);
void Database_get(struct Connection *conn, int id);
void Database_delete(struct Connection *conn, int id);
void Database_list(struct  Connection *conn);
