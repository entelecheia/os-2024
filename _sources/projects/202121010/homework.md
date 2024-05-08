# Homework

**202121010 양필성**

## week 1

1. 다음 플래그와 함께 `process-run.py`를 실행하세요: `-l 5:100,5:100`. CPU 활용률(예: CPU가 사용 중인 시간의 백분율)은 어떻게 되어야 할까요? 왜 그렇게 알 수 있나요? `-c`와 `-p` 플래그를 사용해 여러분의 생각이 맞는지 확인해 보세요.

   - Answer: pid:0 process가 5개의 명령어를 처리할 동안 pid:1 process는 Ready 상태를 가진다. pid:0 process의 상태가 Done을 가지면 pid:1 process의 상태가 Run으로 바뀌며 CPU 사용을 한다. pid:1 process 또한 5개의 명령어를 처리해야한다. 따라서 CPU 활용률은 process 각각 50%씩 사용한다.

2. 이제 다음 플래그로 실행해 보세요: `./process-run.py -l 4:100,1:0`. 이 플래그는 4개의 명령어(모두 CPU 사용)를 가진 하나의 프로세스와 I/O를 실행하고 완료될 때까지 기다리는 하나의 프로세스를 지정합니다. 두 프로세스를 모두 완료하는 데 얼마나 걸리나요? `-c`와 `-p`를 사용해 여러분의 생각이 맞는지 확인해 보세요.

   - Answer: 11

3. 프로세스의 순서를 바꿔 보세요: `-l 1:0,4:100`. 이제 어떻게 되나요? 순서를 바꾸는 것이 중요한가요? 왜 그런가요? (항상 그렇듯이 `-c`와 `-p`를 사용해 여러분의 생각이 맞는지 확인해 보세요)

   - Answer: 두 프로세스를 모두 완료하는 시간이 줄었다. PID:0 process가 io를 기다려 CPU를 사용하지 않는 BLOCKED 상태로 바뀐다. PID:1 process가 PID:0 process BLOCKED 상태일 때 CPU를 사용할 수 있어 총 반환시간이 짧아진다.

4. 이제 다른 플래그들을 살펴보겠습니다. 중요한 플래그 중 하나는 `-S`인데, 이는 프로세스가 I/O를 실행할 때 시스템이 어떻게 반응하는지 결정합니다. 플래그를 `SWITCH_ON_END`로 설정하면 한 프로세스가 I/O를 수행하는 동안 시스템은 다른 프로세스로 전환하지 않고 해당 프로세스가 완전히 끝날 때까지 기다립니다. 다음 두 프로세스를 실행할 때 어떤 일이 일어나나요(`-l 1:0,4:100 -c -S SWITCH_ON_END`), 하나는 I/O를 수행하고 다른 하나는 CPU 작업을 수행합니다?

   - Answer: PID:0 process가 io를 기다려 CPU를 사용하지 않는 BLOCKED 상태로 바뀜에도 PID:1 process가 CPU를 사용하지 않는다. 이로인해 총 반환시간이 늘어났다.

5. 이제 같은 프로세스를 실행하되, 한 프로세스가 I/O를 기다리는(`WAITING`) 동안 다른 프로세스로 전환되도록 switching 동작을 설정해 봅시다(`-l 1:0,4:100 -c -S SWITCH_ON_IO`). 이제 어떤 일이 일어나나요? `-c`와 `-p`를 사용해 여러분의 생각이 맞는지 확인해 보세요.

   - Answer: PID:0 process가 io를 기다려 CPU를 사용하지 않는 BLOCKED 상태로 바뀌어 PID:1 process가 CPU를 사용한다. 이로인해 총 반환시간이 줄어들었다.

