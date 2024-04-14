# 개요

process-run.py라는 이 프로그램을 통해 프로세스가 CPU에서 실행되면서 프로세스 상태가 어떻게 변화하는지 볼 수 있습니다. 챕터에서 설명한 대로, 프로세스는 몇 가지 다른 상태를 가질 수 있습니다:

```sh
RUNNING - 프로세스가 지금 당장 CPU를 사용하고 있는 상태
READY   - 프로세스가 지금 당장 CPU를 사용할 수 있지만
          (아쉽게도) 다른 프로세스가 사용 중인 상태
BLOCKED - 프로세스가 I/O를 기다리고 있는 상태
          (예: 디스크에 요청을 보냈음)
DONE    - 프로세스가 실행을 완료한 상태
```

이 숙제에서는 프로그램이 실행되면서 이러한 프로세스 상태가 어떻게 변하는지 보고, 이를 통해 이런 것들이 어떻게 작동하는지 조금 더 잘 배우게 될 것입니다.

프로그램을 실행하고 옵션을 확인하려면 다음과 같이 하세요:

```sh
./process-run.py -h
```

이것이 작동하지 않으면 명령 앞에 `python`을 입력하세요:

```sh
python process-run.py -h
```

다음과 같은 내용이 표시되어야 합니다:

```sh
Usage: process-run.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -l PROCESS_LIST, --processlist=PROCESS_LIST
                        a comma-separated list of processes to run, in the
                        form X1:Y1,X2:Y2,... where X is the number of
                        instructions that process should run, and Y the
                        chances (from 0 to 100) that an instruction will use
                        the CPU or issue an IO
  -L IO_LENGTH, --iolength=IO_LENGTH
                        how long an IO takes
  -S PROCESS_SWITCH_BEHAVIOR, --switch=PROCESS_SWITCH_BEHAVIOR
                        when to switch between processes: SWITCH_ON_IO,
                        SWITCH_ON_END
  -I IO_DONE_BEHAVIOR, --iodone=IO_DONE_BEHAVIOR
                        type of behavior when IO ends: IO_RUN_LATER,
                        IO_RUN_IMMEDIATE
  -c                    compute answers for me
  -p, --printstats      print statistics at end; only useful with -c flag
                        (otherwise stats are not printed)
```

가장 중요하게 이해해야 할 옵션은 PROCESS_LIST(-l 또는 --processlist 플래그로 지정)입니다. 이 옵션은 실행 중인 각 프로그램(또는 '프로세스')이 정확히 무엇을 할지 지정합니다. 프로세스는 명령어들로 구성되며, 각 명령어는 다음 두 가지 중 하나만 할 수 있습니다:

- CPU 사용
- I/O 실행(및 완료 대기)

프로세스가 CPU를 사용할 때(전혀 I/O를 수행하지 않음), 단순히 CPU에서 RUNNING 상태와 실행 READY 상태를 번갈아 가며 있어야 합니다. 예를 들어, 다음은 하나의 프로그램만 실행되고 그 프로그램은 CPU만 사용하는(I/O를 전혀 수행하지 않음) 간단한 실행입니다.

```sh
./process-run.py -l 5:100
```

```sh
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)

prompt>
```

여기서 우리가 지정한 프로세스는 "5:100"인데, 이는 프로세스가 5개의 명령어로 구성되어야 하며 각 명령어가 CPU 명령어일 확률이 100%라는 것을 의미합니다.

-c 플래그를 사용하면 프로세스에 어떤 일이 일어나는지 확인할 수 있습니다. 이 플래그는 답을 계산해 줍니다:

```sh
./process-run.py -l 5:100 -c
```

```sh
Time     PID: 0        CPU        IOs
  1     RUN:cpu          1
  2     RUN:cpu          1
  3     RUN:cpu          1
  4     RUN:cpu          1
  5     RUN:cpu          1
```

이 결과는 그다지 흥미롭지 않습니다: 프로세스는 RUN 상태에 있다가 끝나며, 전체 실행 시간 동안 CPU를 사용하여 CPU를 계속 바쁘게 유지하고 I/O는 전혀 수행하지 않습니다.

두 개의 프로세스를 실행하여 조금 더 복잡하게 만들어 보겠습니다:

```sh
./process-run.py -l 5:100,5:100
```

```sh
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Process 1
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  Scheduler will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

이 경우 두 개의 다른 프로세스가 실행되며, 각각 다시 CPU만 사용합니다. 운영 체제가 이들을 실행하면 어떤 일이 일어날까요? 알아보겠습니다:

```sh
./process-run.py -l 5:100,5:100 -c
```

```sh
Time     PID: 0     PID: 1        CPU        IOs
  1     RUN:cpu      READY          1
  2     RUN:cpu      READY          1
  3     RUN:cpu      READY          1
  4     RUN:cpu      READY          1
  5     RUN:cpu      READY          1
  6        DONE    RUN:cpu          1
  7        DONE    RUN:cpu          1
  8        DONE    RUN:cpu          1
  9        DONE    RUN:cpu          1
 10        DONE    RUN:cpu          1
