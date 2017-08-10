#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <vector>

void stream_process(int repeat_time,std::vector<std::string> stream, std::stack<std::string> &token_stack);

void calculate_process(std::string s, std::stack<std::string> &token_stack);

void check_stack(std::stack<std::string> &token_stack);

int main(int argc, char* argv[]) {

	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading
	std::ifstream in;
	in.open(argv[1]);

	// string to be read into
	std::string s;

    // stack to store the tokens
    std::stack<std::string> token_stack;

    // vector to store all of the input stream
    std::vector<std::string> input_stream;
    
    // read the file while we have input.
	while (in >> s) {
        input_stream.push_back(s);   // put the read string into vector
	}
    stream_process(1, input_stream, token_stack); // call function to do the process
	in.close();
    return 0;
}

/* Function for input stream process, 3 parameters:
 *      integer 'reapt_time' : how many times need the stream be repeated
 *      vector 'stream' : may be whole input stream or part of stream that need to be repeated
 *      stack 'token_stack' : store the tokens for calculating
 * If meet 'repeat' command, create new stream vector to store the part that need be repeated,
 * then call itself to do recursion, otherwise call function 'calculate_process' to do some operations. */
void stream_process(int repeat_time,std::vector<std::string> stream, std::stack<std::string> &token_stack) {
    auto repeat_count = 0;   // use for matching the repeat with corresponding endrepeat
    
    int repeat_time_local = 0;  // store the repeated times if meet 'repeat' command in stream.
    
    bool repeat_flag = false;   // flag for check if already meet a 'repeat' command
    
    std::vector<std::string> stream_repeat_part;  // vector to store repeat content

    // stream will be repeated until 'repeated_time' reach 0.
    while (repeat_time-- > 0) {
        // read the stream, store the string in 's'
        for (auto s : stream) {
            // if meet 'repeat' command
            if (s == "repeat") {
                // if 'repeat' occur more than once, count it and then push 'repeat' into new stream vector.
                if (repeat_flag) {      
                    ++repeat_count;
                    stream_repeat_part.push_back(s);
                } else {
                    // if it is the first repeat in stream, get repeat time, clear the new stream vector, set a flag
                    check_stack(token_stack);
                    repeat_time_local = std::stoi(token_stack.top());
                    token_stack.pop();
                    repeat_flag = true;
                    stream_repeat_part.clear();
                }
            } else if (s == "endrepeat") { // if meet 'endrepeat' command
                // if this 'endrepeat' match the first 'repeat', call function stream_process to do recursion.
                if (repeat_count == 0) {
                    stream_process(repeat_time_local, stream_repeat_part, token_stack);
                    repeat_flag = false;
                } else {
                    // if it is not match, minus repeat count by 1, then push 'endrepeat' into new stream vector.
                    --repeat_count;
                    stream_repeat_part.push_back(s);
                }
            }
            else {
                // if not 'repeat' or 'endrepeat' command
                // if repeat flag is set, push token into new stream vector
                // otherwise, call function 'calculate_process' to do some operation
                if (repeat_flag)
                    stream_repeat_part.push_back(s);
                else
                    calculate_process(s, token_stack);
            }
        }
    }

}

/* Function for tokens excpet 'repeat' and 'endrepeat', 2 parameters
 *         string s : store the token, may be number or command (except 'repeat', 'endrepeat')
 *         stack token_stack : the stack that store the tokens for calculation. */