6. 또 다른 중요한 동작은 I/O가 완료될 때 수행할 작업입니다. `-I IO_RUN_LATER`를 사용하면 I/O가 완료될 때 그것을 실행한 프로세스가 반드시 즉시 실행되는 것은 아닙니다. 오히려 그 시점에 실행 중이던 프로세스가 계속 실행됩니다. 이러한 프로세스 조합을 실행하면 어떤 일이 일어나나요? (`./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`) 시스템 자원이 효율적으로 활용되고 있나요?

   - Answer: PID:0 process가 첫번째 io 명령어를 처리하는 동안 CPU를 사용하지 않아 PID:1 process가 CPU를 사용한다. 하지만 PID:0 process가 io 명령어를 다 처리해도 READY 상태로 바뀐다. PID:2, PID:3 process가 모두 실행되고 나서 PID:0 process의 첫번째 io 명령어가 끝나고 나머지 두번의 io 명령어를 연달아 실행한다. 하지만 두번째, 세번째 io 명령어를 처리하는 동안 CPU는 사용하지 않는다. 따라서 효율적이지 않다.

7. 이제 같은 프로세스를 실행하되, `-I IO_RUN_IMMEDIATE`를 설정해 I/O를 실행한 프로세스를 즉시 실행하도록 해 봅시다. 이 동작은 어떻게 다른가요? 방금 I/O를 완료한 프로세스를 다시 실행하는 것이 왜 좋은 생각일 수 있을까요?

   - Answer: PID:0 process가 첫번째 io 명령어를 처리하는 동안 3개의 프로세스를 처리하지 않는다, PID:0 process가 READY 상태로 바뀌지 않고 각 io 명령어마다 다른 프로세스들이 CPU를 사용함으로써 효율적으로 바뀌었다.

8. 이제 무작위로 생성된 프로세스로 실행해 봅시다: `-s 1 -l 3:50,3:50` 또는 `-s 2 -l 3:50,3:50` 또는 `-s 3 -l 3:50,3:50`. 추적 결과가 어떻게 될지 예측해 보세요. `-I IO_RUN_IMMEDIATE` 플래그와 `-I IO_RUN_LATER` 플래그를 사용할 때 어떤 일이 일어나나요? `-S SWITCH_ON_IO`와 `-S SWITCH_ON_END`를 사용할 때 어떤 일이 일어나나요?

   - Answer:
     1. `-I IO_RUN_IMMEDIATE` 플래그와 `-I IO_RUN_LATER` 플래그 효율성은 `IO_RUN_IMMEDIATE` 플래그가 더 효율성이 좋았다. 프로세스가 io 명령어 처리를 끝내고 READY 상태를 최대한 짧게 가지며 프러세스를 끝내고 그 다음 io 명령어를 시작하며 다른 프로세스가 CPU를 사용할 수 있게 했다.
     2. `-S SWITCH_ON_IO` 플래그와 `-S SWITCH_ON_END` 플래그 효율성은 `SWITCH_ON_IO` 플래그가 더 효율성이 좋았다. 프로세스가 CPU를 사용하지 않는 BLOCKED 상태에 다른 프로세스가 CPU를 사용할 수 있기 떄문이다.

## week 2

1. SJF와 FIFO 스케줄러로 길이가 200인 세 개의 작업을 실행할 때의 응답 시간과 반환 시간을 계산하세요.

   - Answer
     1. SJF
        |Job number|Response Time|Turnaround Time|
        |:------:|:---:|:---:|
        |Job 0|0|200|
        |Job 1|200|400|
        |Job 2|400|600|
     2. FIFO
        |Job number|Response Time|Turnaround Time|
        |:------:|:---:|:---:|
        |Job 0|0|200|
        |Job 1|200|400|
        |Job 2|400|600|

2. 이제 작업의 길이를 다르게 하여 동일한 작업을 수행해 보세요: 100, 200, 300.

   - Answer
     1. SJF
        |Job number|Response Time|Turnaround Time|
        |:------:|:---:|:---:|
        |Job 0|0|100|
        |Job 1|100|300|
        |Job 2|300|600|
     2. FIFO
        |Job number|Response Time|Turnaround Time|
        |:------:|:---:|:---:|
        |Job 0|0|100|
        |Job 1|100|300|
        |Job 2|300|600|

