# Homeworks

Each chapter has some questions at the end; we call these "homeworks", because you should do the "work" at your "home". Make sense? It's one of the innovations of this book.

Homeworks can be used to solidify your knowledge of the material in each of the chapters. Many homeworks are based on running a simulator, which mimic some aspect of an operating system. For example, a disk scheduling simulator could be useful in understanding how different disk scheduling algorithms work. Some other homeworks are just short programming exercises, allowing you to explore how real systems work.

For the simulators, the basic idea is simple: each of the simulators below let you both generate problems and obtain solutions for an infinite number of problems. Different random seeds can usually be used to generate different problems; using the `-c` flag computes the answers for you (presumably after you have tried to compute them yourself!).

Each homework included below has a README file that explains what to do. Previously, this material had been included in the chapters themselves, but that was making the book too long. Now, all that is left in the book are the questions you might want to answer with the simulator; the details on how to run code are all in the README.

Thus, your task: read a chapter, look at the questions at the end of the chapter, and try to answer them by doing the homework. Some require a simulator (written in Python); those are available by below. Some others require you to write some code. At this point, reading the relevant README is a good idea. Still others require some other stuff, like writing C code to accomplish some task.

To use these, the best thing to do is to clone the homeworks. For example:

```shell
prompt> git clone https://github.com/remzi-arpacidusseau/ostep-homework/
prompt> cd file-disks
prompt> ./disk.py -h
```

## Introduction

| Chapter                                                                               | What To Do        |
| ------------------------------------------------------------------------------------- | ----------------- |
| [Introduction](http://www.cs.wisc.edu/~remzi/OSTEP/intro.pdf)                         | No homework (yet) |

## Virtualization

| Chapter                                                                              | What To Do                                                                                                             |
| ------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------- |
| [Abstraction: Processes](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-intro.pdf)          | Run [process-run.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-intro)                      |
| [Process API](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)                       | Run [fork.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-api) and write some code           |
| [Direct Execution](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-mechanisms.pdf)           | Write some code                                                                                                        |
| [Scheduling Basics](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf)               | Run [scheduler.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-sched)                        |
| [MLFQ Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-mlfq.pdf)            | Run [mlfq.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-sched-mlfq)                        |
| [Lottery Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf)      | Run [lottery.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-sched-lottery)                  |
| [Multiprocessor Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-multi.pdf) | Run [multi.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-sched-multi)                      |
| [Abstraction: Address Spaces](http://www.cs.wisc.edu/~remzi/OSTEP/vm-intro.pdf)      | Write some code                                                                                                        |
| [VM API](http://www.cs.wisc.edu/~remzi/OSTEP/vm-api.pdf)                             | Write some code                                                                                                        |
| [Relocation](http://www.cs.wisc.edu/~remzi/OSTEP/vm-mechanism.pdf)                   | Run [relocation.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-mechanism)                    |
| [Segmentation](http://www.cs.wisc.edu/~remzi/OSTEP/vm-segmentation.pdf)              | Run [segmentation.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-segmentation)               |
| [Free Space](http://www.cs.wisc.edu/~remzi/OSTEP/vm-freespace.pdf)                   | Run [malloc.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-freespace)                        |
| [Paging](http://www.cs.wisc.edu/~remzi/OSTEP/vm-paging.pdf)                          | Run [paging-linear-translate.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-paging)          |
| [TLBs](http://www.cs.wisc.edu/~remzi/OSTEP/vm-tlbs.pdf)                              | Write some code                                                                                                        |
| [Multi-level Paging](http://www.cs.wisc.edu/~remzi/OSTEP/vm-smalltables.pdf)         | Run [paging-multilevel-translate.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-smalltables) |
| [Paging Mechanism](http://www.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys.pdf)            | Run [mem.c](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-beyondphys)                           |
| [Paging Policy](http://www.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys-policy.pdf)        | Run [paging-policy.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/vm-beyondphys-policy)         |
| [Complete VM](http://www.cs.wisc.edu/~remzi/OSTEP/vm-complete.pdf)                   | No homework (yet)                                                                                                      |

## Concurrency

| Chapter                                                                           | What To Do                                                                                                 |
| --------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| [Threads Intro](http://www.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)            | Run [x86.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-intro)              |
| [Thread API](http://www.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)                 | Run [some C code](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-api)           |
| [Locks](http://www.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)                    | Run [x86.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-locks)              |
| [Lock Usage](http://www.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf)         | Write some code                                                                                            |
| [Condition Variables](http://www.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)         | Run [some C code](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-cv)            |
| [Semaphores](http://www.cs.wisc.edu/~remzi/OSTEP/threads-sema.pdf)                | Read and write [some code](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-sema) |
| [Concurrency Bugs](http://www.cs.wisc.edu/~remzi/OSTEP/threads-bugs.pdf)          | Run [some C code](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/threads-bugs)          |
| [Event-based Concurrency](http://www.cs.wisc.edu/~remzi/OSTEP/threads-events.pdf) | Write some code                                                                                            |

## Persistence

| Chapter                                                                                     | What To Do                                                                                                              |
| ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| [I/O Devices](http://www.cs.wisc.edu/~remzi/OSTEP/file-devices.pdf)                         | No homework (yet)                                                                                                       |
| [Hard Disk Drives](http://www.cs.wisc.edu/~remzi/OSTEP/file-disks.pdf)                      | Run [disk.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-disks)                             |
| [RAID](http://www.cs.wisc.edu/~remzi/OSTEP/file-raid.pdf)                                   | Run [raid.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-raid)                              |
| [FS Intro](http://www.cs.wisc.edu/~remzi/OSTEP/file-intro.pdf)                              | Write some code                                                                                                         |
| [FS Implementation](http://www.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf)            | Run [vsfs.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-implementation)                    |
| [Fast File System](http://www.cs.wisc.edu/~remzi/OSTEP/file-ffs.pdf)                        | Run [ffs.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-ffs)                                |
| [Crash Consistency and Journaling](http://www.cs.wisc.edu/~remzi/OSTEP/file-journaling.pdf) | Run [fsck.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-journaling)                        |
| [Log-Structured File Systems](http://www.cs.wisc.edu/~remzi/OSTEP/file-lfs.pdf)             | Run [lfs.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-lfs)                                |
| [Solid-State Disk Drives](http://www.cs.wisc.edu/~remzi/OSTEP/file-ssd.pdf)                 | Run [ssd.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-ssd)                                |
| [Data Integrity](http://www.cs.wisc.edu/~remzi/OSTEP/file-integrity.pdf)                    | Run [checksum.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-integrity) and Write some code |
| [Distributed Intro](http://www.cs.wisc.edu/~remzi/OSTEP/dist-intro.pdf)                     | Write some code                                                                                                         |
| [NFS](http://www.cs.wisc.edu/~remzi/OSTEP/dist-nfs.pdf)                                     | Write some analysis code                                                                                                |
| [AFS](http://www.cs.wisc.edu/~remzi/OSTEP/dist-afs.pdf)                                     | Run [afs.py](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/dist-afs)                                |
