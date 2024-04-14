# Homework: 스케줄링

| [스케줄링 기초](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf) | [scheduler.py 실행](https://github.com/chu-aie/os-2024/tree/main/src/ostep-homework/cpu-sched) |

```{note}
본 과제는 프로젝트를 위해 생성한 본인의 개별 폴더에 제출하기 바랍니다.
```

## 소개

`scheduler.py` 프로그램을 통해 다양한 스케줄러가 응답 시간, 반환 시간, 총 대기 시간 등의 스케줄링 메트릭 하에서 어떻게 수행되는지 확인할 수 있습니다. 자세한 내용은 README를 참조하세요.

## 질문

1. SJF와 FIFO 스케줄러로 길이가 200인 세 개의 작업을 실행할 때의 응답 시간과 반환 시간을 계산하세요.

2. 이제 작업의 길이를 다르게 하여 동일한 작업을 수행해 보세요: 100, 200, 300.

3. 이제 RR 스케줄러와 시간 할당량(time-slice)을 1로 하여 동일한 작업을 수행해 보세요.

4. 어떤 유형의 워크로드에 대해 SJF가 FIFO와 동일한 반환 시간을 제공하나요?

5. 어떤 유형의 워크로드와 시간 할당량에 대해 SJF가 RR과 동일한 응답 시간을 제공하나요?

6. 작업 길이가 증가함에 따라 SJF에서 응답 시간은 어떻게 되나요? 시뮬레이터를 사용하여 이 추세를 보여줄 수 있나요?

7. 시간 할당량이 증가함에 따라 RR에서 응답 시간은 어떻게 되나요? N개의 작업이 주어졌을 때 최악의 응답 시간을 제공하는 수식을 작성할 수 있나요?

## 용어 설명

- **스케줄링(Scheduling)**: 프로세스나 스레드에 CPU 시간을 할당하는 프로세스입니다. 스케줄러는 시스템의 성능, 공정성, 안정성 등의 목표를 달성하기 위해 특정 스케줄링 알고리즘을 사용합니다.

- **선입 선처리(FIFO, First-In-First-Out)**: 작업이 도착한 순서대로 처리되는 간단한 스케줄링 알고리즘입니다. 먼저 도착한 작업이 먼저 처리됩니다.

- **최단 작업 우선(SJF, Shortest Job First)**: 실행 시간이 가장 짧은 작업을 먼저 처리하는 스케줄링 알고리즘입니다. SJF는 평균 대기 시간을 최소화하지만 실행 시간을 미리 알아야 한다는 단점이 있습니다.

- **라운드 로빈(RR, Round-Robin)**: 각 프로세스에 동일한 시간 할당량(일반적으로 매우 작음)을 할당하고 순서대로 돌아가면서 실행하는 스케줄링 알고리즘입니다. 프로세스는 시간 할당량을 다 쓰면 선점됩니다.

- **응답 시간(Response Time)**: 작업이 도착한 시점부터 처음으로 CPU를 얻어 실행을 시작할 때까지 걸린 시간입니다. 대화형 시스템에서는 응답 시간이 사용자 경험에 중요합니다.

- **반환 시간(Turnaround Time)**: 작업이 도착한 시점부터 완료될 때까지 걸린 총 시간입니다. 배치 시스템에서는 반환 시간이 중요한 성능 지표입니다.

- **대기 시간(Wait Time)**: 작업이 실행 가능한 상태로 준비 큐에서 기다리는 데 소요된 총 시간입니다. 대기 시간은 시스템의 부하와 스케줄링 알고리즘의 효율성을 나타내는 지표입니다.

## 코드 예제

다음은 Python으로 작성된 간단한 FIFO 스케줄러의 예입니다:

```python
def fifo_scheduler(processes):
    clock = 0
    total_wait_time = 0
    total_turnaround_time = 0
    total_response_time = 0

    for process in processes:
        arrival_time, burst_time = process

        if clock < arrival_time:
            clock = arrival_time

        response_time = clock - arrival_time
        wait_time = response_time
        turnaround_time = wait_time + burst_time

        clock += burst_time

        total_wait_time += wait_time
        total_turnaround_time += turnaround_time
        total_response_time += response_time

    avg_wait_time = total_wait_time / len(processes)
    avg_turnaround_time = total_turnaround_time / len(processes)
    avg_response_time = total_response_time / len(processes)

    return avg_wait_time, avg_turnaround_time, avg_response_time
```

이 함수는 프로세스 리스트를 입력으로 받습니다. 각 프로세스는 도착 시간과 실행 시간의 튜플로 표현됩니다. 스케줄러는 각 프로세스의 응답 시간, 대기 시간, 반환 시간을 계산하고, 이를 사용하여 평균 메트릭을 계산합니다.

다음은 이 스케줄러를 사용하는 방법입니다:

```python
processes = [(0, 10), (1, 5), (2, 8)]
avg_wait, avg_turnaround, avg_response = fifo_scheduler(processes)
print(f"Average Wait Time: {avg_wait:.2f}")
print(f"Average Turnaround Time: {avg_turnaround:.2f}")
print(f"Average Response Time: {avg_response:.2f}")
```

이 코드는 세 개의 프로세스로 구성된 워크로드를 정의하고 FIFO 스케줄러를 사용하여 스케줄링합니다. 그런 다음 결과 메트릭을 출력합니다.

물론 이는 매우 간단한 예시일 뿐입니다. 실제 시스템에서는 더 복잡한 스케줄링 알고리즘을 사용하며, 프로세스의 우선순위, I/O 요구 사항 등 다양한 요소를 고려합니다. 하지만 이 예시는 스케줄링 메트릭을 계산하는 기본 아이디어를 보여줍니다.

## 결론

이 과제는 다양한 스케줄링 알고리즘의 동작과 성능 특성을 탐구할 수 있는 기회를 제공합니다. FIFO, SJF, RR과 같은 기본 알고리즘을 구현하고 다양한 워크로드 하에서 시뮬레이션해 봄으로써, 이들이 응답 시간, 반환 시간, 대기 시간 등의 메트릭에 어떤 영향을 미치는지 이해할 수 있습니다.

스케줄링은 운영 체제 설계에서 매우 중요한 주제입니다. 효과적인 스케줄링은 시스템 자원을 효율적으로 활용하고, 사용자에게 좋은 성능과 응답성을 제공하며, 다양한 유형의 워크로드를 공정하게 처리할 수 있게 해줍니다. 동시에 스케줄링은 복잡한 트레이드오프를 내포하고 있어서, 어떤 알고리즘도 모든 상황에 이상적이지는 않습니다.

이 과제를 통해 얻은 통찰력을 실제 시스템과 연결해 보세요. 스마트폰, 노트북, 서버 등 우리 주변의 디바이스들이 어떻게 수많은 프로세스와 사용자 요청을 스케줄링하는지 생각해 보세요. 현대 운영 체제는 정교한 스케줄링 체계를 사용하여 성능, 공정성, 에너지 효율성 등 다양한 목표의 균형을 맞춥니다.

운영 체제 스케줄링의 세계는 광범위하고 흥미진진합니다. 이 과제가 그 세계를 탐험하는 여러분의 여정을 시작하는 데 도움이 되었기를 바랍니다. 계속해서 배우고, 실험하고, 탐구하세요!
