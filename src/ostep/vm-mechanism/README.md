# Address Translation Simulation

This program allows you to simulate the address translation process in a system that uses base and bound registers. Through this program, you can understand the basic concepts of base and bound and practice using them.

The lab is conducted in two steps:

1. First, run the program without the `-c` flag and attempt to perform the address translation manually.
2. Then, run the program with the `-c` flag to check the correct answers.

## Key Concepts

- Base register: A register that stores the starting physical memory address of a process's address space.
- Bound register: A register that stores the size of a process's address space. It is used to restrict the process to access only within the allowed memory region.
- Address translation: The process of mapping a virtual address to a physical address. It is performed using the base and bound registers.
- Segmentation violation: An exception that occurs when a process tries to access memory outside its allowed region.

## Memory Layout Assumption

In this lab, we assume a memory layout that differs from the typical structure:

- The code section is located at the beginning of the address space.
- A small, fixed-size stack is located immediately after the code section.
- The heap is located after the stack and grows downward (towards higher addresses).

```
-------------- 0KB
|    Code    |
-------------- 2KB
|   Stack    |
-------------- 4KB
|    Heap    |
|     |     |
|     v     |
-------------- 7KB
|   (free)   |
|    ...     |
```

In the above diagram, the bound register is set to 7KB, which represents the end of the address space.

- Memory references within the range of 0 ~ 7KB are considered valid.
- Memory references exceeding 7KB are considered out of bounds and trigger a hardware exception.

## Program Execution Examples

### Basic Execution

You can run the program with default flags by entering `relocation.py` in the command line.

```
prompt> ./relocation.py
...
Base-and-Bounds register information:
  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) --> PA or segmentation violation?
  VA  1: 0x0109 (decimal:265) --> PA or segmentation violation?
  VA  2: 0x020b (decimal:523) --> PA or segmentation violation?
  VA  3: 0x019e (decimal:414) --> PA or segmentation violation?
  VA  4: 0x0322 (decimal:802) --> PA or segmentation violation?
```

As shown above, for each randomly generated virtual address (VA), you need to determine whether it can be translated to a physical address (PA) or if a segmentation violation occurs.

### Checking Answers

You can use the `-c` flag to check the correct answers.

```
prompt> ./relocation.py -c
...
Virtual Address Trace
  VA  0: 0x01ae (decimal:430) --> VALID: 0x00003230 (decimal:12848)
  VA  1: 0x0109 (decimal:265) --> VALID: 0x0000318b (decimal:12683)
  VA  2: 0x020b (decimal:523) --> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) --> VALID: 0x00003220 (decimal:12832)
  VA  4: 0x0322 (decimal:802) --> SEGMENTATION VIOLATION
```

When the base value is 12418 (0x3082) and the bound is 472:

- 430 (0x01ae) is a valid address because it falls within the bound when added to the base, resulting in 12848.
- 523 (0x020b) and 802 (0x0322) exceed the bound of 472, causing a segmentation violation.

## Additional Flags

You can use the following flags to control the program more precisely:

```
prompt> ./relocation.py -h
Usage: relocation.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -a ASIZE, --asize=ASIZE address space size (e.g., 16, 64k, 32m)
  -p PSIZE, --physmem=PSIZE physical memory size (e.g., 16, 64k)
  -n NUM, --addresses=NUM   number of virtual addresses to generate
  -b BASE, --b=BASE     value of base register
  -l LIMIT, --l=LIMIT   value of limit register
  -c, --compute         compute answers for me
```

- `-a`: Set the size of the virtual address space.
- `-p`: Set the size of the physical memory.
- `-n`: Set the number of virtual addresses to generate.
- `-b`: Set the value of the process's base register.
- `-l`: Set the value of the process's bound register.

Using these flags, you can practice address translation under various conditions.

## Lab Exercises

Perform useful experiments using this code to understand memory mechanisms:

1. Experiment with different combinations of address space size and physical memory size:

   - Set the address space size smaller than the physical memory size (e.g., -a 16k -p 32k).
   - Set the address space size larger than the physical memory size (e.g., -a 32k -p 16k).
   - Set the address space size and physical memory size to be the same (e.g., -a 16k -p 16k).

   Observe how the relationship between address space size and physical memory size affects address translation.

2. Observe the impact of changing the base register value:

   - Set the base register value to 0 (e.g., -b 0).
   - Set the base register value equal to the physical memory size (e.g., -b 16384 -p 16k).
   - Set the base register value larger than the physical memory size (e.g., -b 32768 -p 16k).

   Verify the impact of changing the base register value on address translation results and segmentation violation occurrences.

3. Observe the impact of changing the bound register value:

   - Set the bound register value smaller than the address space size (e.g., -l 1024 -a 16k).
   - Set the bound register value equal to the address space size (e.g., -l 16384 -a 16k).
   - Set the bound register value larger than the address space size (e.g., -l 32768 -a 16k).

   Observe how changing the bound register value affects the valid address range and segmentation violation occurrences.

