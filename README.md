Aims:

Familiarity with C++ Containers
File and string processing
Write a stack-based calculator program in C++. The calculator reads tokens (numbers and commands) from a specified input file. In the input file each token is separated by a space or a new line. Each number is placed on a stack and each command consumes one or more values from the stack. A command may result in a modification of the stack, an additional number added to the stack, and/or output printed to the console (stdout / cout). The calculator should support int and double values and only promote an int to a double when necessary. Your program should be written in pure C++ (not C) and should make use of the C++ STL. When printing output your double values should be formatted to three decimal places. Your calculator should act on the following tokens:

Token	Description	Console Output
x.y	A double (where x and y are numbers). All doubles will have decimal places. e.g., 2.1	 
x	An integer (where x is a number).	 
add	Removes two numbers from the stack and calculates their sum. The sum is pushed back onto the stack.	x + y = z
sub	Removes two numbers from the stack and subtracts the second number removed from the first. The result is pushed back onto the stack.	x - y = z
mult	Removes two numbers from the stack and multiplies their product. The result is pushed back onto the stack.	x * y = z
div	Removes two numbers from the stack and divides the first by the second. The result is pushed back onto the stack.	x / y = z
sqrt	Removes the top of the stack and calculates the sqrt of this number. Pushes the result back onto the stack.	sqrt y = z
pop	Removes the top of the stack.	 
reverse	Removes the top of the stack. The ordering of the next n (the number that was at the top of the stack) elements are reversed on the stack.	 
repeat	Removes the top of the stack. Numbers and commands continue to be read from the file but not acted on until an endrepeat command.	 
endrepeat	Processes the numbers and commands that were read from the input file but stored from the start of the repeat command. These numbers and commands are repeated n times (where n is the number that was at the top of the stack when the repeat command was given).	 
Sample input:

10 20 sub
4 3 add 
2 mult
16.0 
2 repeat
sqrt
endrepeat
pop
2.0
mult
3 repeat
2 
2 reverse
div
3 mult
endrepeat
Sample output:

20 - 10 = 10
3 + 4 = 7
2 * 7 = 14
sqrt 16.000 = 4.000
sqrt 4.000 = 2.000
2.000 * 14 = 28.000
28.000 / 2 = 14.000
3 * 14.000 = 42.000
42.000 / 2 = 21.000
3 * 21.000 = 63.000
63.000 / 2 = 31.500
3 * 31.500 = 94.500
