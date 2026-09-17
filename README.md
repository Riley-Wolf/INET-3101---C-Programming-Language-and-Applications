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
  