3. 이제 RR 스케줄러와 시간 할당량(time-slice)을 1로 하여 동일한 작업을 수행해 보세요.

   - Answer
     - RR -> same len
       |Job number|Response Time|Turnaround Time|
       |:------:|:---:|:---:|
       |Job 0|0|598|
       |Job 1|1|599|
       |Job 2|2|600|
     - RR -> different len
       |Job number|Response Time|Turnaround Time|
       |:------:|:---:|:---:|
       |Job 0|0|298|
       |Job 1|1|499|
       |Job 2|2|600|

4. 어떤 유형의 워크로드에 대해 SJF가 FIFO와 동일한 반환 시간을 제공하나요?

   - Answer: 짧은 순서로 도착하거나 동일한 길이의 작업이면 SJF와 FIFO는 동일한 반환 시간을 갖는다.

5. 어떤 유형의 워크로드와 시간 할당량에 대해 SJF가 RR과 동일한 응답 시간을 제공하나요?

   - Answer: 작업의 길이가 시간 할당량보다 짧거나 같을 때이다. 이 경우, SJF나 RR이나 모든 작업이 최초의 시간 할당량에서 한 번에 처리되므로 응답 시간이 동일하다.

6. 작업 길이가 증가함에 따라 SJF에서 응답 시간은 어떻게 되나요? 시뮬레이터를 사용하여 이 추세를 보여줄 수 있나요?

   - Answer: 작업의 길이가 증가함에 따라 응답시간은 일반적으로 증가한다. 작업 길이가 긴 작업이 뒤로 밀리기 때문에, 짧은 작업이 우선적으로 처리되고, 그 다음 긴 작업이 대기 시간을 더 가지게 된다.

7. 시간 할당량이 증가함에 따라 RR에서 응답 시간은 어떻게 되나요? N개의 작업이 주어졌을 때 최악의 응답 시간을 제공하는 수식을 작성할 수 있나요?

   - Answer: 시간 할당량이 증가함에 따라 응답 시간은 일반적으로 증가한다. 최악의 응답 시간 = (N-1)\*(시간 할당량).


## week 4

