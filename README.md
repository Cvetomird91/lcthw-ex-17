# lcthw-ex-17
Extended code of Exercise 17 from the "Learn C the Hard Way" book.

This example turned into a proof-of-concept project that represents a serverless database (similar to the way SQLite works).
It reads and writes data to a binary file.

The first bytes of the file store configurations for the database and are always ommitted first when read and write operations
against the database are performed. The metadata contains the maximum number of rows the database can have and the maximum
length in bytes of the emails and names of the records.
There is a hard limit for the size of the name and email data that is specified in preprocessor constants and is 512 bytes.

I have added a BASH shell script for performing automated testing in the utils/ directory.

I have added a CMake config for generating Code::Blocks project files.

You can generate the project files for the Debug and Release modes respectively:

```
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

```
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
```

