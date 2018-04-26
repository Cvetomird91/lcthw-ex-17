//declare struct for address record
struct Address;

//declare struct for the database sizes
struct Config;

//declare struct that represents the database
struct Database;

//declare a struct that represents the DB connection to a file pointer and Database struct
struct Connection;

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
