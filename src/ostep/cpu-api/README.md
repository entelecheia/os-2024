# CPU API

Code from OSTEP chapter [Interlude: Process API](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf).

## Lab Title

**Basics of Process Creation and Execution**

## Lab Objectives

This lab is designed to achieve the following objectives:

1. Understand how to create a new process using the fork() function.
2. Understand the execution flow of parent and child processes.
3. Learn how to use the wait() function to make the parent process wait for the child process to terminate.
4. Learn how to execute a new program using the execvp() function.
5. Discover how to redirect standard output to a file.

## Lab Contents

In this lab, we will use 4 C program files (p1.c, p2.c, p3.c, p4.c) and one Makefile to learn about process creation and execution.

1. p1.c - Basics of Process Creation

   - Use the fork() function to create a new child process.
   - Verify the execution flow of parent and child processes.
   - Print the process ID (PID) to distinguish between parent and child processes.

2. p2.c - Waiting for Parent Process

   - Use the fork() function to create a new child process.
   - Use the sleep() function in the child process to wait for a certain period of time.
   - Use the wait() function in the parent process to wait for the child process to terminate.

3. p3.c - Executing a New Program

   - Use the fork() function to create a new child process.
   - Use the execvp() function in the child process to execute the "wc" program.
   - Set the arguments to be passed to the execvp() function to count the number of words in the "p3.c" file.

4. p4.c - Standard Output Redirection
   - Use the fork() function to create a new child process.
   - Close the standard output in the child process and redirect it to the "./p4.output" file.
   - Use the execvp() function to execute the "wc" program and count the number of words in the "p4.c" file.
   - Make the parent process wait for the child process to terminate.

Makefile:

- Use the "make all" command to compile all programs (p1, p2, p3, p4) at once.
- Define compilation rules for each program.
- Use the "make clean" command to delete the generated executable files.

Through this lab, you can learn the concepts of process creation, execution flow between parent and child processes, executing new programs, and redirecting standard output. Each program file includes detailed comments to help understand the code's behavior.
