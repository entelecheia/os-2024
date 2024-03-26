# Implementing the Lottery Scheduling Algorithm

Code from OSTEP chapter [Scheduling: Proportional Share](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf).

## Objectives

This lab is designed to achieve the following objectives:

1. Understand the basic concepts of the Lottery scheduling algorithm.
2. Learn how to implement a job queue using a linked list.
3. Learn how to select a job using random number generation.
4. Discover how to automate program building and execution using Makefiles.

## Contents

In this lab, we will use the lottery.c file and Makefile to implement and run the Lottery scheduling algorithm.

### lottery.c - Implementing the Lottery Scheduling Algorithm

- Implement a job queue using a linked list.
- Each job is assigned a number of tickets, and the total number of tickets is stored in the `gtickets` variable.
- Use the `insert` function to add new jobs to the queue and assign ticket numbers.
- Use the `print_list` function to print the current state of the job queue.
- In the `main` function, receive the seed and loop count as command-line arguments to perform Lottery scheduling.
  - Generate a random number to select the winning ticket.
  - Find the job corresponding to the selected ticket and schedule it.
  - Repeat the scheduling for the specified number of loops.

### Makefile - Automating Build and Execution

- Use the `make all` command to build the `lottery` program.
- Use the `make run` command to run the built `lottery` program.
  - Pass the seed value and loop count as arguments.
- Use the `make clean` command to delete the built `lottery` program.

Through this lab, you can understand the working principles of the Lottery scheduling algorithm and learn how to implement a job queue using a linked list. You will also learn how to automate the program build and execution process using Makefiles.

## Lab Steps

1. Open the lottery.c file, understand the code, and identify the functionality of each part through comments.
2. Open the Makefile and check the build and execution rules.
3. In the terminal, enter the `make all` command to build the `lottery` program.
4. In the terminal, enter the `make run` command to run the `lottery` program.
5. Verify the behavior of the Lottery scheduling algorithm through the execution results.
6. If necessary, use the `make clean` command to delete the built program.

Through this lab, you will understand the implementation and working principles of the Lottery scheduling algorithm, and you will become familiar with the usage of linked lists and Makefiles.
