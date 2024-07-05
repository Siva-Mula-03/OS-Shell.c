## Problem statement: (Operating Systems)

### Write your own command shell using OS system calls to execute built-in Linux commands.

Your command shell should support below listed features.

### 1. Basic stuff
The shell should run an infinite loop (which will only exit with the ‘exit’ command) and 
interactively process user commands.

- The shell should print a prompt that indicate the current 
working directory followed by ‘$’ character.

- For reading the complete input line, use getline() function.
- For separating the multiple words (in case of multiple commands or command with multiple arguments) from the input
line, use strsep() function. 

To execute the commands with a new process, use fork, exec and wait system calls.

### 2. Changing directory
Your shell should support ‘cd’ command. The command ‘cd <directoryPath>’ should 
change its working directory to directoryPath and ‘cd ..’ should change the working 
directory to the parent directory. You may use chdir() system call for implementing this. 

### 3.Incorrect command
An incorrect command format (which your shell is unable to process) should print an error 
message ‘Shell: Incorrect command’ (do not change this message). If your shell is able to 
execute the command, but the execution results in error messages generation, those error 
messages must be displayed to the terminal. An empty command should simply cause the shell 
to display the prompt again without any error messages.

### 4. Signal handling
Your shell should be able to handle signals generated from keyboard using ‘Ctrl + C’ and
‘Ctrl + Z’. When these signals are generated, your shell should continue to work and the 
commands being executed by the shell should respond to these signals. Your shell should stop 
only with the ‘exit’ command.

### 5. Executing multiple commands
Your shell should support multiple command execution for sequential execution as well as for
parallel execution. The commands separated by ‘&&’ should be executed in parallel and the 
commands separated by ‘##’ should be executed sequentially. Also your shell must wait for all 
the commands to be terminated (for parallel and sequential executions, both) before accepting 
further inputs. Do not worry about simultaneous use of ‘&&’ and ‘##’, we will not test that.

### 6. Output redirection
Your shell should be able to redirect STDOUT for the commands using ‘>’ symbol. For example,
‘ls > info.out’ should write the output of ‘ls’ command to ‘info.out’ file instead of writing it on 
screen. Again, do not worry about simultaneous use of multiple commands and output 
redirection, as that won’t be tested too.

### 7. Support command pipelines 
You may want to implement pipe so that you can run command pipelines 
such as:
% cat myshell.c | grep open | wc
The ‘|’ token should indicate that the immediate token after the ‘|’ is another command. Your 
program should redirect the standard output of the command on the left to the standard input 
of the command on the right. If there’s no following token after ‘|’, your program should print 
out an appropriate error message. There can be multiple pipe operators in a single command.
