# Writeup 6 - Binaries I

Name: George Gaynor
Section: 0101

I pledge on my honor that I have not given or received any unauthorized assistance on this assignment or examination.

Digital acknowledgement: George Gaynor

## Assignment Writeup

### Part 1 (50 pts)

*Please use this space to provide flag from program*

### Part 2 (50 pts)

*Please use this space to detail your approach and solutions for part 2. Include
descriptions of checks implemented as well as your final input to produce flag.*

The program read user input by running the crackme executable and passing parameters into it. To impact control flow, different parameters were passed into the executable file. The purpose of each check was to check the parameters passed into the program and direct the control flow based on the parameters. The first check compared the parameter to the string "Oh God". If only one parameter was passed to the function and the string was not equal to "Oh God", the string "Multi-word arguments can be quoted ;)". If the string was equal to "Oh God", then the program continued execution. The second check called the c function getenv() and checked for an environment variable passed to the program. If the parameter provided was "Oh God" but no environment variable was provided, the program returned "I wish you cared more about the environment."
