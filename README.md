INET 3101: C Programming Language and Applications README.md
- This repository is used for uploading and viewing assignments for my INET 3101 class.


Week 1: myname.c
This is the first assignment for this class. It consists of a simple code that prints out the phrase, "My name is Riley".

Week 2: basen.c
This code is supposed to convert a positive integer into any base between 2 and 16.
BUG #1: Missing bounds check
  - Concept: The bounds are missing in this code, meaning that "base" can be any number.
  - Solution: This was a simple fix. All I did was write in an "if" statement that would throw an error if "base" was lower than 2 or higher than 16.
BUG #2: Digits >= 10 print as numbers
  - Concept: This is an issue with trying to convert digits to characters based what "base" they are. 0-9 returns the digit, 10-15 returns the character associated with said digit (i.e. 10=a, 11=b, 12=c, ...).
  - Solution: I did less fixing and more adding onto what was already here. Basically, if the remainder of "num % base" is >= 10, that indicates the digit is between 10-15. The code then converts the formatting to a-f (%c). Otherwise, it prints as the digit 0-9 (%d).
BUG #3: Prefix formatting
  - Concept: This is for figuring how formatting should work for the bases.
  - Solution: If "base = 8", it will print as g0. If "base = 16", it will print as g0x. Everything else prints nothing, since they aren't octal or hexadecimal.
BUG #4: Edge cases
  - Concept: This is for making sure everything is covered when entering a number. DOES NOT CATCH NEGATIVE NUMBERS.
  - Solution: Pretty self-explanatory. If "num" equals 0, then it returns 0.
The star of this code is first_call. I used this variable as a flag to indicate whether the input was in the original call or a recursive call. This helps keep track of formatting between octal and hexadecimal bases.
Call-Stack Tracing:
  - Initial Call: to_base_n(129, 16)
  - Variables: num = 129, base = 16, r = 129 % 16 = 1
  - Recursion: Since 129 >= 16, it recursively calls to_base_n(8, 16)
    - Second Call: to_base_n(8, 16)
    - Variables: num = 8, base = 16, r = 8 % 16 = 8
    - Recursion: Since 8 < 16, recursion stops here
    - Printing: to_base_n(8, 16) prints 8, then to_base_n(129, 16) prints 1
AI Tool Reflection:
  - I used ChatGPT as my helper for this assignment. The best suggestion ChatGPT made was to implement first_call. I could not seem to get the code working until that feature was added. ChatGPT did miss the mark early on by trying to have me use fprintf(stderr, "Error: base must be between 2 and 16.\n"); for my error message.
 
Week 3:
## Problem & Solution Summary
I wrote an airline seat assignment system in C from scratch. The program manages two separate flights, an outbound flight and an inbound flight, with 24 seats available on each flight.

A structure `struct seat` stores the information for each seat, including the seat number, assignment status, passenger first name, and passenger last name. Two separate arrays, `outbound_seats[24]` and `inbound_seats[24]`, keep the seats for each flight separate.

The program uses a first-level menu to select either the outbound or inbound flight. Each flight has its own menu for counting empty seats, listing empty seats, displaying assigned passengers alphabetically, assigning customers, deleting assignments, and returning to the main menu.

## Input Stream & Buffer Analysis
The program uses different input methods depending on the type of information being entered. Menu choices use `scanf(" %c", &choice)`. The space before `%c` skips whitespace and leftover newlines from previous input.

Seat numbers use `scanf("%d", &seat)` and the return value is checked to detect invalid input. Passenger names use `%49[^\n]`, which allows names to contain spaces while limiting the input to 49 characters so the 50-character array is not exceeded.

The original program used:

```c
while (getchar() != '\n');
```

to clear unwanted input. This caused a problem during automated testing because `getchar()` can return `EOF` when the input file reaches its end. The loop could then continue indefinitely.

The code was changed to:

```c
while ((ch = getchar()) != '\n' && ch != EOF);
```

This clears the remaining input until either a newline or `EOF` is reached, preventing the program from getting stuck.

## AI Test Harness Evaluation
An AI-generated `test_input.txt` file was used to automatically test the program. The test included invalid menu choices, assigning seats, attempting to assign occupied seats, deleting assignments, cancelling with `-1`, invalid seat numbers, long names, names containing spaces, alphabetical sorting, and testing both flights.