4. Experiment with generating and translating a large number of virtual addresses:

   - Set a large number of virtual addresses to generate (e.g., -n 1000).
   - Experiment with various combinations of address space size, physical memory size, base value, and bound value.

   Verify the performance and limitations of the memory mechanism when generating and translating a large number of virtual addresses.

5. Observe the behavior of the memory mechanism under extreme conditions:

   - Set the address space size to 1 byte (e.g., -a 1).
   - Set the physical memory size to 1 byte (e.g., -p 1).
   - Set the base register value to a negative number (e.g., -b -1024).
   - Set the bound register value to 0 (e.g., -l 0).

   Observe how the memory mechanism behaves under these extreme conditions and what error messages are displayed.

6. Fix the base register value and observe the impact of changing the bound register value:

   - Fix the base register value to 0 (e.g., -b 0).
   - Gradually increase the bound register value (e.g., -l 1024, -l 2048, -l 4096, ...).

   Observe how changing the bound register value affects the valid address range when the base register value is fixed.

7. Fix the bound register value and observe the impact of changing the base register value:

   - Fix the bound register value to half of the address space size (e.g., -l 8192 -a 16k).
   - Gradually increase the base register value (e.g., -b 0, -b 4096, -b 8192, ...).

   Verify how changing the base register value affects the valid address range and physical memory mapping when the bound register value is fixed.

8. Observe the segmentation violation occurrence pattern while changing the address space size:

   - Fix the physical memory size and bound register value (e.g., -p 16k -l 8192).
   - Gradually increase the address space size (e.g., -a 4k, -a 8k, -a 16k, -a 32k, ...).

   Observe how the pattern and ratio of segmentation violations change as the address space size varies.

9. Observe the address translation pattern while changing the physical memory size:

   - Fix the address space size and bound register value (e.g., -a 16k -l 8192).
   - Gradually increase the physical memory size (e.g., -p 16k, -p 32k, -p 64k, ...).

   Verify the impact of changing the physical memory size on address translation results and segmentation violation occurrences.

10. Compare address translation results while changing the random seed value:

    - Change only the random seed value under the same conditions (address space size, physical memory size, base/bound values) (e.g., -s 1, -s 2, -s 3, ...).

    By comparing the generated virtual address sequences and address translation results while changing the random seed value, you can understand the influence of randomness on the memory mechanism.

Through these experiments, you can analyze the behavior of the memory mechanism in more detail and identify address translation patterns and segmentation violation occurrence tendencies under various conditions. This will help operating system developers and system programmers optimize memory management techniques and improve stability.# Lab: Address Translation Simulation

This program allows you to simulate the address translation process in a system that uses base and bound registers. Through this program, you can understand the basic concepts of base and bound and practice using them.

The lab is conducted in two steps:

1. First, run the program without the `-c` flag and attempt to perform the address translation manually.
2. Then, run the program with the `-c` flag to check the correct answers.

## Key Concepts

- Base register: A register that stores the starting physical memory address of a process's address space.
- Bound register: A register that stores the size of a process's address space. It is used to restrict the process to access only within the allowed memory region.
- Address translation: The process of mapping a virtual address to a physical address. It is performed using the base and bound registers.
- Segmentation violation: An exception that occurs when a process tries to access memory outside its allowed region.

## Memory Layout Assumption

In this lab, we assume a memory layout that differs from the typical structure:

- The code section is located at the beginning of the address space.
- A small, fixed-size stack is located immediately after the code section.
- The heap is located after the stack and grows downward (towards higher addresses).

```
-------------- 0KB
|    Code    |
-------------- 2KB
|   Stack    |
-------------- 4KB
|    Heap    |
|     |     |
|     v     |
-------------- 7KB
|   (free)   |
|    ...     |
```

In the above diagram, the bound register is set to 7KB, which represents the end of the address space.

- Memory references within the range of 0 ~ 7KB are considered valid.
- Memory references exceeding 7KB are considered out of bounds and trigger a hardware exception.

## Program Execution Examples

### Basic Execution

You can run the program with default flags by entering `relocation.py` in the command line.

```
prompt> ./relocation.py
...
Base-and-Bounds register information:
  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) --> PA or segmentation violation?
  VA  1: 0x0109 (decimal:265) --> PA or segmentation violation?
  VA  2: 0x020b (decimal:523) --> PA or segmentation violation?
  VA  3: 0x019e (decimal:414) --> PA or segmentation violation?
  VA  4: 0x0322 (decimal:802) --> PA or segmentation violation?
```

As shown above, for each randomly generated virtual address (VA), you need to determine whether it can be translated to a physical address (PA) or if a segmentation violation occurs.

### Checking Answers

You can use the `-c` flag to check the correct answers.

```
prompt> ./relocation.py -c
...
Virtual Address Trace
  VA  0: 0x01ae (decimal:430) --> VALID: 0x00003230 (decimal:12848)
  VA  1: 0x0109 (decimal:265) --> VALID: 0x0000318b (decimal:12683)
  VA  2: 0x020b (decimal:523) --> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) --> VALID: 0x00003220 (decimal:12832)
  VA  4: 0x0322 (decimal:802) --> SEGMENTATION VIOLATION
```

