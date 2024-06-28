# Week 3 Homework

1.SJF와 FIFO 스케줄러로 길이가 200인 세 개의 작업을 실행할 때의 응답 시간과 반환 시간을 계산하세요.
```
SJF(SRTF, Shortest Remaining Time First):

작업 길이: 200, 200, 200
실행 순서: 0, 1, 2
응답 시간:
Job 0: Response = 0
Job 1: Response = 200
Job 2: Response = 400
반환 시간:
Job 0: Turnaround = 200
Job 1: Turnaround = 400
Job 2: Turnaround = 600
FIFO(First In, First Out):

작업 길이: 200, 200, 200
실행 순서: 0, 1, 2
응답 시간:
Job 0: Response = 0
Job 1: Response = 200
Job 2: Response = 400
반환 시간:
Job 0: Turnaround = 200
Job 1: Turnaround = 400
Job 2: Turnaround = 600
```

2.이제 작업의 길이를 다르게 하여 동일한 작업을 수행해 보세요: 100, 200, 300.
```
SJF(SRTF):

작업 길이: 100, 200, 300
실행 순서: 0, 1, 2
응답 시간:
Job 0: Response = 0
Job 1: Response = 100
Job 2: Response = 300
반환 시간:
Job 0: Turnaround = 100
Job 1: Turnaround = 400
Job 2: Turnaround = 700
FIFO:

작업 길이: 100, 200, 300
실행 순서: 0, 1, 2
응답 시간:
Job 0: Response = 0
Job 1: Response = 100
Job 2: Response = 300
반환 시간:
Job 0: Turnaround = 100
Job 1: Turnaround = 300
Job 2: Turnaround = 600
```

3.이제 RR 스케줄러와 시간 할당량(time-slice)을 1로 하여 동일한 작업을 수행해 보세요.
```
RR(Round Robin):
작업 길이: 100, 200, 300

시간 할당량: 1

실행 순서:

Job 0: [ time 0 ] Run job 0 for 1.00 secs
Job 1: [ time 1 ] Run job 1 for 1.00 secs
Job 2: [ time 2 ] Run job 2 for 1.00 secs
Job 0: [ time 3 ] Run job 0 for 1.00 secs
Job 1: [ time 4 ] Run job 1 for 1.00 secs
Job 2: [ time 5 ] Run job 2 for 1.00 secs
Job 2: [ time 6 ] Run job 2 for 1.00 secs ( DONE at 7.00 )
Job 0: [ time 7 ] Run job 0 for 1.00 secs ( DONE at 8.00 )
Job 1: [ time 8 ] Run job 1 for 1.00 secs ( DONE at 9.00 )
반환 시간:

Job 0: Turnaround = 8.00
Job 1: Turnaround = 9.00
Job 2: Turnaround = 7.00
```

4.어떤 유형의 워크로드에 대해 SJF가 FIFO와 동일한 반환 시간을 제공하나요?

-> SJF가 FIFO와 동일한 반환 시간을 제공하는 경우는 작업 길이가 모두 동일한 경우입니다. 예를 들어, 모든 작업의 길이가 100인 경우에는 SJF와 FIFO가 동일한 반환 시간을 가집니다.


5.어떤 유형의 워크로드와 시간 할당량에 대해 SJF가 RR과 동일한 응답 시간을 제공하나요?

-> SJF가 RR과 동일한 응답 시간을 제공하는 경우는 모든 작업이 동일한 길이를 가지며, 시간 할당량이 충분히 큰 경우입니다. 시간 할당량이 작으면 SJF는 짧은 작업에 우선순위를 더 주기 때문에 RR보다 더 짧은 응답 시간을 보일 수 있습니다.


6.작업 길이가 증가함에 따라 SJF에서 응답 시간은 어떻게 되나요? 시뮬레이터를 사용하여 이 추세를 보여줄 수 있나요?

-> 작업 길이가 증가할수록 SJF에서는 평균적인 응답 시간이 증가합니다. 짧은 작업에 더 빠른 응답을 주기 때문에, 작업 길이가 길어질수록 다른 작업이 먼저 처리되어 기다리는 시간이 증가합니다.


7.시간 할당량이 증가함에 따라 RR에서 응답 시간은 어떻게 되나요? N개의 작업이 주어졌을 때 최악의 응답 시간을 제공하는 수식을 작성할 수 있나요?

-> 시간 할당량이 증가할수록 RR에서는 평균 응답 시간이 증가합니다. 이는 시간 할당량이 커질수록 각 작업이 처리될 때까지 기다리는 시간이 늘어나기 때문입니다.

N개의 작업이 주어졌을 때, RR에서의 최악의 응답 시간은 다음과 같이 계산할 수 있습니다:

Worst Response Time=(N−1)*quantum
여기서 quantum은 시간 할당량입니다.
