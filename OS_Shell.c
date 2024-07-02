
/********************************************************************************************
This is a template for assignment on writing a custom Shell.

Students may change the return types and arguments of the functions given in this template,
but do not change the names of these functions.

Though use of any extra functions is not recommended, students may use new functions if they need to,
but that should not make code unnecessorily complex to read.

Students should keep names of declared variable (and any new functions) self explanatory,
and add proper comments for every logical step.

Students need to be careful while forking a new process (no unnecessory process creations)
or while inserting the CMD_SINGLE handler code (should be added at the correct places).

Finally, keep your filename as myshell.c, do not change this name (not even myshell.cpp,
as you not need to use any features for this assignment that are supported by C++ but not by C).

Name - Mula Siva Prasad Babu
Roll No - BT21CSE129
OS - Assignment 1
*********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>	  // to use exit() function
#include <unistd.h>	  // to use fork(), getpid(), exec() functions
#include <sys/wait.h> // to use wait() function
#include <signal.h>	  // to use signal() function
#include <fcntl.h>	  // to use close(), open() functions


// This function will parse the input string into multiple commands or a CMD_SINGLE command depending on the delimiter (&&, ##, >, or spaces).
void tokeniseorparsetheInput(char *Token_line, char **Token_words)
{

	int m = 0;
	char *string;
	int flag = 1;
	while ((string = strsep(&Token_line, " ")) != NULL) // strsep to separate the inputs  by space delimiter
	{
		if (strlen(string) == 0) // for handling the null string
		{
			continue;
		}
		Token_words[m] = string;
		m++;
	}
	Token_words[m] = NULL;
}
// for type of commands
enum choiceofcommand
{
	CMD_SINGLE,
	CMD_SEQUENTIAL,
	CMD_PARALLEL,
	CMD_REDIRECTION
};

void cd_command(char **Token_words)
{

	int finalresult;
	if (Token_words[1] != NULL)
	{
		finalresult = chdir(Token_words[1]);

	}	
	else
	{
		finalresult = chdir(getenv("HOME"));
	}
		
	if (Token_words[2] != NULL) // handling case of too many arguments
	{
		
		printf("shell: cd: too many arguments\n");
		// reset();
		return;
	}
	if (finalresult == -1)
	{
	
		printf("shell: cd: %s: No such file or directory\n", Token_words[1]); // when no such path exists
																				// reset();
	}
}

// This function will fork a new process to execute a command.
void executeCommand(char **Token_words)
{

	if (strcmp(Token_words[0], "cd") == 0)
	{
		cd_command(Token_words); // To change the current working Directory.
	}
	else if (strcmp(Token_words[0], "exit") == 0)
	{
		

		printf("Exiting shell...\n");
		// reset();
		exit(1);
	}
	else
	{
		size_t pid;
		int status;
		pid = fork();
		if (pid < 0)
		{
		
			// Error in forking.
			printf("Fork failed");
			// reset();
			exit(1);
		}
		else if (pid == 0)
		{
			// Child Process.
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			if (execvp(Token_words[0], Token_words) < 0) // in case of wrong command.
			{
				
				printf("Shell: Incorrect command\n");
				// reset();
				exit(1);
			}
		}
		else
		{
			waitpid(pid, &status, WUNTRACED); // wait system call works even if the child has stopped
		}
	}
}

//  This function will run multiple commands in CMD_PARALLEL.
void executeCMD_PARALLELCommands(char **Token_words)
{
	pid_t pid, pidt;
	int status = 0;
	int r = 0;
	int previous_index = r;
	while (Token_words[r] != NULL)
	{
		while (Token_words[r] != NULL && strcmp(Token_words[r], "&&") != 0)
		{
			r++;
		}
		Token_words[r] = NULL;
		pid = fork();
		if (pid < 0)
		{
			exit(1);
		}
		if (pid == 0)
		{
			// child process.
			if (execvp(Token_words[previous_index], &Token_words[previous_index]) < 0)
			{
				
				printf("Shell:Incorrect command\n");
				// reset();
				exit(1);
			}
		}
		else
		{
			waitpid(pid, &status, WUNTRACED); // wait system call works even if the child has stopped
		}
		r++;
		previous_index = r;
	}
}

//  This function will run multiple commands in a sequence.
void executeCMD_SEQUENTIALCommands(char **Token_words)
{
	int n = 0,w,place;
	char **Command_val;
	size_t CMD_size = 64;
	Command_val = malloc(CMD_size * sizeof(char *));
	while ((n == 0 && Token_words[n] != NULL) || (Token_words[n - 1] != NULL && Token_words[n] != NULL))
	{
		w = n;
	    place = 0;
		while (Token_words[w] != NULL && strcmp(Token_words[w], "##") != 0)
		{
			Command_val[place] = Token_words[w];
			place++;
			w++;
		}
		Command_val[place] = NULL;
		executeCommand(&Command_val[0]);
		n = w + 1;
	}
}

void executeCommandCMD_REDIRECTION(char **Token_words)
{
	pid_t pid;
	int status = 0;
	int c = 0;
	while (Token_words[c] != NULL)
	{
		while (Token_words[c] != NULL && strcmp(Token_words[c], ">") != 0)
		{
			c++;
		}
		Token_words[c] = NULL;
		pid = fork();
		if (pid < 0)
		{
		
			printf("Fork failed");
			// reset();
			exit(1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);											// enable SIGINT
			signal(SIGTSTP, SIG_DFL);											// enable SIGTSTP
			close(STDOUT_FILENO);												// close the standard output
			open(Token_words[c + 1], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU); // open the requested file

			// child process.
			if (execvp(Token_words[0], Token_words) < 0) // in case of wrong command.
			{
				
				printf("Shell: Incorrect command\n");
				// reset();
				exit(1);
			}
		}
		else
		{
			waitpid(pid, &status, WUNTRACED); // this version of wait system call works even if the child has stopped
		}
	}
}

int main()
{
	// Initial Declarations
	size_t cmdsize = 64;
	char *Token_line;
	Token_line = (char *)malloc(cmdsize * sizeof(char));

	signal(SIGINT, SIG_IGN); // Used in order to ignore both Ctrl+C(SIGINT) and Ctrl+Z(SIGKILL) in the shell
	signal(SIGTSTP, SIG_IGN);

	while (1) // This loop will keep your shell running until user enters exit i.e will create infinite loop.
	{
		// Print the prompt in format - currentWorkingDirectory$
		char dir[1000];
	
		printf("%s$", getcwd(dir, 1000));
		// reset();

		// Taking the Input.

		getline(&Token_line, &cmdsize, stdin); // using getline function to take input.

		int length = strlen(Token_line);
		Token_line[length - 1] = '\0'; // getline is terminated by '\n' so replace it with'\0'.

		if (strcmp(Token_line, "") == 0) // When user enters just enter button.
		{
			continue; // we do nothing and just continue.
		}

		char **Token_words;
		Token_words = malloc(cmdsize * sizeof(char *));

		// Parse input with 'strsep()' for different symbols (&&, ##, >) and for spaces.
		tokeniseorparsetheInput(Token_line, Token_words);

		if (strcmp(Token_words[0], "exit") == 0) // When user enters exit command.
		{
			
			printf("Exiting shell...\n");
			// reset();
			break;
		}

		int i = 0;
		int choiceofcommand = CMD_SINGLE;	 // enumeration for checking type of process
		while (Token_words[i] != NULL) // checking for CMD_PARALLEL and CMD_SEQUENTIAL processes and CMD_REDIRECTION
		{
			if (strcmp(Token_words[i], "&&") == 0)
			{
				choiceofcommand = CMD_PARALLEL;
				break;
			}
			if (strcmp(Token_words[i], "##") == 0)
			{
				choiceofcommand = CMD_SEQUENTIAL;
				break;
			}
			if (strcmp(Token_words[i], ">") == 0)
			{
				choiceofcommand = CMD_REDIRECTION;
				break;
			}
			i++;
		}

		
		 if (choiceofcommand == CMD_SEQUENTIAL)
			executeCMD_SEQUENTIALCommands(Token_words);
		else	if (choiceofcommand == CMD_PARALLEL) // user wants to run multiple commands in CMD_PARALLEL (commands separated by &&).
			executeCMD_PARALLELCommands(Token_words); // user wants to run multiple commands CMD_SEQUENTIALly (commands separated by ##).
		else if (choiceofcommand == CMD_REDIRECTION)
			executeCommandCMD_REDIRECTION(Token_words); // user wants redirect output of a CMD_SINGLE command to and output file specificed by user.
		else
			executeCommand(Token_words); // user wants to run a CMD_SINGLE commands.
	}
}
