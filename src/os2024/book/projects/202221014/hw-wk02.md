# Week 2 Homework

## 1. CPU 활용률 예측

예상 CPU 활용률: 약 100%

### 해설

- -l 5:100 플래그는 두 개의 프로세스를 생성합니다. 첫 번째 프로세스는 CPU를 5초 동안 100% 사용하고, 두 번째 프로세스는 CPU를 5초 동안 100% 사용합니다.
- 프로세스가 실행되는 동안 다른 프로세스가 생성되지 않기 때문에 CPU는 항상 사용 중이어야 합니다.
- 따라서 CPU 활용률은 약 100%가 될 것으로 예상됩니다.
- "./process-run.py -l 5:100,5:100 -c -p"를 사용하여 CPU 활용률이 100%임을 확인

```{note}
-c 플래그를 사용하여 CPU 활용률을 확인합니다.
-p 플래그를 사용하여 프로세스 스케줄링을 확인합니다.
```

```
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5        RUN:cpu         READY             1
  6           DONE       RUN:cpu             1
  7           DONE       RUN:cpu             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

## 2. 프로세스 완료 시간 예측

### 해설

- -l 4:100,1:0 플래그로 실행하면, 첫 번째 프로세스는 CPU 작업 4개를 수행하고, 두 번째 프로세스는 I/O 작업 1개를 수행합니다.
- 두 프로세스를 모두 완료하는 데 걸리는 시간은 CPU 작업 4개와 I/O 작업 1개를 수행하는 데 필요한 시간의 합입니다.

```
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

## 3. 프로세스 순서 변경

### 변화점

- 프로세스 순서가 바뀌면 CPU 활용률과 프로세스 완료 시간에 영향을 미칠 수 있습니다.
- I/O 작업을 수행하는 프로세스가 먼저 실행되면 CPU가 사용되지 않고 I/O 작업이 완료될 때까지 기다려야 하기 때문에 CPU 활용률이 낮아질 수 있습니다.

```
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

## 4. I/O 완료 시 시스템 동작

### 수행

- -S SWITCH_ON_END 플래그는 시스템이 I/O를 수행하는 프로세스가 완료될 때까지 다른 프로세스로 전환하지 않도록 합니다.
- CPU는 I/O 작업이 완료될 때까지 첫 번째 프로세스만 사용합니다.
- 첫 번째 프로세스가 I/O 작업을 완료하면 두 번째 프로세스가 실행됩니다.

### 결과

- CPU 활용률은 처음에는 100%이고 I/O 작업이 완료된 후에는 0%가 됩니다.
- 두 번째 프로세스는 첫 번째 프로세스의 I/O 작업이 완료될 때까지 시작되지 않습니다.

```
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED         READY                           1
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7*   RUN:io_done         READY             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
 11           DONE       RUN:cpu             1
```

## 5. I/O 완료 후 프로세스 실행

플래그: ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -c -p

### 수행

- -I IO_RUN_IMMEDIATE 플래그는 I/O가 완료되면 I/O를 수행한 프로세스를 즉시 실행하도록 합니다.
- I/O 작업이 완료되면 시스템은 I/O를 수행한 프로세스를 실행하고 다른 실행 가능한 프로세스가 있다면 해당 프로세스를 계속 실행합니다.

### 결과

- CPU 활용률은 높을 가능성이 높습니다.
- I/O 작업이 완료되면 I/O를 수행한 프로세스가 즉시 실행되므로 CPU가 유휴 상태가 되지 않습니다.
- 시스템 자원의 효율적인 활용으로 이어질 수 있습니다.

```
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1
  2        BLOCKED       RUN:cpu         READY         READY             1             1
  3        BLOCKED       RUN:cpu         READY         READY             1             1
  4        BLOCKED       RUN:cpu         READY         READY             1             1
  5        BLOCKED       RUN:cpu         READY         READY             1             1
  6        BLOCKED       RUN:cpu         READY         READY             1             1
  7*   RUN:io_done          DONE         READY         READY             1
  8         RUN:io          DONE         READY         READY             1
  9        BLOCKED          DONE       RUN:cpu         READY             1             1
 10        BLOCKED          DONE       RUN:cpu         READY             1             1
 11        BLOCKED          DONE       RUN:cpu         READY             1             1
 12        BLOCKED          DONE       RUN:cpu         READY             1             1
 13        BLOCKED          DONE       RUN:cpu         READY             1             1
 14*   RUN:io_done          DONE          DONE         READY             1
 15         RUN:io          DONE          DONE         READY             1
 16        BLOCKED          DONE          DONE       RUN:cpu             1             1
 17        BLOCKED          DONE          DONE       RUN:cpu             1             1
 18        BLOCKED          DONE          DONE       RUN:cpu             1             1
 19        BLOCKED          DONE          DONE       RUN:cpu             1             1
 20        BLOCKED          DONE          DONE       RUN:cpu             1             1
 21*   RUN:io_done          DONE          DONE          DONE             1

