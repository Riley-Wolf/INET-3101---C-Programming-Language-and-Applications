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