```

위에서 볼 수 있듯이, 먼저 "프로세스 ID"(또는 "PID") 0인 프로세스가 실행되고, 프로세스 1은 실행 준비(READY) 상태이지만 0이 끝날 때까지 기다립니다. 0이 끝나면 DONE 상태로 이동하고 1이 실행됩니다. 1이 끝나면 추적이 완료됩니다.

질문으로 넘어가기 전에 한 가지 예를 더 살펴보겠습니다. 이 예에서는 프로세스가 I/O 요청만 실행합니다. 여기서는 -L 플래그로 I/O가 완료되는 데 5개의 시간 단위가 걸린다고 지정합니다.

```sh
./process-run.py -l 3:0 -L 5
```

```sh
Produce a trace of what would happen when you run these processes:
Process 0
  io
  io_done
  io
  io_done
  io
  io_done

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

실행 추적이 어떻게 보일지 생각해 보세요. 알아보겠습니다:

```sh
./process-run.py -l 3:0 -L 5 -c
```

```sh
Time    PID: 0       CPU       IOs
  1         RUN:io             1
  2        BLOCKED                           1
  3        BLOCKED                           1
  4        BLOCKED                           1
  5        BLOCKED                           1
  6        BLOCKED                           1
  7*   RUN:io_done             1
  8         RUN:io             1
  9        BLOCKED                           1
 10        BLOCKED                           1
 11        BLOCKED                           1
 12        BLOCKED                           1
 13        BLOCKED                           1
 14*   RUN:io_done             1
 15         RUN:io             1
 16        BLOCKED                           1
 17        BLOCKED                           1
 18        BLOCKED                           1
 19        BLOCKED                           1
 20        BLOCKED                           1
 21*   RUN:io_done             1
```

보시다시피 프로그램은 세 개의 I/O만 실행합니다. 각 I/O가 실행될 때 프로세스는 BLOCKED 상태로 이동하고, 장치가 I/O 처리로 바쁜 동안 CPU는 유휴 상태입니다.

I/O 완료를 처리하기 위해 CPU 작업이 하나 더 실행됩니다. I/O 시작과 완료를 처리하는 단일 명령어는 특별히 현실적이지는 않지만 여기서는 단순성을 위해 사용됩니다.

몇 가지 전반적인 동작을 보기 위해 통계를 출력해 보겠습니다(위와 동일한 명령을 실행하되 -p 플래그 포함):

```sh
Stats: Total Time 21
Stats: CPU Busy 6 (28.57%)
Stats: IO Busy  15 (71.43%)
```

보시다시피 추적은 실행에 21번의 클럭 틱이 걸렸지만 CPU는 30% 미만의 시간 동안만 바빴습니다. 반면 I/O 장치는 꽤 바빴습니다. 일반적으로 모든 장치를 바쁘게 유지하는 것이 자원을 더 잘 활용하는 방법입니다.

몇 가지 다른 중요한 플래그가 있습니다:

```sh
  -s SEED, --seed=SEED  the random seed
    이것은 무작위로 많은 다양한 작업을 생성할 수 있는 방법을 제공합니다

  -L IO_LENGTH, --iolength=IO_LENGTH
    이것은 IO가 완료되는 데 걸리는 시간을 결정합니다(기본값은 5 틱)

  -S PROCESS_SWITCH_BEHAVIOR, --switch=PROCESS_SWITCH_BEHAVIOR
                        when to switch between processes: SWITCH_ON_IO, SWITCH_ON_END
    이것은 다른 프로세스로 전환하는 시점을 결정합니다:
    - SWITCH_ON_IO, 프로세스가 IO를 실행할 때 시스템이 전환합니다
    - SWITCH_ON_END, 현재 프로세스가 끝났을 때만 시스템이 전환합니다

  -I IO_DONE_BEHAVIOR, --iodone=IO_DONE_BEHAVIOR
                        type of behavior when IO ends: IO_RUN_LATER, IO_RUN_IMMEDIATE
    이것은 프로세스가 IO를 실행한 후 언제 실행되는지를 결정합니다:
    - IO_RUN_IMMEDIATE: 지금 바로 이 프로세스로 전환합니다
    - IO_RUN_LATER: 자연스러운 시점에 이 프로세스로 전환합니다
      (예: 프로세스 전환 동작에 따라)
```

이제 챕터 뒷부분의 질문에 답하면서 더 많은 것을 배워보세요.

## 키 용어 설명

- **프로세스(Process)**: 실행 중인 프로그램의 인스턴스입니다. 각 프로세스는 자체 메모리 공간을 가지며 CPU 시간을 할당받습니다.

- **프로세스 상태(Process State)**: 프로세스가 현재 어떤 상태에 있는지를 나타냅니다. 주요 상태로는 RUNNING(CPU 사용 중), READY(실행 준비 완료), BLOCKED(I/O 또는 다른 이벤트를 기다리는 중), DONE(실행 완료) 등이 있습니다.