When the base value is 12418 (0x3082) and the bound is 472:

- 430 (0x01ae) is a valid address because it falls within the bound when added to the base, resulting in 12848.
- 523 (0x020b) and 802 (0x0322) exceed the bound of 472, causing a segmentation violation.

## Additional Flags

You can use the following flags to control the program more precisely:

```
prompt> ./relocation.py -h
Usage: relocation.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -a ASIZE, --asize=ASIZE address space size (e.g., 16, 64k, 32m)
  -p PSIZE, --physmem=PSIZE physical memory size (e.g., 16, 64k)
  -n NUM, --addresses=NUM   number of virtual addresses to generate
  -b BASE, --b=BASE     value of base register
  -l LIMIT, --l=LIMIT   value of limit register
  -c, --compute         compute answers for me
```

- `-a`: Set the size of the virtual address space.
- `-p`: Set the size of the physical memory.
- `-n`: Set the number of virtual addresses to generate.
- `-b`: Set the value of the process's base register.
- `-l`: Set the value of the process's bound register.

Using these flags, you can practice address translation under various conditions.

## Lab Exercises

Perform useful experiments using this code to understand memory mechanisms:

1. Experiment with different combinations of address space size and physical memory size:

   - Set the address space size smaller than the physical memory size (e.g., -a 16k -p 32k).
   - Set the address space size larger than the physical memory size (e.g., -a 32k -p 16k).
   - Set the address space size and physical memory size to be the same (e.g., -a 16k -p 16k).

   Observe how the relationship between address space size and physical memory size affects address translation.

2. Observe the impact of changing the base register value:

   - Set the base register value to 0 (e.g., -b 0).
   - Set the base register value equal to the physical memory size (e.g., -b 16384 -p 16k).
   - Set the base register value larger than the physical memory size (e.g., -b 32768 -p 16k).

   Verify the impact of changing the base register value on address translation results and segmentation violation occurrences.

3. Observe the impact of changing the bound register value:

   - Set the bound register value smaller than the address space size (e.g., -l 1024 -a 16k).
   - Set the bound register value equal to the address space size (e.g., -l 16384 -a 16k).
   - Set the bound register value larger than the address space size (e.g., -l 32768 -a 16k).

   Observe how changing the bound register value affects the valid address range and segmentation violation occurrences.

4. Experiment with generating and translating a large number of virtual addresses:

   - Set a large number of virtual addresses to generate (e.g., -n 1000).
   - Experiment with various combinations of address space size, physical memory size, base value, and bound value.

   Verify the performance and limitations of the memory mechanism when generating and translating a large number of virtual addresses.

5. Observe the behavior of the memory mechanism under extreme conditions:

   - Set the address space size to 1 byte (e.g., -a 1).
   - Set the physical memory size to 1 byte (e.g., -p 1).
   - Set the base register value to a negative number (e.g., -b -1024).
   - Set the bound register value to 0 (e.g., -l 0).

   Observe how the memory mechanism behaves under these extreme conditions and what error messages are displayed.

6. Fix the base register value and observe the impact of changing the bound register value:

   - Fix the base register value to 0 (e.g., -b 0).
   - Gradually increase the bound register value (e.g., -l 1024, -l 2048, -l 4096, ...).

   Observe how changing the bound register value affects the valid address range when the base register value is fixed.

7. Fix the bound register value and observe the impact of changing the base register value:

   - Fix the bound register value to half of the address space size (e.g., -l 8192 -a 16k).
   - Gradually increase the base register value (e.g., -b 0, -b 4096, -b 8192, ...).

   Verify how changing the base register value affects the valid address range and physical memory mapping when the bound register value is fixed.

8. Observe the segmentation violation occurrence pattern while changing the address space size:

   - Fix the physical memory size and bound register value (e.g., -p 16k -l 8192).
   - Gradually increase the address space size (e.g., -a 4k, -a 8k, -a 16k, -a 32k, ...).

   Observe how the pattern and ratio of segmentation violations change as the address space size varies.

9. Observe the address translation pattern while changing the physical memory size:

   - Fix the address space size and bound register value (e.g., -a 16k -l 8192).
   - Gradually increase the physical memory size (e.g., -p 16k, -p 32k, -p 64k, ...).

   Verify the impact of changing the physical memory size on address translation results and segmentation violation occurrences.

10. Compare address translation results while changing the random seed value:

    - Change only the random seed value under the same conditions (address space size, physical memory size, base/bound values) (e.g., -s 1, -s 2, -s 3, ...).

    By comparing the generated virtual address sequences and address translation results while changing the random seed value, you can understand the influence of randomness on the memory mechanism.

Through these experiments, you can analyze the behavior of the memory mechanism in more detail and identify address translation patterns and segmentation violation occurrence tendencies under various conditions. This will help operating system developers and system programmers optimize memory management techniques and improve stability.
