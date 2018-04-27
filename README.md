# lcthw-ex-17
Extended code of Exercise 17 from the "Learn C the Hard Way" book.

This example turned into a proof-of-concept project that represents a serverless database (similar to the way SQLite works).
It reads and writes data to a binary file.

The first bytes of the file store configurations for the database and are always ommitted first when read and write operations
against the database are performed. The metadata contains the maximum number of rows the database can have and the maximum
length in bytes of the emails and names of the records.

I have added a CMake config for generating Code::Blocks project files.