- **문맥 교환(Context Switch)**: CPU가 한 프로세스에서 다른 프로세스로 전환하는 과정입니다. 이는 현재 프로세스의 상태를 저장하고 새로운 프로세스의 상태를 로드하는 것을 포함합니다. 문맥 교환은 오버헤드를 유발하므로 자주 발생하면 시스템 성능이 저하될 수 있습니다.

- **I/O(Input/Output)**: 프로세스가 디스크, 네트워크 등의 외부 장치와 통신하는 것을 말합니다. I/O 작업은 일반적으로 CPU 작업보다 훨씬 오래 걸립니다. 프로세스는 I/O 작업이 완료될 때까지 기다려야 하므로, I/O는 종종 프로세스 실행을 차단(block)합니다.

- **CPU 버스트(CPU Burst)**: 프로세스가 연속적으로 CPU를 사용하는 기간입니다. CPU 버스트 동안 프로세스는 계산을 수행하고 메모리에 접근합니다.

- **I/O 버스트(I/O Burst)**: 프로세스가 I/O 작업을 수행하는 기간입니다. I/O 버스트 동안 프로세스는 CPU를 사용하지 않고 I/O 작업이 완료되기를 기다립니다.

## 코드 예제

process-run.py 프로그램을 실행하는 예제입니다:

```sh
# 하나의 CPU 집약적 프로세스 실행
./process-run.py -l 5:100 -c

# 두 개의 CPU 집약적 프로세스 실행
./process-run.py -l 5:100,5:100 -c

# I/O 집약적 프로세스 실행
./process-run.py -l 3:0 -L 5 -c
```

첫 번째 예제는 5개의 명령어로 구성된 단일 CPU 집약적 프로세스를 실행합니다. 두 번째 예제는 각각 5개의 명령어로 구성된 두 개의 CPU 집약적 프로세스를 실행합니다. 마지막 예제는 3개의 I/O 작업을 수행하는 단일 프로세스를 실행하며, 각 I/O 작업은 5개의 시간 단위가 걸립니다.

-c 플래그는 프로세스 실행 추적을 출력하도록 지시합니다. 이를 통해 프로세스 상태가 시간에 따라 어떻게 변하는지 볼 수 있습니다.

예를 들어 I/O 집약적 프로세스의 출력은 다음과 같습니다:

```sh
Time    PID: 0       CPU       IOs
  1         RUN:io             1
  2        BLOCKED                           1
  3        BLOCKED                           1
  4        BLOCKED                           1
  5        BLOCKED                           1
  6        BLOCKED                           1
  7*   RUN:io_done             1
  8         RUN:io             1
  9        BLOCKED                           1
 10        BLOCKED                           1
 11        BLOCKED                           1
 12        BLOCKED                           1
 13        BLOCKED                           1
 14*   RUN:io_done             1
 15         RUN:io             1
 16        BLOCKED                           1
 17        BLOCKED                           1
 18        BLOCKED                           1
 19        BLOCKED                           1
 20        BLOCKED                           1
 21*   RUN:io_done             1
```

이 출력은 프로세스가 I/O 요청을 실행할 때마다 BLOCKED 상태로 전환되고, I/O가 완료될 때까지 그 상태로 유지됨을 보여줍니다. I/O가 완료되면 프로세스는 io_done 명령어를 실행하여 I/O 완료를 처리합니다.

## 결론

이 숙제는 프로세스 실행과 프로세스 상태 전이의 기본 개념을 탐구합니다. process-run.py 프로그램을 사용하여 다양한 시나리오를 시뮬레이션하고 결과를 관찰함으로써, 이러한 개념이 실제로 어떻게 작동하는지 더 잘 이해할 수 있습니다.

핵심은 프로세스가 CPU와 I/O 장치 간에 어떻게 시간을 할당하는지 이해하는 것입니다. I/O는 프로세스 실행을 차단하므로 I/O 동안 다른 프로세스를 실행하는 것이 종종 더 효율적입니다. 그러나 문맥 교환 오버헤드로 인해 너무 자주 프로세스를 전환하는 것은 성능에 해로울 수 있습니다.

이 숙제는 또한 프로세스 스케줄링과 자원 활용의 중요성을 보여줍니다. CPU와 I/O 장치를 최대한 활용하려면 적절한 프로세스 믹스와 스케줄링 정책이 필요합니다.

프로세스와 시스템 성능을 최적화하는 것은 운영 체제 설계의 핵심 과제입니다. 이 기본 개념들을 이해함으로써 더 효율적이고 반응성이 뛰어난 시스템을 구축하기 위한 토대를 마련할 수 있습니다.

앞으로도 계속 탐구하고 실험해 보세요! 다양한 프로세스 워크로드와 시스템 설정을 시도해 보고 결과를 관찰하세요. 가장 중요한 것은 호기심을 갖고 배우려는 자세입니다. 행운을 빕니다!