The automated test uncovered two major problems. First, the original `getchar()` loop could enter an infinite loop at `EOF`, which caused the output file to grow to several gigabytes.

Second, the original passenger name input used:

```c
%*49[^\n]
```

The `*` suppresses assignment, so the name was read but not actually stored in the passenger's struct. This was changed to:

```c
%49[^\n]
```

After these changes, the test harness confirmed that seat counting, assignment, deletion, input validation, alphabetical sorting, and navigation between the flight menus worked correctly.

I also used AI to help me format this README better, since I'm unhappy about the look of the previous two weeks.

Week 4:
# Airline Seat Assignment System

## Problem Statement & Persistence Design

The airline seat assignment system manages two flights, with 24 seats on the outbound flight and 24 seats on the inbound flight. Each seat is represented using a `struct seat` containing an ID number, assignment status, passenger first name, and passenger last name.

The program uses binary file persistence through a file named `flight_data.bin`. The 24 outbound seat structures are written to the file first, followed by the 24 inbound seat structures. The program uses `fwrite()` to save the structures and `fread()` to load them. Since each structure has a fixed size, the expected file size can be calculated as 48 multiplied by `sizeof(struct seat)`.

The program also uses a temporary file when saving. Data is first written to `flight_data.tmp`. After the writes, the program flushes and closes the file successfully before replacing the existing `flight_data.bin`. This helps prevent a failed save operation from destroying an existing valid data file.

When the program starts, it attempts to load the saved data. If the file does not exist or contains invalid data, the program initializes all seats as empty instead of continuing with potentially corrupted information.

## File Validation & Error Recovery Analysis

The program performs several checks when loading the binary file to prevent corrupted data from being used.

First, `fopen()` checks whether the file can be opened. If it cannot, the program initializes the seats and reports an error. The program then uses `fseek()` and `ftell()` to determine the file size. The file must contain exactly 48 `struct seat` records. This detects files that are truncated or contain unexpected extra data.

The program also checks the return value of `fread()`. The outbound and inbound arrays each require exactly 24 structures to be read. If `fread()` returns fewer than 24, the program treats this as an incomplete read or unexpected end-of-file. The file is closed and the seat data is reinitialized.

After successfully reading the structures, the program validates the contents of each record. Each seat ID must match its expected value from 1 through 24. The `assignment_status` must also be either `0` for an empty seat or `1` for an assigned seat.

For assigned seats, the program calls `valid_name()` to validate the passenger first and last names. This prevents corrupted binary data containing non-printable characters from being accepted as a passenger name.

When any validation fails, the program closes the file, reinitializes the seat arrays, and returns without using the corrupted data. This provides an error recovery process instead of allowing invalid information to remain in memory.

## Pros & Cons of Solution

### Advantages of Binary Serialization

* Binary files are compact and efficient.
* `fread()` and `fwrite()` make saving and loading the structures relatively simple.
* The fixed structure makes it easy to calculate the expected file size.
* The program can save and restore the complete seat state without manually formatting every field.
* Binary data does not require parsing strings separated by commas or other delimiters.
* The temporary-file save process reduces the chance of losing a valid data file because of a failed save.

### Disadvantages of Binary Serialization

* Binary files are not human-readable.
* The file depends on the current layout and size of the C `struct`.
* Changing the structure in a future version could make older binary files incompatible.
* Corrupted binary data is more difficult to inspect manually than a text file.
* Binary files are less portable between systems or programs that use different structure layouts.

An alternative would be formatted ASCII text, such as CSV. A CSV file would be easier for a person to open and inspect, and it would be more portable between programs. However, text I/O would require additional code to format each record when saving and parse the records when loading. For this project, binary serialization is a practical choice because the program has a fixed number of records and a simple structure.

## AI Fuzzing Reflection

I used AI to help design a Python-based file fuzzer for testing the persistence and validation portions of the C program. The prompt requested a fuzzer that would create maliciously corrupted binary files and test different types of invalid data.

The fuzzer was designed to generate several different test cases:

* `corrupt_truncated.bin` — contains only the first 10 seat records instead of all 48.
* `corrupt_oversized.bin` — contains extra bytes after the expected 48 seat records.
* `corrupt_garbage_name.bin` — inserts non-printable binary values into a passenger name field.
* `corrupt_seat_number.bin` — changes a seat ID to an invalid value such as `999`.
* `corrupt_status.bin` — changes an assignment status to an invalid value such as `999`.
* `corrupt_random.bin` — randomly changes bytes throughout the binary file.

The corrupted files were tested by copying each one over `flight_data.bin` and running the C program. The truncated and oversized files were rejected by the file-size validation. The invalid seat number and invalid assignment status files were rejected by the corresponding structure validation checks. The garbage-name file was also detected once the corrupted seat contained an assigned passenger, allowing the passenger-name validation code to run.

The fuzzing process demonstrated that successfully reading a binary file does not necessarily mean the data inside the file is valid. The original structures could be read successfully even when individual fields had been corrupted. This is why the final program performs validation after `fread()` instead of trusting the loaded structures.

The testing also helped verify the program's error recovery. When invalid data is detected, the program closes the file, reinitializes the seat arrays, and avoids continuing with corrupted passenger or seat information. The fuzzer therefore provided a way to test cases that would be difficult to reproduce through normal menu input alone.

Week 5:
# Socket File Server

## Problem Statement & Architecture

This project implements a simple TCP client/server system in C. The client connects to the server on TCP port 8080, asks the user for a filename, and sends the filename to the server. The server receives the request, validates the filename, opens the requested file, and sends the file contents back to the client. If the file cannot be found or the request is invalid, the server sends an error message instead.

The communication uses TCP sockets because TCP provides reliable, ordered delivery between the client and server. The protocol is intentionally simple: the client sends one filename request, and the server sends either the requested file contents or an error message. The server uses port 8080 and listens for incoming connections on all local interfaces. The client connects to `127.0.0.1`, so testing is performed locally on the same computer.

The server processes clients iteratively. It accepts one connection, processes the request, closes that client socket, and then waits for the next connection.

## Socket Security & Buffer Analysis

TCP is a byte-stream protocol rather than a message-based protocol. The server therefore uses `recv()` to read the filename into a fixed-size buffer. The receive buffer is limited to `sizeof(filename) - 1`, leaving space for an explicit null terminator. After `recv()` returns, the server places `'\0'` at `filename[bytes_received]` so that the received data can safely be treated as a C string.

The server also checks the return value of `recv()`. A value of zero or less causes the connection to be closed without attempting to process invalid data. Requests that fill the receive buffer are rejected as filenames that are too long. This prevents a large input such as the 10,000-byte fuzzer payload from overflowing the filename array.

The server sanitizes filenames before calling `fopen()`. It rejects filenames containing `..`, `/`, or `.`. This prevents common path traversal requests such as `../../etc/passwd` and absolute paths such as `/etc/passwd`. The server also rejects empty filenames and detects embedded null bytes before opening a file.

File contents are limited to 255 bytes. The server checks the file size with `fseek()` and `ftell()` before reading it. The file data is stored in a 256-byte buffer, allowing one additional byte for a null terminator. Socket and file descriptors are closed on error paths as well as after successful requests.

The protocol remains intentionally simple. Because TCP does not preserve application-level message boundaries, a production protocol would normally use explicit framing, such as a length field or delimiter. This implementation relies on the filename request being short enough to be received in one `recv()` call, which is acceptable for this small assignment but is a limitation of the design.

## Pros & Cons of Solution

One advantage of the solution is its simplicity. The server uses standard C socket functions and processes one client at a time, making the program easier to understand, compile, test, and debug. The iterative design also avoids the additional complexity of threads, processes, synchronization, and shared resources.

The security checks provide protection against the specific malformed inputs tested by the fuzzer. Fixed-size buffers and explicit null termination reduce the risk of traditional C string buffer overflows. Filename validation also prevents the server from intentionally opening files outside the server's working directory through common path traversal techniques.

The main disadvantage is that the server is iterative rather than concurrent. While one client is being processed, another client must wait. A multithreaded or fork-based server could process multiple clients at the same time, but that would add complexity and require additional resource and error management.