1. 3개의 작업과 난수 시드 1, 2, 3으로 시뮬레이션한 솔루션을 계산하세요.

   - Answer
      1. seed = 1
      ```sh
      ** Solutions **

      Random 651593 -> Winning ticket 119 (of 153) -> Run 2
      Jobs:
      (  job:0 timeleft:1 tix:84 )  (  job:1 timeleft:7 tix:25 )  (* job:2 timeleft:4 tix:44 ) 
      Random 788724 -> Winning ticket 9 (of 153) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:84 )  (  job:1 timeleft:7 tix:25 )  (  job:2 timeleft:3 tix:44 ) 
      --> JOB 0 DONE at time 2
      Random 93859 -> Winning ticket 19 (of 69) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:7 tix:25 )  (  job:2 timeleft:3 tix:44 ) 
      Random 28347 -> Winning ticket 57 (of 69) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:6 tix:25 )  (* job:2 timeleft:3 tix:44 ) 
      Random 835765 -> Winning ticket 37 (of 69) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:6 tix:25 )  (* job:2 timeleft:2 tix:44 ) 
      Random 432767 -> Winning ticket 68 (of 69) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:6 tix:25 )  (* job:2 timeleft:1 tix:44 ) 
      --> JOB 2 DONE at time 6
      Random 762280 -> Winning ticket 5 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:6 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      Random 2106 -> Winning ticket 6 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:5 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      Random 445387 -> Winning ticket 12 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:4 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      Random 721540 -> Winning ticket 15 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:3 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      Random 228762 -> Winning ticket 12 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:2 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      Random 945271 -> Winning ticket 21 (of 25) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:1 tix:25 )  (  job:2 timeleft:0 tix:--- ) 
      --> JOB 1 DONE at time 12
      ```
      
      2. seed = 2

      
      ```sh
      ** Solutions **

      Random 605944 -> Winning ticket 169 (of 197) -> Run 2
      Jobs:
      (  job:0 timeleft:9 tix:94 )  (  job:1 timeleft:8 tix:73 )  (* job:2 timeleft:6 tix:30 ) 
      Random 606802 -> Winning ticket 42 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:9 tix:94 )  (  job:1 timeleft:8 tix:73 )  (  job:2 timeleft:5 tix:30 ) 
      Random 581204 -> Winning ticket 54 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:8 tix:94 )  (  job:1 timeleft:8 tix:73 )  (  job:2 timeleft:5 tix:30 ) 
      Random 158383 -> Winning ticket 192 (of 197) -> Run 2
      Jobs:
      (  job:0 timeleft:7 tix:94 )  (  job:1 timeleft:8 tix:73 )  (* job:2 timeleft:5 tix:30 ) 
      Random 430670 -> Winning ticket 28 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:7 tix:94 )  (  job:1 timeleft:8 tix:73 )  (  job:2 timeleft:4 tix:30 ) 
      Random 393532 -> Winning ticket 123 (of 197) -> Run 1
      Jobs:
      (  job:0 timeleft:6 tix:94 )  (* job:1 timeleft:8 tix:73 )  (  job:2 timeleft:4 tix:30 ) 
      Random 723012 -> Winning ticket 22 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:6 tix:94 )  (  job:1 timeleft:7 tix:73 )  (  job:2 timeleft:4 tix:30 ) 
      Random 994820 -> Winning ticket 167 (of 197) -> Run 2
      Jobs:
      (  job:0 timeleft:5 tix:94 )  (  job:1 timeleft:7 tix:73 )  (* job:2 timeleft:4 tix:30 ) 
      Random 949396 -> Winning ticket 53 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:5 tix:94 )  (  job:1 timeleft:7 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 544177 -> Winning ticket 63 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:4 tix:94 )  (  job:1 timeleft:7 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 444854 -> Winning ticket 28 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:3 tix:94 )  (  job:1 timeleft:7 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 268241 -> Winning ticket 124 (of 197) -> Run 1
      Jobs:
      (  job:0 timeleft:2 tix:94 )  (* job:1 timeleft:7 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 35924 -> Winning ticket 70 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:94 )  (  job:1 timeleft:6 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 27444 -> Winning ticket 61 (of 197) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:94 )  (  job:1 timeleft:6 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      --> JOB 0 DONE at time 14
      Random 464894 -> Winning ticket 55 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:6 tix:73 )  (  job:2 timeleft:3 tix:30 ) 
      Random 318465 -> Winning ticket 92 (of 103) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:5 tix:73 )  (* job:2 timeleft:3 tix:30 ) 
      Random 380015 -> Winning ticket 48 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:5 tix:73 )  (  job:2 timeleft:2 tix:30 ) 
      Random 891790 -> Winning ticket 16 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:4 tix:73 )  (  job:2 timeleft:2 tix:30 ) 
      Random 525753 -> Winning ticket 41 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:3 tix:73 )  (  job:2 timeleft:2 tix:30 ) 
      Random 560510 -> Winning ticket 87 (of 103) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:2 tix:73 )  (* job:2 timeleft:2 tix:30 ) 
      Random 236123 -> Winning ticket 47 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:2 tix:73 )  (  job:2 timeleft:1 tix:30 ) 
      Random 23858 -> Winning ticket 65 (of 103) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:1 tix:73 )  (  job:2 timeleft:1 tix:30 ) 
      --> JOB 1 DONE at time 22
      Random 325143 -> Winning ticket 3 (of 30) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:1 tix:30 ) 
      --> JOB 2 DONE at time 23
      ```


      3. seed = 3


      ```sh
      ** Solutions **

      Random 13168 -> Winning ticket 88 (of 120) -> Run 1
      Jobs:
      (  job:0 timeleft:2 tix:54 )  (* job:1 timeleft:3 tix:60 )  (  job:2 timeleft:6 tix:6 ) 
      Random 837469 -> Winning ticket 109 (of 120) -> Run 1
      Jobs:
      (  job:0 timeleft:2 tix:54 )  (* job:1 timeleft:2 tix:60 )  (  job:2 timeleft:6 tix:6 ) 
      Random 259354 -> Winning ticket 34 (of 120) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:54 )  (  job:1 timeleft:1 tix:60 )  (  job:2 timeleft:6 tix:6 ) 
      Random 234331 -> Winning ticket 91 (of 120) -> Run 1
      Jobs:
      (  job:0 timeleft:1 tix:54 )  (* job:1 timeleft:1 tix:60 )  (  job:2 timeleft:6 tix:6 ) 
      --> JOB 1 DONE at time 4
      Random 995645 -> Winning ticket 5 (of 60) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:54 )  (  job:1 timeleft:0 tix:--- )  (  job:2 timeleft:6 tix:6 ) 
      --> JOB 0 DONE at time 5
      Random 470263 -> Winning ticket 1 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:6 tix:6 ) 
      Random 836462 -> Winning ticket 2 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:5 tix:6 ) 
      Random 476353 -> Winning ticket 1 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:4 tix:6 ) 
      Random 639068 -> Winning ticket 2 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:3 tix:6 ) 
      Random 150616 -> Winning ticket 4 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:2 tix:6 ) 
      Random 634861 -> Winning ticket 1 (of 6) -> Run 2
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (  job:1 timeleft:0 tix:--- )  (* job:2 timeleft:1 tix:6 ) 
      --> JOB 2 DONE at time 11
      ```