Stats: Total Time 21
Stats: CPU Busy 21 (100.00%)
Stats: IO Busy  15 (71.43%)
```

## 6. 시스템 자원 활용 효율성 비교

### 플래그 비교

| 플래그                | 설명                           | 장점                                                                            | 단점                                                                    |
| --------------------- | ------------------------------ | ------------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| `-I IO_RUN_LATER`     | I/O 완료 후 프로세스 지연 실행 | - CPU 바운드 프로세스 우선 실행 가능성 증가<br>- 시스템 안정성 향상 가능성 증가 | - I/O 바운드 프로세스 지연 가능성 증가<br>- CPU 활용률 저하 가능성 증가 |
| `-I IO_RUN_IMMEDIATE` | I/O 완료 후 프로세스 즉시 실행 | - CPU 활용률 향상 가능성 증가<br>- I/O 작업 완료 후 응답 시간 단축 가능성 증가  | - CPU 바운드 프로세스 I/O 바운드 프로세스 차단 가능성 증가              |

### 결론

- `-I IO_RUN_IMMEDIATE` 플래그는 일반적으로 시스템 자원 활용 효율성을 향상시키는 데 도움이 될 수 있지만, I/O 작업이 CPU 작업보다 느린 경우 주의해서 사용해야 합니다.
- `-I IO_RUN_LATER` 플래그는 CPU 바운드 프로세스가 I/O 바운드 프로세스를 차단할 가능성이 있으므로 주의해서 사용해야 합니다.

### 적절한 플래그 선택

- 시스템의 작업 유형과 I/O 작업 속도를 고려하여 플래그를 선택해야 합니다.
- CPU 작업이 I/O 작업보다 훨씬 많은 경우 `-I IO_RUN_IMMEDIATE` 플래그를 사용하는 것이 효율적일 수 있습니다.
- I/O 작업이 CPU 작업과 비슷하거나 더 많은 경우 `-I IO_RUN_LATER` 플래그를 사용하는 것이 더 나은 성능을 제공할 수 있습니다.

### 추가 고려 사항

- 시스템의 CPU 코어 수도 플래그 선택에 영향을 미칠 수 있습니다.
- CPU 코어가 많을수록 여러 프로세스를 동시에 실행할 수 있으므로 `-I IO_RUN_IMMEDIATE` 플래그를 사용해도 CPU 바운드 프로세스가 I/O 바운드 프로세스를 차단할 가능성이 줄어듭니다.

## 7. I/O 즉시 실행 플래그

### 기본 동작

- I/O 작업을 완료한 프로세스는 실행 대기 큐에 삽입됩니다.
- 시스템은 스케줄링 알고리즘에 따라 실행 대기 큐에서 프로세스를 선택하여 실행합니다.
- 일반적으로 CPU 사용량이 높은 프로세스가 우선 실행됩니다.

### I/O 즉시 실행 플래그 사용 시 동작

- I/O 작업을 완료한 프로세스가 CPU를 사용할 수 있는 경우 즉시 실행됩니다.
- 다른 실행 가능한 프로세스가 없는 경우 시스템은 I/O 작업을 완료한 프로세스를 CPU에 할당합니다.

#### 다른 실행 가능한 프로세스가 있는 경우

- -S SWITCH_ON_IO 플래그와 함께 사용하면 I/O 작업을 완료한 프로세스가 다른 실행 가능한 프로세스보다 우선 실행됩니다.
- -S SWITCH_ON_IO 플래그를 사용하지 않으면 일반적인 스케줄링 알고리즘에 따라 실행됩니다.
- I/O 작업을 완료한 프로세스가 CPU를 사용할 수 없는 경우 시스템은 다른 실행 가능한 프로세스를 실행하거나 유휴 상태가 됩니다.

```{Tip}
유휴 상태: CPU가 현재 사용 가능한 작업이 없는 상태.
```

## 8. 무작위 프로세스 실행

- 무작위로 생성된 프로세스를 실행할 때, -I IO_RUN_IMMEDIATE 플래그를 사용하면 I/O 작업 완료 시 해당 프로세스를 즉시 실행하고, -I IO_RUN_LATER 플래그를 사용하면 현재 실행 중인 프로세스가 계속 실행됩니다.
- -S SWITCH_ON_IO 플래그를 사용하면 프로세스가 I/O 작업을 기다리는 동안 다른 프로세스로 전환되고, -S SWITCH_ON_END 플래그를 사용하면 I/O 작업이 완료될 때까지 기다립니다. 추적 결과는 이러한 플래그 조합에 따라 달라질 것이다.