Another limitation is the simple TCP protocol. The server assumes the short filename request can be handled by a single `recv()` operation. TCP can split or combine data arbitrarily, so a more robust implementation would define explicit request framing and continue receiving until the complete request has been received.

## AI Fuzzing Reflection

The network fuzzer was created with the following prompt:

> "Write a test script named fuzzer_2.py designed to craft malicious raw TCP socket payloads against your server. The fuzzer must simulate path traversal attacks, buffer overflow/payload flooding by sending 10,000 characters without a newline or null terminator, abrupt disconnections before sending data or during transmission, and null byte injection inside requested file strings."

The fuzzer was then refined to retest the hardened server after the C code was refactored. It sends path traversal requests such as `../../etc/passwd`, an absolute path, a 10,000-byte payload, an immediate connection teardown, a mid-transmission teardown, and a filename containing a binary null byte.

The initial simplified server did not perform explicit path sanitization, so it could potentially attempt to open paths supplied by the client. It also did not explicitly reject a payload that filled the receive buffer. These weaknesses were addressed by adding filename validation, checking the number of bytes returned by `recv()`, explicitly null-terminating received data, rejecting oversized requests, detecting null-byte injection, and closing sockets on error paths.

The final fuzzer did not require a memory crash to demonstrate that the hardening worked. The important result was that malicious requests were rejected without crashing or hanging the server, while normal requests such as `solution.c` continued to work.

## Expected Fuzzer Output

With the hardened server running, the fuzzer should produce results similar to:

```yaml
--- Path Traversal ---
Response: Error: Invalid filename.

--- Absolute Path ---
Response: Error: Invalid filename.

--- 10,000-Byte Payload Flood ---
Response: Error: Filename is too long.

--- Abrupt Disconnect: Before Sending ---
Connected and closed without sending data.

--- Abrupt Disconnect: During Transmission ---
Sent partial payload and closed connection.

--- Null Byte Injection ---
Response: Error: Invalid filename.

Fuzzing complete.

The exact order of messages may vary slightly depending on the operating system and timing, but the important result is that the server remains running and does not crash when receiving the malicious payloads.

Week 6:
# GNU Coreutils 9.7: `ls.c` and `whoami.c`

## 1. Problem Statement & Solution Overview

The goal was to modify two GNU Coreutils 9.7 utilities:

- **`ls.c`:** Change the default output to long format (`ls -l`) when no arguments are supplied.
- **`whoami.c`:** Add the prefix `You are: ` before the current username.

After modifying both C source files, GNU Coreutils was rebuilt, and the resulting binaries were tested.

## 2. C System Mechanics & Diff Analysis

### `ls.c`

The format-selection logic in `decode_switches()` was modified to select `long_format` by default in normal `ls` mode.

Relevant variables:
- `format`: Selected listing format.
- `format_opt`: Tracks explicitly selected format options.
- `ls_mode`: Determines the utility's operating mode.
- `long_format`: Enables long-format output.

Explicit command-line options should continue to override the default.

### `whoami.c`

The utility uses `getuid()` and `getpwuid()` to retrieve the current user's identity and username. The output formatting was changed to include the `You are: ` prefix while retaining dynamic username lookup.

### Unified Git Diff

Generate the exact patch using:

```bash
git diff -- src/ls.c src/whoami.c
```

Include the resulting output here to document the actual source changes.

## 3. Pros & Cons

**Pros:**
- Makes detailed file information available by default.
- Adds a descriptive prefix to `whoami` output.
- Requires only small changes to existing code.

**Cons:**
- Changing default `ls` output may break scripts expecting standard short listings.
- The `whoami` prefix may break scripts expecting only the username.
- Future Coreutils updates may require reapplying the changes.

## 4. AI Audit Reflection

AI assistance helped locate the relevant source code, understand how `ls` uses format-selection variables and command-line options, and identify how `whoami` retrieves usernames.

The changes should be verified by rebuilding Coreutils, testing both utilities, checking explicit `ls` options, and reviewing the unified Git diff. Only tests actually performed should be reported as completed.

## 5. Conclusion

This assignment demonstrates how small C source modifications can change command-line utility behavior. Rebuilding, testing, and reviewing the Git diff help verify and document the changes.