2. 이제 두 개의 특정 작업으로 실행해 보세요: 각각 길이는 10이지만 하나(작업 0)는 티켓이 1장이고 다른 하나(작업 1)는 100장입니다(예: `-l 10:1,10:100`). 티켓의 수가 이렇게 불균형할 때 어떤 일이 일어날까요? 작업 1이 완료되기 전에 작업 0이 실행될 수 있을까요? 얼마나 자주 그럴까요? 일반적으로 이러한 티켓 불균형이 복권 스케줄링의 동작에 어떤 영향을 미칠까요?


   - Answer
      ```sh
      ** Solutions **

      Random 134364 -> Winning ticket 34 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:10 tix:100 ) 
      Random 847434 -> Winning ticket 44 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:9 tix:100 ) 
      Random 763775 -> Winning ticket 13 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:8 tix:100 ) 
      Random 255069 -> Winning ticket 44 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:7 tix:100 ) 
      Random 495435 -> Winning ticket 30 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:6 tix:100 ) 
      Random 449491 -> Winning ticket 41 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:5 tix:100 ) 
      Random 651593 -> Winning ticket 42 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:4 tix:100 ) 
      Random 788724 -> Winning ticket 15 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:3 tix:100 ) 
      Random 93859 -> Winning ticket 30 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:2 tix:100 ) 
      Random 28347 -> Winning ticket 67 (of 101) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:1 )  (* job:1 timeleft:1 tix:100 ) 
      --> JOB 1 DONE at time 10
      Random 835765 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:10 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 432767 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:9 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 762280 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:8 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 2106 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:7 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 445387 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:6 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 721540 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:5 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 228762 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:4 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 945271 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:3 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 901428 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      Random 30590 -> Winning ticket 0 (of 1) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:1 )  (  job:1 timeleft:0 tix:--- ) 
      --> JOB 0 DONE at time 20
      ```


      - 이러한 유형의 작업을 하게되면 일방적으로 티켓수가 비교적 많은 작업이 빨리 끝난다. 이와 같은 예시처럼 극단적으로 차이가 많이 난다면 티켓수가 낮은 작업은 높은 확률로 작업을 하지 못하는 상태로 있는다. 티켓이 많은 작업이 끝난 후에 작업이 시작될 것이다. 이러한 티켓 불균형은 자원을 받는 것이 공평하게 흘러가지 않아 티켓 수가 낮은 작업은 반환 시간이 더 길어질 것이다. 


