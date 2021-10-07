# Interpretator for math calculations

Here you can find my implementation of a simple programming language interpreter for math calculations. 

It supports:
1) Variables;
2) Expressions with all standard arithmetic operations(`+`, `-`, `*`, `/`, `%`, `^`), brackets, unary minus, variables
3) `while` loops
4) `print(<Expression>)` function
5) `if` statment

All calculations are performed in double numbers. 
# How to use
  You need to run main.c file and input the absolute path to the file you want to process. The processed syntax is close to C/C++, with precision to small simplifications. Examples of files you can find in the tests directory. 
  
  After that, you will receive the output of your program, the final values of all variables, or an error message if the file is incorrect.

  
# Expressions
Programm can work with correct algebraic expressions, which contains names of known variables, double numbers, brackets, and binary operations: plus(`+`), minus(`-`), multiply(`*`), dividing(`/`), getting remainder modulo(`%`), exponentiation(`^`) and unary operation minus(`-`). Expression is parsed taking into account the generally accepted concepts of associativity and priority of operations, you can read about it here https://en.wikipedia.org/wiki/Order_of_operations
 
# Scanner 
Just right after reading the file, you need to break the text given to us into tokens so that you can interact with it in the future. 
Also here comes the first error output if some unrecognized characters appears in the text
  
# Parser
 
To interpret the expression, we need to convert the list of tokens into reverse polish notation. If an expression is incorrect or some unavailable commands were met, the parser will throw an error.
  
# Virtual machine
 Now we can simulate the working of the program, by calculating all expression's values and doing needed actions with them
  
  

  

