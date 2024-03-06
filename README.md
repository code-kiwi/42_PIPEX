![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
# PIPEX

> **Note:** This is a project from **42 learning cursus**.

## Description
This project is the first 42 UNIX project. It introduces us redirections and pipes and a first step before coding our own tiny shell.<br>
We are invited to reproduce the pipe mechanism used in UNIX terminal.<br>
We are supposed to build an executable `pipex` working as follow:<br>
`./pipex infile "ls -l" "wc -l" outfile` behaves like: `< infile ls -l | wc -l > outfile`<br><br>
The subject suggests two bonus:
- handle multiple pipes (infinit pipes) like this: `./pipex infile cmd1 cmd2 cmd3 cmd... cmdn outfile`
- handle here_doc like this: `./pipex here_doc LIMITER cmd cmd1 file` behaves like: `cmd << LIMITER | cmd1 >> file`

## Pinciples
Here are the main steps of the project:
1. Test if here_doc is asked: then a pipe is opened, we write inside of it, close the right end and the read end will be used as first command STDIN;
2. File handling: open the input file (if not in here_doc mode) and the output files in order to have valid file descriptors for them
3. Parse the `envp` variable in order to extract the PATH
4. Parse the commands in order to store them into a linked list of t_command structures
5. Execute the commands, loop through the linked list created before and, for each command:
    - attribute it its file descriptors
    - use `fork()` in order to create a child process
    - use dup2() in order to redirect the child process STDIN_FILENO and STDOUT_FILENO
    - try to find the right executable with `access()`
    - execute the command with `execve()`
    - in the main process, close the useless file descriptors (the child process has its fds, the parent process do not need them anymore)
6. Use `wait()` or `waitpid()` for each child process you created

Each step can encounter an error, we needed to handle them properly.<br>
Valgrind is a useful tool, especially with the `--track-fds=yes` flag. 


## Setup instructions
1. Clone the repo: `git clone https://github.com/code-kiwi/42_PIPEX.git`
2. Clone the submodule libft: `git submodule update --init --recursive`
3. Compile the project `make`
The executable `pipex` should be created.
> **Note:** The bonus part is included into the mandatory part.

## How to use the progam ?
Pretty simple to use: test any commands and files you want, the pipex should behave exactly the same than the bash pipe.<br>
Here is an example:
- create a file "infile" and put some dummy content inside of it
- run the command: `./pipex infile cat cat outfile`
- your content should have been written into outfile
- run the command `./pipex infile cat cat cat cat cat outfile` for testing multiple pipes
- run the command `./pipex here_doc MY_LIMITER cat cat outfile`
- type the lines you want and end the STDIN reading process by typing "MY_LIMITER"
- your lines should have been appended to outfile
Finally as boring as a true terminal pipe.

## Testing
I did not write any testing scripts.<br>
However, here are the edge cases I found while working on this project:
|TEST                                                              |COMMENT                                                             |ADVICE                                                                                                                                                                                                                |
|------------------------------------------------------------------|--------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|`./pipex infile "sleep 5" "sleep 5" outfile`                      |Your program should sleep for 5 seconds, not for 10 seconds         |The problem usually encountered is due to the way you use `wait()` or `waitpid()`                                                                                                                                     |
|`./pipex /dev/urandom cat "head -1" outfile`                      |Your program has to stop running, if it does not you made a mistake |Some file descriptors have not been closed correctly                                                                                                                                                                  |
|`./pipex infile "/bin/cat" "/bin/cat" outfile`                    |You have to handle absolute paths for the commands                  |Handle them                                                                                                                                                                                                           |
|`./pipex infile "./pipex infile cat cat outfile2" ls outfile`     |You have to handle relative paths for the commands                  |Handle them                                                                                                                                                                                                           |
|`env -i ./pipex infile cat cat outfile`                           |You should not crash when the environment is not provided           |Check the way you handle the path                                                                                                                                                                                     |
|`./pipex infile cat cat cat...cat outfile`                        |You do not handle more than 1024 pipes                              |The number of file descriptors is limited, but by closing them wisely, you will not encounter any problem                                                                                                             |
|Program returned value                                            |Your program returned value is not correct                          |You always return the last command returned value. When you call `wait()` or `waitpid()`, use the status in order to retrieve the last command returned value. Check for the values you put into your `exit()` calls. |
|Files created rights                                              |Your program does not create output files with de correct rights    |Have a look at the real pipex way of doing                                                                                                                                                                            |

Be careful with the errors and the way you handle them. Take time to see how errors affect the pipe process, which command is executed or not, etc.<br>
There are useful testing tools like [this one](https://github.com/vfurmane/pipex-tester) from [vfurmane](https://github.com/vfurmane)<br>
It does not test everything but can help you to find some mistakes.

## Grade
![PS](https://github.com/ayogun/42-project-badges/blob/main/badges/pipexm.png)<br><br>
`125/125`<br>
> I spent time on this project in order to build solid foundations around UNIX systems. As an introduction, this project is not really long or difficult, but working hard on it is a good way to prepare for Minishell project.

## Credits
The badge comes from [this repo](https://github.com/ayogun/42-project-badges/tree/main), by [@ayogun](https://github.com/ayogun) which proposes a lot of amazing badges for 42 projects.