3. 길이가 100이고 티켓 할당이 100으로 동일한 두 개의 작업(`-l 100:100,100:100`)으로 실행할 때 스케줄러는 얼마나 불공정할까요? 몇 가지 다른 난수 시드로 실행하여 (확률적) 답을 결정하세요; 한 작업이 다른 작업보다 얼마나 일찍 끝나는지에 따라 불공정성이 결정됩니다.

   - Answer
      1. seed = 1
      ```sh
      ** Solutions **

      Random 134364 -> Winning ticket 164 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:100 tix:100 )  (* job:1 timeleft:100 tix:100 ) 
      Random 847434 -> Winning ticket 34 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:100 tix:100 )  (  job:1 timeleft:99 tix:100 ) 
      ...
      Random 194118 -> Winning ticket 118 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:4 tix:100 )  (* job:1 timeleft:1 tix:100 ) 
      --> JOB 1 DONE at time 196
      Random 104424 -> Winning ticket 24 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:4 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 665958 -> Winning ticket 58 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:3 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 296072 -> Winning ticket 72 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 499800 -> Winning ticket 0 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      --> JOB 0 DONE at time 200
      ```


      2. seed = 2
      ```sh
      ** Solutions **

      Random 956035 -> Winning ticket 35 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:100 tix:100 )  (  job:1 timeleft:100 tix:100 ) 
      Random 947828 -> Winning ticket 28 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:99 tix:100 )  (  job:1 timeleft:100 tix:100 ) 
      ...
      Random 511501 -> Winning ticket 101 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:10 tix:100 )  (* job:1 timeleft:1 tix:100 ) 
      --> JOB 1 DONE at time 190
      Random 581076 -> Winning ticket 76 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:10 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 51234 -> Winning ticket 34 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:9 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 418016 -> Winning ticket 16 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:8 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 525065 -> Winning ticket 65 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:7 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 181225 -> Winning ticket 25 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:6 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 93786 -> Winning ticket 86 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:5 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 802656 -> Winning ticket 56 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:4 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 366184 -> Winning ticket 84 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:3 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 519210 -> Winning ticket 10 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      Random 921451 -> Winning ticket 51 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      --> JOB 0 DONE at time 200
      ```


      3. seed = 3
      ```sh
      ** Solutions **

      Random 237964 -> Winning ticket 164 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:100 tix:100 )  (* job:1 timeleft:100 tix:100 ) 
      Random 544229 -> Winning ticket 29 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:100 tix:100 )  (  job:1 timeleft:99 tix:100 )

      ...

      Random 662987 -> Winning ticket 187 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:1 tix:100 )  (* job:1 timeleft:5 tix:100 ) 
      Random 519015 -> Winning ticket 15 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:100 )  (  job:1 timeleft:4 tix:100 ) 
      --> JOB 0 DONE at time 196
      Random 289042 -> Winning ticket 42 (of 100) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:4 tix:100 ) 
      Random 341069 -> Winning ticket 69 (of 100) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:3 tix:100 ) 
      Random 227466 -> Winning ticket 66 (of 100) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:2 tix:100 ) 
      Random 68067 -> Winning ticket 67 (of 100) -> Run 1
      Jobs:
      (  job:0 timeleft:0 tix:--- )  (* job:1 timeleft:1 tix:100 ) 
      --> JOB 1 DONE at time 200
      ```


      3. seed = 4
      ```sh
      ** Solutions **

      Random 236048 -> Winning ticket 48 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:100 tix:100 )  (  job:1 timeleft:100 tix:100 ) 
      Random 103166 -> Winning ticket 166 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:99 tix:100 )  (* job:1 timeleft:100 tix:100 )
      Random 488831 -> Winning ticket 31 (of 200) -> Run 0
      Jobs:
      (* job:0 timeleft:2 tix:100 )  (  job:1 timeleft:1 tix:100 ) 
      Random 864720 -> Winning ticket 120 (of 200) -> Run 1
      Jobs:
      (  job:0 timeleft:1 tix:100 )  (* job:1 timeleft:1 tix:100 ) 
      --> JOB 1 DONE at time 199
      Random 608125 -> Winning ticket 25 (of 100) -> Run 0
      Jobs:
      (* job:0 timeleft:1 tix:100 )  (  job:1 timeleft:0 tix:--- ) 
      --> JOB 0 DONE at time 200
      ```


      |Seed number|Job 0 time| Job 1 time|
      |:---:|:---:|:---:|
      |1|200|196|
      |2|200|190|
      |3|196|200|
      |4|200|199|
      
      결론: 몇가지 난수 시드를 통해 결과를 확인 한 결과 작업 1번이 작업 0번에 비해 2번 더 빨리 끝난 것을 알 수 있었다. 2번 시드 같은 경우에는 다른 시드들 보다 더 많은 차이가 난 것을 알 수 있었다.


