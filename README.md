*This activity has been created as part of the 42 curriculum by csodini.*

# get_next_line

## Description

`get_next_line` is a C function that reads and returns one line at a time from a given file descriptor.

The goal of this project is to understand how low-level reading works with `read()`, how to manage dynamically allocated memory, and how to preserve data between multiple function calls using a static variable.

Unlike higher-level input functions, `read()` does not read line by line. It reads a fixed number of bytes defined by `BUFFER_SIZE`. Because of this, the function must be able to store data that has already been read, return only the next complete line, and keep the remaining characters for the following call.

## Prototype

```c
char	*get_next_line(int fd);
```

## Files

The project contains the following files:

```text
get_next_line.c
get_next_line_utils.c
get_next_line.h
README.md
```

## Compilation

The project can be compiled with a custom `BUFFER_SIZE` value:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

It can also be compiled without defining `BUFFER_SIZE`, in which case a default value is provided in `get_next_line.h`:

```bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c
```

## Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

The function can also read from the standard input by using file descriptor `0`:

```c
line = get_next_line(0);
```

## Algorithm

The implementation is based on three main steps:

1. Read data from the file descriptor into a temporary buffer.
2. Append this buffer to a saved string.
3. Extract the next line and keep the remaining characters for the next call.

### Static Storage

A static variable is used to store data that has already been read but not yet returned.

For example, if `read()` reads the following content:

```text
hello\nwo
```

`get_next_line` must return:

```text
hello\n
```

but it must keep:

```text
wo
```

for the next call.

This remaining data is stored in the static saved string.

### Reading Process

The function reads from the file descriptor until one of the following conditions is met:

- a newline character `\n` is found;
- `read()` reaches the end of file and returns `0`;
- `read()` fails and returns `-1`.

The temporary buffer is allocated with `BUFFER_SIZE + 1` bytes. The extra byte is needed to manually add the null terminator `\0`, since `read()` does not create C strings by itself.

### Line Extraction

Once enough data has been read, the function extracts a new string from the beginning of the saved data up to and including the first newline character.

If the end of file is reached and the last line does not end with a newline, the function returns that last line without adding a newline.

Examples:

```text
Saved data: "hello\nworld"
Returned line: "hello\n"
Remaining data: "world"
```

```text
Saved data: "last line"
Returned line: "last line"
Remaining data: NULL
```

### Save Update

After a line has been extracted, the saved string is updated.

If characters remain after the newline, they are copied into a new saved string for the next call.

If there is nothing left after the returned line, the saved string is freed and reset to `NULL`.

This allows repeated calls to `get_next_line` to continue reading the same file descriptor line by line.

## Error Handling

The implementation handles the following cases:

- invalid file descriptor;
- invalid `BUFFER_SIZE`;
- failed memory allocation;
- failed `read()` call;
- empty file;
- end of file without a final newline.

Allocated memory is freed when it is no longer needed.

The returned line is dynamically allocated and must be freed by the caller.

## Limitations

This mandatory implementation uses one static saved string.

It is therefore designed to read one file descriptor at a time. Alternating calls between several file descriptors is not supported in this version.

Reading from binary files is considered outside the expected behavior of this project.

## Resources

- `man 2 read`
- `man 3 malloc`
- `man 3 free`
- Online documentation and articles about file descriptors, `read()`, and static variables
- 42 subject: Get Next Line
- Peer discussions and manual testing

## AI Usage

AI was used as a support tool during this project, mainly to clarify concepts, discuss possible approaches, and improve documentation.

It helped with:

- understanding the project requirements and the expected behavior of `get_next_line`;
- discussing high-level strategies for handling memory, errors, and edge cases;
- structuring and wording this README.

The submitted code was written, tested, and validated by the author.