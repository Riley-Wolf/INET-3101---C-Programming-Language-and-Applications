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