## week 6


1. 시드 1, 2, 3으로 실행하고 프로세스에서 생성된 각 가상 주소가 범위 내에 있는지 또는 범위를 벗어났는지 계산하세요. 범위 내라면 변환을 계산하세요.


   - Answer
      1. seed = 1
      ```sh
      Base-and-Bounds register information:

      Base   : 0x0000363c (decimal 13884)
      Limit  : 290

      Virtual Address Trace
      VA  0: 0x0000030e (decimal:  782) --> PA or segmentation violation?
      VA  1: 0x00000105 (decimal:  261) --> PA or segmentation violation?
      VA  2: 0x000001fb (decimal:  507) --> PA or segmentation violation?
      VA  3: 0x000001cc (decimal:  460) --> PA or segmentation violation?
      VA  4: 0x0000029b (decimal:  667) --> PA or segmentation violation?
      ```


      VA 0~4번중 Limit을 넘지 않는 것은 VA 1번만 있으므로 VA 1번은 VALID: (decimal: 14145) 나올 것이며 나머지는 segmentation violation이 나올 것이다.


      2. seed = 2
      ```sh
      Base-and-Bounds register information:

      Base   : 0x00003ca9 (decimal 15529)
      Limit  : 500

      Virtual Address Trace
      VA  0: 0x00000039 (decimal:   57) --> PA or segmentation violation?
      VA  1: 0x00000056 (decimal:   86) --> PA or segmentation violation?
      VA  2: 0x00000357 (decimal:  855) --> PA or segmentation violation?
      VA  3: 0x000002f1 (decimal:  753) --> PA or segmentation violation?
      VA  4: 0x000002ad (decimal:  685) --> PA or segmentation violation?
      ```


      VA 0~4번중 Limit을 넘지 않는 것은 VA 0번과 1번만 있으므로 VA 0번은 VALID: (decimal: 15586), VA 1번은  VALID: (decimal: 15615)가  나올 것이며 나머지는 segmentation violation이 나올 것이다.


      3. seed = 3
      ```sh
      Base-and-Bounds register information:

      Base   : 0x000022d4 (decimal 8916)
      Limit  : 316

      Virtual Address Trace
      VA  0: 0x0000017a (decimal:  378) --> PA or segmentation violation?
      VA  1: 0x0000026a (decimal:  618) --> PA or segmentation violation?
      VA  2: 0x00000280 (decimal:  640) --> PA or segmentation violation?
      VA  3: 0x00000043 (decimal:   67) --> PA or segmentation violation?
      VA  4: 0x0000000d (decimal:   13) --> PA or segmentation violation?
      ```


      VA 0~4번중 Limit을 넘지 않는 것은 VA 3번과 4번만 있으므로 VA 3번은 VALID: (decimal: 8983), VA 4번은  VALID: (decimal: 8929)가  나올 것이며 나머지는 segmentation violation이 나올 것이다.


