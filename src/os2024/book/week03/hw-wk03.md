# Homework: Scheduling

| [Scheduling Basics](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf) | Run [scheduler.py](https://github.com/chu-aie/os-2024/tree/main/src/ostep-homework/cpu-sched) |

## Introduction

This program, `scheduler.py`, allows you to see how different schedulers perform under scheduling metrics such as response time, turnaround time, and total wait time. See the README for details.

## Questions

1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

2. Now do the same but with jobs of different lengths: 100, 200, and 300.

3. Now do the same, but also with the RR scheduler and a time-slice of 1.

4. For what types of work loads does SJF deliver the same turnaround times as FIFO?

5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?

7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
