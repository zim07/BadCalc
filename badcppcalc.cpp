#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <stack>
#include <cmath>

using namespace std;

// Function to check if a character is an operator (+, -, *, /)
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
// Side note, doesn't use floating point math so it will fuck up somewhere along the way
// Function to determine the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;  // Default precedence for other characters
}

// Function to perform arithmetic operations based on operator
double applyOperator(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b != 0)
            return a / b;
        else {
            cout << "Error: Division by zero" << endl;
            return NAN;  // Return NaN for error
        }
    default:
        cout << "Error: Invalid operator" << endl;
        return NAN;  // Return NaN for error
    }
}

// Function to evaluate an expression with parentheses and order of operations (trust it not)
double evaluateExpression(const string& expression) {
    stack<double> values;
    stack<char> operators;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (c == ' ')
            continue;  // Skip spaces

        if (isdigit(c) || c == '.') {
            // If the character is a digit or '.', parse the number and push to values stack
            double number;
            istringstream(expression.substr(i)) >> number;
            values.push(number);
            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                ++i;
            }
            --i; // Adjust i to the last digit of the number
        }
        else if (c == '(') {
            // If the character is an opening parenthesis, push it to operators stack
            operators.push(c);
        }
        else if (c == ')') {
            // If the character is a closing parenthesis, perform calculations until the matching '(' is found                (it wont be found)
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperator(a, b, op));
            }
            operators.pop(); // Pop the '('            (i forgot what this does just everything breaks if its gone)
        }
        else if (isOperator(c)) {
            // If the character is an operator, perform calculations based on precedence
            while (!operators.empty() && operators.top() != '(' && precedence(operators.top()) >= precedence(c)) {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperator(a, b, op));
            }
            operators.push(c);
        }
        else if (c == 's' || c == 'c' || c == 't') {
            // The slip up professional at trigonometric functions (sine, cosine, tangent)
            string trigFunction;
            while (isalpha(c)) {
                trigFunction += c;
                c = expression[++i];
            }
            if (c == '(') {
                double arg;
                if (trigFunction == "sin") {
                    // Calculate sine       (i also forgot to include the function that allows you to use trig, maybe? i dont remember)
                    while (expression[++i] != ')') {
                        string argStr;
                        while (isdigit(expression[i]) || expression[i] == '.') {
                            argStr += expression[i++];
                        }
                        istringstream(argStr) >> arg;
                    }
                    values.push(sin(arg));
                }
                else if (trigFunction == "cos") {
                    // Calculate cosine
                    while (expression[++i] != ')') {
                        string argStr;
                        while (isdigit(expression[i]) || expression[i] == '.') {
                            argStr += expression[i++];
                        }
                        istringstream(argStr) >> arg;
                    }
                    values.push(cos(arg));
                }
                else if (trigFunction == "tan") {
                    // Calculate tangent
                    while (expression[++i] != ')') {
                        string argStr;
                        while (isdigit(expression[i]) || expression[i] == '.') {
                            argStr += expression[i++];
                        }
                        istringstream(argStr) >> arg;
                    }
                    values.push(tan(arg));
                }
                else {
                    cout << "Error: Invalid trigonometric function" << endl;
                    return NAN;  // Return NaN for error
                }
            }
            else {
                cout << "Error: Invalid syntax for trigonometric function" << endl;
                return NAN;  // Return NaN for error
            }
        }
        else {
            cout << "Error: Invalid character in the expression" << endl;
            return NAN; // Return NaN for error
        }
    }

    // Perform remaining calculations
    while (!operators.empty()) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(applyOperator(a, b, op));
    }

    return values.top();
}

int main() {
    string equation;
    char reset;

    do {
        cout << "Simple Console Calculator: Now with Trig! (may or may not fuck up order of operations)" << endl; //it will fuck up oop
        cout << "Enter an equation (e.g., 3 + sin(0.5) * (4 - 2)): ";
        getline(cin, equation);

        double result = evaluateExpression(equation);

        if (!isnan(result)) {
            cout << "Result: " << result << endl;
        }

        cout << "Press space to do nothing and hit enter: "; //this works half of the time
        cin.ignore(); // Clear the newline character from previous input
        reset = cin.get();

        if (reset == ' ') {
            // Clear the console screen.         (it does work on my system, no where else though)
#ifdef _WIN32
            system("cls"); // Windows
#else
            system("clear"); // Linux/macOS (i swear this if else statement wont work)
#endif
        }

    } while (reset == ' ');

    return 0;  // Exit successfully (it acually crashes but that gets the job done right?)
}
