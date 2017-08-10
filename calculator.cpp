#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <vector>

void stack_operation(std::string s, std::stack<std::string> &token_stack);

void check_stack_empty(std::stack<std::string> &token_stack);

bool check_number_valid(std::string &num);

int main(int argc, char* argv[]) {

	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading.
	std::ifstream in;
	in.open(argv[1]);

	// string to be read into.
	std::string s;

    // stack to store the tokens.
    std::stack<std::string> token_stack;

    // vector for store tokens that need to be repeated.
    std::vector<std::string> repeat_part;

    // integer for store repeat number
    int repeat_time;

    // flag for check if in the part that need to be repeated.
    bool repeat_flag = false;
    
    // read the file while we have input.
	while (in >> s) {
        if (s == "repeat") {    // repeat command, get repeat number and set repeat flag to true.
            check_stack_empty(token_stack);
            if (token_stack.top().find('.') != std::string::npos) { // if repeat number is double, error will occur.
                std::cerr << "Repeat number cannot be double" << std::endl;
                exit(EXIT_FAILURE);
            }
            repeat_time = std::stoi(token_stack.top());
            token_stack.pop();
            if (repeat_time < 0) {  // if repeat number is negative, error will occur.
                std::cerr << "Repeat number cannot be negative" << std::endl;
                exit(EXIT_FAILURE);
            }
            repeat_flag = true;
        } else if (s == "endrepeat") {  // 'endrepeat' command
            repeat_flag = false;
            while (repeat_time-- > 0) { // repeat until 'repeat_time' reach to 0
                // for every tokens in repeat_part, call function to do operation with stack.
                for (auto i : repeat_part)
                    stack_operation(i, token_stack);
            }
            repeat_part.clear();
        } else {
            // check if repeat flag is set
            if (repeat_flag)
                repeat_part.push_back(s);   // if flag is set push token into vector 'repeat_part'
            else
                stack_operation(s, token_stack);  // otherwise call function to do operation with stack.
        }
	}
	in.close();
    return 0;
}

/* Function for tokens except 'repeat' and 'endrepeat', 2 parameters
 *         string s : store the token, may be number or command (except 'repeat', 'endrepeat')
 *         stack token_stack : the stack that store the tokens for calculation. */
void stack_operation(std::string s, std::stack<std::string> &token_stack) {
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
    if (check_number_valid(s))
        token_stack.push(s);
    else {
        // handle the situation of 'add', 'sub', 'mult', 'div'
        if (s == "add" || s == "sub" || s == "mult" || s == "div") {
            // get two numbers from stack, store in 'x', 'y', before get numbers, check if stack is empty.
            check_stack_empty(token_stack);
            x = token_stack.top();
            token_stack.pop();
            check_stack_empty(token_stack);
            y = token_stack.top();
            token_stack.pop();

            if (x.find('.') != std::string::npos || y.find('.') != std::string::npos) {
                // if at least one number is double, first check if 'x' is integer or double, output 'x'
                // next do calculation depends on command, output symbol(+-*/) and store double result in 'z_double',
                // then check if 'y' is int or double, output 'y = z_double', finally push result into stack.
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
                        exit(EXIT_FAILURE);
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
                // if both x and y are integers, output 'x' first, do calculation depends on command
                // then output symbol(+-*/) and store integer result in 'z_int'
                // next output 'y = z_int', finally push result into stack.
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
                        exit(EXIT_FAILURE);
                    }
                    std::cout << " / ";
                    z_int = std::stoi(x) / std::stoi(y);
                }
                std::cout << std::stoi(y) << " = " << z_int << std::endl;
                token_stack.push(std::to_string(z_int));
            }
        } else if (s == "sqrt") {   // sqrt command
            // get number and store in x, check if x is integer or double, calculate sqrt(x)
            // output calculation and store result back to stack
            check_stack_empty(token_stack);
            x = token_stack.top();
            token_stack.pop();
            if (x.find('-') != std::string::npos) { // if number is negative, error will occur
                std::cerr << "Sqrt number cannot be negative" << std::endl;
                exit(EXIT_FAILURE);
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
        } else if (s == "pop") {    // pop command, pop the token at top
            check_stack_empty(token_stack);
            token_stack.pop();
        } else if (s == "reverse") {    // reverse command
            check_stack_empty(token_stack);
            if (token_stack.top().find('.') != std::string::npos) { // double number will occur error
                std::cerr << "Reverse number cannot be double" << std::endl;
                exit(EXIT_FAILURE);
            }
            // get the number and store in reverse_n, next reverse_n tokens will be reversed in stack
            reverse_n = std::stoi(token_stack.top());
            token_stack.pop();
            if (reverse_n < 0) {    // negative number will occur error
                std::cerr << "Reverse number cannot be negative" << std::endl;
                exit(EXIT_FAILURE);
            }
            // get next reverse_n tokens, push back in vector 'reverse_vector' in order.
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
            // the token is not a number or a command, error will occur.
            std::cerr << "Invalid input \"" << s << "\"" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Function for check if stack is empty, if it is empty, error will occur.
void check_stack_empty(std::stack<std::string> &token_stack) {
    if (token_stack.empty()) {
        std::cerr << "Stack is empty" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function for check if number is valid
bool check_number_valid(std::string &num) {

    // for check if a '.' already exists.
    bool point_flag = false;

    // If first character is digits or is '-' and size greater than 1, do next process. If not, return false
    if (isdigit(num[0]) || (num[0] == '-' && num.size() > 1)) {
        // check other character
        for (auto i = num.begin() + 1; i != num.end(); ++i) {
            //if it is not number
            if (!isdigit(*i)) {
                // if it is the '.' and occur first time, set point_flag to true.
                if (*i == '.' && point_flag == false)
                    point_flag = true;
                else
                    return false;   // return false if any character is valid.
            }
        }
        return true;    // return true if all characters are valid.
    } else
        return false;
}