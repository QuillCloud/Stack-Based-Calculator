#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <vector>

void calculate_process(std::string s, std::stack<std::string> &token_stack);

void check_stack(std::stack<std::string> &token_stack);

bool check_number_valid(std::string &num);

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

    std::vector<std::string> repeat_part;

    int repeat_time;

    bool repeat_flag = false;
    
    // read the file while we have input.
	while (in >> s) {
        if (s == "repeat") {
            check_stack(token_stack);
            repeat_time = std::stoi(token_stack.top());
            token_stack.pop();
            repeat_flag = true;
        } else if (s == "endrepeat") {
            repeat_flag = false;
            while (repeat_time-- > 0) {
                for (auto i : repeat_part)
                    calculate_process(i, token_stack);
            }
            repeat_part.clear();
        } else {
            if (repeat_flag)
                repeat_part.push_back(s);
            else
                calculate_process(s, token_stack);
        }
	}
	in.close();
    return 0;
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
    if (check_number_valid(s))
        token_stack.push(s);
    else {
        // handle the situation of 'add', 'sub', 'mult', 'div'
        if (s == "add" || s == "sub" || s == "mult" || s == "div") {
            // get two numbers from stack, store in 'x', 'y', before get second number, check if stack is empty.
            check_stack(token_stack);
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
                        exit(EXIT_FAILURE);
                    }
                    std::cout << " / ";
                    z_int = std::stoi(x) / std::stoi(y);
                }
                std::cout << std::stoi(y) << " = " << z_int << std::endl;
                token_stack.push(std::to_string(z_int));
            }
        } else if (s == "sqrt") {
            check_stack(token_stack);
            // get number store in x, check if x is integer or double, calculate sqrt(x)
            // output calculation and store result back to stack
            x = token_stack.top();
            token_stack.pop();
            if (x.find('-') != std::string::npos) { // if number is negative, error will occur
                std::cerr << "sqrt number is negative" << std::endl;
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
        } else if (s == "pop") {// pop the token
            check_stack(token_stack);
            token_stack.pop();
        } else if (s == "reverse") {
            check_stack(token_stack);
            // get the number and store in reverse_n, next reverse_n elements will be reversed in stack
            reverse_n = std::stoi(token_stack.top());
            token_stack.pop();
            if (reverse_n < 0) { // negative number will occur error
                std::cerr << "reverse number is negative" << std::endl;
                exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
    }
}

// Function for check if stack is empty
void check_stack(std::stack<std::string> &token_stack) {
    if (token_stack.empty()) {
        std::cerr << "Stack is empty" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool check_number_valid(std::string &num) {
    bool point_flag = false;
    if (isdigit(num[0]) || num[0] == '-') {
        for (auto i = num.begin() + 1; i != num.end(); ++i) {
            if (!isdigit(*i)) {
                if (*i == '.' && point_flag == false)
                    point_flag = true;
                else
                    return false;
            }
        }
        return true;
    } else
        return false;
}