2. 다음 플래그로 실행하세요: `-s 0 -n 10`. 생성된 모든 가상 주소가 범위 내에 있도록 하려면 `-l`(한계 레지스터)을 어떤 값으로 설정해야 할까요?


   - Answer
   ```sh
   Base-and-Bounds register information:

   Base   : 0x00003082 (decimal 12418)
   Limit  : 472

   Virtual Address Trace
   VA  0: 0x000001ae (decimal:  430) --> PA or segmentation violation?
   VA  1: 0x00000109 (decimal:  265) --> PA or segmentation violation?
   VA  2: 0x0000020b (decimal:  523) --> PA or segmentation violation?
   VA  3: 0x0000019e (decimal:  414) --> PA or segmentation violation?
   VA  4: 0x00000322 (decimal:  802) --> PA or segmentation violation?
   VA  5: 0x00000136 (decimal:  310) --> PA or segmentation violation?
   VA  6: 0x000001e8 (decimal:  488) --> PA or segmentation violation?
   VA  7: 0x00000255 (decimal:  597) --> PA or segmentation violation?
   VA  8: 0x000003a1 (decimal:  929) --> PA or segmentation violation?
   VA  9: 0x00000204 (decimal:  516) --> PA or segmentation violation?
   ````

   VA 0~9번중 decimal 숫자가 가장 큰 것은 `VA 8: (decimal: 929)`이기 때문에 `-l`(한계 레지스터)을  929 숫자로 해야한다.


3. 다음 플래그로 실행하세요: `-s 1 -n 10 -l 100`. 주소 공간이 여전히 물리 메모리에 전체적으로 맞도록 기준을 설정할 수 있는 최대값은 얼마인가요?


   - Answer
      ```sh

      ARG seed 1
      ARG address space size 1k
      ARG phys mem size 16k

      Base-and-Bounds register information:

      Base   : 0x00000899 (decimal 2201)
      Limit  : 100

      Virtual Address Trace
      VA  0: 0x00000363 (decimal:  867) --> PA or segmentation violation?
      VA  1: 0x0000030e (decimal:  782) --> PA or segmentation violation?
      VA  2: 0x00000105 (decimal:  261) --> PA or segmentation violation?
      VA  3: 0x000001fb (decimal:  507) --> PA or segmentation violation?
      VA  4: 0x000001cc (decimal:  460) --> PA or segmentation violation?
      VA  5: 0x0000029b (decimal:  667) --> PA or segmentation violation?
      VA  6: 0x00000327 (decimal:  807) --> PA or segmentation violation?
      VA  7: 0x00000060 (decimal:   96) --> PA or segmentation violation?
      VA  8: 0x0000001d (decimal:   29) --> PA or segmentation violation?
      VA  9: 0x00000357 (decimal:  855) --> PA or segmentation violation?
      ```

      `phys mem size`를 통해 알 수 있듯이 물리적 메모리에 값은 16k(16,384)이다. 물리적 메모리가 최대 기준값을 가지려면 물리적 `(물리적 메모리 - 한계 레지스터)`를 하면 값을 구할 수 있다. 따라서 최대 기준값은 16,284이다. 


4. 위와 동일한 문제 중 일부를 실행해 보세요. 단, 더 큰 주소 공간(`-a`)과 물리 메모리(`-p`)를 사용하세요.

   - Answer
      ```sh
      ARG seed 1
      ARG address space size 16k
      ARG phys mem size 128k

      Base-and-Bounds register information:

      Base   : 0x0001b1e2 (decimal 111074)
      Limit  : 4646

      Virtual Address Trace
      VA  0: 0x000030e1 (decimal: 12513) --> SEGMENTATION VIOLATION
      VA  1: 0x00001053 (decimal: 4179) --> VALID: 0x0001c235 (decimal: 115253)
      VA  2: 0x00001fb5 (decimal: 8117) --> SEGMENTATION VIOLATION
      VA  3: 0x00001cc4 (decimal: 7364) --> SEGMENTATION VIOLATION
      VA  4: 0x000029b3 (decimal: 10675) --> SEGMENTATION VIOLATION
      VA  5: 0x0000327a (decimal: 12922) --> SEGMENTATION VIOLATION
      VA  6: 0x00000601 (decimal: 1537) --> VALID: 0x0001b7e3 (decimal: 112611)
      VA  7: 0x000001d0 (decimal:  464) --> VALID: 0x0001b3b2 (decimal: 111538)
      VA  8: 0x0000357d (decimal: 13693) --> SEGMENTATION VIOLATION
      VA  9: 0x00001bb2 (decimal: 7090) --> SEGMENTATION VIOLATION
      ```


      결론: 앞서 진행했던 작업들과 비슷한 상황이 나타났다. VA 0~9번중 VA 1,6,7번 만 유효한 변화가 이뤄졌다. 이러한 결과를 통해 알 수 있는 사실은 물리적 메모리의 크기, 주소 공간의 주소는 복잡성이 증가하지만 유효한 변화에 큰 영향을 끼치지 않는다. **한계 레지스터**가 중요한 영향을 끼친다는 사실을 알 수 있다. 만약 한계 레지스터의 크기가 작다면 많은 가상 주소에 대한 변환이 원활하게 이뤄지지 않을 것으로 생각된다.