void calculate_process(std::string s, std::stack<std::string> &token_stack) {
    // x, y for get number in stack
    std::string x, y;

    // 'reverse_n' store the number for reverse
    // 'z_int' store the integer calculation result
    int reverse_n, z_int;

    // store the double calculation result
    double z_double;

    // the vector used for 'reverse' command
    std::vector<std::string> reverse_vector;

    // detect if it is a number, if so, push it into stack
    if (isdigit(s[0]) || (s[0] == '-' && isdigit(s[1])))
        token_stack.push(s);
    else {
        // All command in this part need at least 1 number in stack, so check if stack is empty.
        check_stack(token_stack);
        // handle the situation of 'add', 'sub', 'mult', 'div'
        if (s == "add" || s == "sub" || s == "mult" || s == "div") {
            // get two numbers from stack, store in 'x', 'y', before get second number, check if stack is empty.
            x = token_stack.top();
            token_stack.pop();
            check_stack(token_stack);
            y = token_stack.top();
            token_stack.pop();
            // if at least one number is double, first check if 'x' is int or double, output 'x'
            // next do calculation depends on command, output symbol(+-*/) and store double result in 'z_double',
            // then check if 'y' is int or double, output 'y = z_double', finally push result into stack.
            if (x.find('.') != std::string::npos || y.find('.') != std::string::npos) {
                if (x.find('.') != std::string::npos)
                    std::cout << std::stod(x);
                else
                    std::cout << std::stoi(x);
                if (s == "add") {
                    std::cout << " + ";
                    z_double = std::stod(x) + std::stod(y);
                } else if (s == "sub") {
                    std::cout << " - ";
                    z_double = std::stod(x) - std::stod(y);
                } else if (s == "mult") {
                    std::cout << " * ";
                    z_double = std::stod(x) * std::stod(y);
                } else {
                    if (std::stod(y) == 0) { // divided by 0 will occur error.
                        std::cerr << std::endl << "Can not be divided by zero" << std::endl;
                        exit(0);
                    }
                    std::cout << " / ";
                    z_double = std::stod(x) / std::stod(y);
                }
                if (y.find('.') != std::string::npos)
                    std::cout << std::stod(y);
                else
                    std::cout << std::stoi(y);
                std::cout<< " = " << z_double << std::endl;
                token_stack.push(std::to_string(z_double));
            } else {
                // if both x and y are integers, ouput 'x' do calculation depends on command, output symbol(+-*/)
                // and store integer result in 'z_int', output 'y = z_int', finally push result into stack.
                std::cout << std::stoi(x);
                if (s == "add") {
                    std::cout << " + ";
                    z_int = std::stoi(x) + std::stoi(y);
                } else if (s == "sub") {
                    std::cout << " - ";
                    z_int = std::stoi(x) - std::stoi(y);
                } else if (s == "mult") {
                    std::cout << " * ";
                    z_int = std::stoi(x) * std::stoi(y);
                } else {
                    if (std::stoi(y) == 0) { // divided by 0 will occur error.
                        std::cerr << std::endl << "Can not be divided by zero" << std::endl;
                        exit(0);
                    }
                    std::cout << " / ";
                    z_int = std::stoi(x) / std::stoi(y);
                }
                std::cout << std::stoi(y) << " = " << z_int << std::endl;
                token_stack.push(std::to_string(z_int));
            }
        } else if (s == "sqrt") {
            // get number store in x, check if x is integer or double, calculate sqrt(x)
            // output calculation and store result back to stack
            x = token_stack.top();
            token_stack.pop();
            if (x.find('-') != std::string::npos) { // if number is negative, error will occur
                std::cerr << "sqrt number is negative" << std::endl;
                exit(0);
            }
            if (x.find('.') != std::string::npos) { // if x is double, store result in 'z_double'
                z_double = sqrt(std::stod(x));
                std::cout << "sqrt " << std::stod(x) << " = " << z_double << std::endl;
                token_stack.push(std::to_string(z_double));
            }
            else {  // if x is integer, store result in 'z_int'
                z_int = static_cast<int>(sqrt(std::stoi(x)));
                std::cout << "sqrt " << std::stoi(x) << " = " << z_int << std::endl;
                token_stack.push(std::to_string(z_int));
            }
        } else if (s == "pop") // pop the token
            token_stack.pop();
        else if (s == "reverse") {
            // get the number and store in reverse_n, next reverse_n elements will be reversed in stack
            reverse_n = std::stoi(token_stack.top());
            token_stack.pop();
            if (reverse_n < 0) { // negative number will occur error
                std::cerr << "reverse number is negative" << std::endl;
                exit(0);
            }
            // get next reverse_n elements, push back in vector 'reverse_vector' in order.
            while (reverse_n-- > 0) {
                x = token_stack.top();
                token_stack.pop();
                reverse_vector.push_back(x);
            }
            // push elements in vector 'reverse_vector' back to stack in reversed order.
            for (auto i : reverse_vector)
                token_stack.push(i);
        }
        else {
            // the token is not a number or a command, error will occur
            std::cerr << "Invalid input " << s << std::endl;
            exit(0);
        }
    }
}

// Function for check if stack is empty
void check_stack(std::stack<std::string> &token_stack) {
    if (token_stack.empty()) {
        std::cerr << "Stack is empty" << std::endl;
        exit(0);
    }
}