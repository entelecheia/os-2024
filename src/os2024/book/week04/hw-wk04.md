# Homework: 비례 배분

| [비례 배분](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf) | [lottery.py 실행](https://github.com/entelecheia/os-2024/tree/main/src/ostep-homework/cpu-sched-lottery) |

```{note}
본 과제는 프로젝트를 위해 생성한 본인의 개별 폴더에 제출하기 바랍니다. 제출기한은 5/8(수)입니다.
```

## 개요

이 프로그램인 `lottery.py`는 복권 스케줄러(Lottery Scheduler)가 어떻게 동작하는지 보여줍니다. 항상 그렇듯이, 프로그램을 실행하는 데는 두 단계가 있습니다. 먼저 `-c` 플래그 없이 실행하면 정답을 공개하지 않고 어떤 문제를 해결해야 하는지 보여줍니다.

```sh
./lottery.py -j 2 -s 0
```

```sh
...
Here is the job list, with the run time of each job:
  Job 0 ( length = 8, tickets = 75 )
  Job 1 ( length = 4, tickets = 25 )

Here is the set of random numbers you will need (at most):
Random 511275
Random 404934
Random 783799
Random 303313
Random 476597
Random 583382
Random 908113
Random 504687
Random 281838
Random 755804
Random 618369
Random 250506
```

이런 식으로 시뮬레이터를 실행하면, 먼저 일부 무작위 작업(여기서는 길이 8과 4)을 각각 일정 수의 티켓(여기서는 각각 75와 25)과 함께 할당합니다. 또한 시뮬레이터는 복권 스케줄러가 무엇을 할지 결정하는 데 필요한 난수 목록을 제공합니다. 난수는 0부터 큰 숫자 사이에서 선택되므로, 복권 당첨자를 계산하기 위해 모듈로 연산자를 사용해야 합니다(즉, 당첨자는 이 난수를 시스템의 총 티켓 수로 나눈 나머지와 같아야 함).

`-c` 플래그와 함께 실행하면 정확히 계산해야 하는 내용이 표시됩니다:

```sh
./lottery.py -j 2 -s 0 -c
```

```sh
...
** Solutions **
Random 511275 -> Winning ticket 75 (of 100) -> Run 1
  Jobs:  (  job:0 timeleft:8 tix:75 ) (* job:1 timeleft:4 tix:25 )
Random 404934 -> Winning ticket 34 (of 100) -> Run 0
  Jobs:  (* job:0 timeleft:8 tix:75 ) (  job:1 timeleft:3 tix:25 )
Random 783799 -> Winning ticket 99 (of 100) -> Run 1
  Jobs:  (  job:0 timeleft:7 tix:75 ) (* job:1 timeleft:3 tix:25 )
Random 303313 -> Winning ticket 13 (of 100) -> Run 0
  Jobs:  (* job:0 timeleft:7 tix:75 ) (  job:1 timeleft:2 tix:25 )
Random 476597 -> Winning ticket 97 (of 100) -> Run 1
  Jobs:  (  job:0 timeleft:6 tix:75 ) (* job:1 timeleft:2 tix:25 )
Random 583382 -> Winning ticket 82 (of 100) -> Run 1
  Jobs:  (  job:0 timeleft:6 tix:75 ) (* job:1 timeleft:1 tix:25 )
--> JOB 1 DONE at time 6
Random 908113 -> Winning ticket 13 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:6 tix:75 ) (  job:1 timeleft:0 tix:--- )
Random 504687 -> Winning ticket 12 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:5 tix:75 ) (  job:1 timeleft:0 tix:--- )
Random 281838 -> Winning ticket 63 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:4 tix:75 ) (  job:1 timeleft:0 tix:--- )
Random 755804 -> Winning ticket 29 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:3 tix:75 ) (  job:1 timeleft:0 tix:--- )
Random 618369 -> Winning ticket 69 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:2 tix:75 ) (  job:1 timeleft:0 tix:--- )
Random 250506 -> Winning ticket 6 (of 75) -> Run 0
  Jobs:  (* job:0 timeleft:1 tix:75 ) (  job:1 timeleft:0 tix:--- )
--> JOB 0 DONE at time 12
```

이 추적에서 볼 수 있듯이, 여러분이 해야 할 일은 난수를 사용하여 어떤 티켓이 당첨되었는지 파악하는 것입니다. 그런 다음 당첨 티켓을 바탕으로 어떤 작업을 실행해야 하는지 알아내세요. 모든 작업의 실행이 완료될 때까지 이 작업을 반복하세요. 이것은 매우 간단합니다 -- 여러분은 복권 스케줄러가 하는 일을 직접 에뮬레이션하는 것입니다!

이를 명확히 하기 위해 위의 예에서 내린 첫 번째 결정을 살펴보겠습니다. 이 시점에서 우리는 두 개의 작업(런타임이 8이고 티켓이 75개인 작업 0과 런타임이 4이고 티켓이 25개인 작업 1)을 가지고 있습니다. 주어진 첫 번째 난수는 511275입니다. 시스템에 총 100개의 티켓이 있으므로 511275 % 100은 75이며, 따라서 75가 우리의 당첨 티켓입니다.

티켓 75가 당첨되면 당첨 티켓을 찾을 때까지 작업 목록을 검색하기만 하면 됩니다. 작업 0에 대한 첫 번째 항목에는 75개의 티켓(0부터 74까지)이 있으므로 당첨되지 않습니다. 다음 항목은 작업 1에 대한 것이므로 우리는 승자를 찾았고, 따라서 작업 1을 양자 길이(이 예에서는 1) 동안 실행합니다. 이 모든 내용은 다음과 같이 출력됩니다:

```sh
Random 511275 -> Winning ticket 75 (of 100) -> Run 1
  Jobs:  (  job:0 timeleft:8 tix:75 ) (* job:1 timeleft:4 tix:25 )
```

보시다시피, 첫 번째 줄은 무슨 일이 일어났는지 요약하고, 두 번째 줄은 전체 작업 대기열을 보여주며, 어떤 작업이 선택되었는지 \*로 표시합니다.

시뮬레이터에는 대부분 자명한 몇 가지 다른 옵션이 있습니다. 특히 `-l/--jlist` 플래그를 사용하여 무작위로 생성된 작업 목록 대신 작업과 티켓 값의 정확한 집합을 지정할 수 있습니다.

```sh
./lottery.py -h
```

```sh
Usage: lottery.py [options]

Options:
  -h, --help
      show this help message and exit
  -s SEED, --seed=SEED
      the random seed
  -j JOBS, --jobs=JOBS
      number of jobs in the system
  -l JLIST, --jlist=JLIST
      instead of random jobs, provide a comma-separated list
      of run times and ticket values (e.g., 10:100,20:100
      would have two jobs with run-times of 10 and 20, each
      with 100 tickets)
  -m MAXLEN, --maxlen=MAXLEN
      max length of job
  -T MAXTICKET, --maxtick=MAXTICKET
      maximum ticket value, if randomly assigned
  -q QUANTUM, --quantum=QUANTUM
      length of time slice
  -c, --compute
      compute answers for me
```

### 주요 용어 설명

- 복권 스케줄링(Lottery Scheduling): 각 프로세스에 티켓을 할당하고, 무작위로 티켓을 뽑아 다음에 실행할 프로세스를 선택하는 스케줄링 알고리즘입니다. 티켓이 많은 프로세스일수록 선택될 확률이 높아집니다.

- 티켓(Ticket): 복권 스케줄링에서 각 프로세스에 할당된 수치로, 프로세스의 상대적 중요도 또는 자원 할당 비율을 나타냅니다. 티켓이 많을수록 CPU 시간을 더 많이 할당받습니다.

- 양자(Quantum): 시분할 시스템에서 각 프로세스에 할당된 연속적인 CPU 시간의 단위입니다. 프로세스는 양자 시간 동안 실행되고, 양자가 끝나면 선점됩니다.

- 난수 시드(Random Seed): 난수 생성기를 초기화하는 데 사용되는 값입니다. 동일한 시드를 사용하면 동일한 난수 시퀀스가 생성됩니다.

- 모듈로 연산(Modulo Operation): 두 수의 나눗셈 후 나머지를 구하는 연산입니다. 예를 들어, `7 % 3`의 결과는 1입니다.

## 과제 (시뮬레이션)

### 질문

1. 3개의 작업과 난수 시드 1, 2, 3으로 시뮬레이션한 솔루션을 계산하세요.

2. 이제 두 개의 특정 작업으로 실행해 보세요: 각각 길이는 10이지만 하나(작업 0)는 티켓이 1장이고 다른 하나(작업 1)는 100장입니다(예: `-l 10:1,10:100`). 티켓의 수가 이렇게 불균형할 때 어떤 일이 일어날까요? 작업 1이 완료되기 전에 작업 0이 실행될 수 있을까요? 얼마나 자주 그럴까요? 일반적으로 이러한 티켓 불균형이 복권 스케줄링의 동작에 어떤 영향을 미칠까요?

3. 길이가 100이고 티켓 할당이 100으로 동일한 두 개의 작업(`-l 100:100,100:100`)으로 실행할 때 스케줄러는 얼마나 불공정할까요? 몇 가지 다른 난수 시드로 실행하여 (확률적) 답을 결정하세요; 한 작업이 다른 작업보다 얼마나 일찍 끝나는지에 따라 불공정성이 결정됩니다.
