# Homework: 주소 변환의 원리

| [주소 변환의 원리](http://www.cs.wisc.edu/~remzi/OSTEP/vm-mechanism.pdf) | [relocation.py 실행](https://github.com/chu-aie/os-2024/tree/main/src/ostep-homework/vm-mechanism) |

```{note}
본 과제는 프로젝트를 위해 생성한 본인의 개별 폴더에 제출하기 바랍니다. 제출기한은 5/8(수)입니다.
```

## 개요

이 프로그램은 기준 레지스터와 한계 레지스터가 있는 시스템에서 주소 변환이 어떻게 수행되는지 보여줍니다. 이전과 마찬가지로, 기준과 한계에 대한 이해를 테스트하기 위해 프로그램을 실행하는 데는 두 단계가 있습니다. 먼저, -c 플래그 없이 실행하여 일련의 변환을 생성하고 직접 주소 변환을 올바르게 수행할 수 있는지 확인합니다. 완료되면 -c 플래그와 함께 실행하여 답을 확인하세요.

이 과제에서는 힙과 스택이 공간의 반대쪽 끝에 있는 우리의 규범적인 주소 공간과는 약간 다른 주소 공간을 가정할 것입니다. 오히려 주소 공간에 코드 섹션이 있고, 그 다음에 고정 크기(작은)의 스택이 있고, 그 바로 다음에 아래쪽으로 증가하는 힙이 있다고 가정할 것입니다. 아래 그림에서 볼 수 있듯이 말이죠. 이 구성에서는 주소 공간의 상위 영역을 향해 성장하는 한 방향만 있습니다.

```sh
  -------------- 0KB
  |    Code    |
  -------------- 2KB
  |   Stack    |
  -------------- 4KB
  |    Heap    |
  |     |      |
  |     v      |
  -------------- 7KB
  |   (free)   |
  |     ...    |
```

그림에서 한계 레지스터는 7KB로 설정될 것입니다. 이는 주소 공간의 끝을 나타내기 때문입니다. 한계 내의 모든 주소에 대한 참조는 합법적인 것으로 간주됩니다. 이 값을 초과하는 참조는 범위를 벗어난 것이므로 하드웨어는 예외를 발생시킬 것입니다.

기본 플래그로 실행하려면 명령줄에 relocation.py를 입력하세요. 결과는 다음과 같아야 합니다:

```sh
./relocation.py
```

```sh
...
Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> PA or violation?
  VA  1: 0x0109 (decimal:265) -> PA or violation?
  VA  2: 0x020b (decimal:523) -> PA or violation?
  VA  3: 0x019e (decimal:414) -> PA or violation?
  VA  4: 0x0322 (decimal:802) -> PA or violation?
```

각 가상 주소에 대해 변환되는 물리 주소를 적거나 범위를 벗어난 주소(세그멘테이션 위반)임을 적으세요. 이 문제의 경우 주어진 크기의 간단한 가상 주소 공간을 가정해야 합니다.

보시다시피, 이 과제는 단순히 무작위 가상 주소를 생성합니다. 각각에 대해 범위 내에 있는지 확인하고, 그렇다면 어떤 물리 주소로 변환되는지 결정해야 합니다. -c("compute this for me") 플래그와 함께 실행하면 이러한 변환 결과, 즉 유효한지 여부와 유효한 경우 결과 물리 주소를 알려줍니다. 편의를 위해 모든 숫자는 16진수와 10진수로 제공됩니다.

```sh
./relocation.py -c
```

```sh
...
Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x00003230 (dec:12848)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x00003220 (dec:12832)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
```

기준 주소가 12418(10진수)인 경우, 주소 430은 범위 내에 있습니다(즉, 한계 레지스터 472보다 작음). 따라서 12418에 430을 더한 12848로 변환됩니다. 위에 표시된 주소 중 일부는 한계를 초과하므로 범위를 벗어납니다(523, 802). 꽤 간단하죠? 실제로 이것이 기준과 한계의 장점 중 하나입니다. 너무나 단순하다는 거죠!

진행 상황을 더 잘 제어할 수 있는 몇 가지 플래그가 있습니다:

```sh
./relocation.py -h
```

```sh
Usage: relocation.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -a ASIZE, --asize=ASIZE address space size (e.g., 16, 64k, 32m)
  -p PSIZE, --physmem=PSIZE physical memory size (e.g., 16, 64k)
  -n NUM, --addresses=NUM # of virtual addresses to generate
  -b BASE, --b=BASE     value of base register
  -l LIMIT, --l=LIMIT   value of limit register
  -c, --compute         compute answers for me
```

특히 가상 주소 공간 크기(-a), 물리 메모리 크기(-p), 생성할 가상 주소 수(-n), 이 프로세스의 기준 및 한계 레지스터 값(-b 및 -l)을 제어할 수 있습니다.

### 주요 용어 설명

- 기준 레지스터(Base Register): 프로세스의 주소 공간이 물리 메모리의 어디에서 시작되는지를 나타내는 레지스터입니다. 가상 주소에 기준 값을 더하여 물리 주소를 얻습니다.

- 한계 레지스터(Limit Register): 프로세스의 주소 공간 크기를 나타내는 레지스터입니다. 가상 주소가 한계 값보다 크면 주소 범위를 벗어난 것으로 간주되어 세그멘테이션 폴트가 발생합니다.

- 주소 공간(Address Space): 프로세스가 사용할 수 있는 메모리 주소의 범위입니다. 각 프로세스는 자신만의 독립적인 주소 공간을 가지며, 이는 가상 메모리에 매핑됩니다.

- 세그멘테이션 폴트(Segmentation Fault): 프로세스가 할당되지 않은 메모리 영역이나 접근 권한이 없는 메모리 영역에 접근하려고 할 때 발생하는 오류입니다.

- 가상 주소(Virtual Address): 프로세스가 사용하는 메모리 주소로, 실제 물리 메모리 주소와는 다를 수 있습니다. 가상 주소는 가상 메모리 시스템에 의해 물리 주소로 변환됩니다.

- 물리 주소(Physical Address): 실제 물리 메모리에서 데이터가 위치한 주소입니다. 가상 주소는 MMU(Memory Management Unit)에 의해 물리 주소로 변환됩니다.

## 과제 (시뮬레이션)

### 질문

1. 시드 1, 2, 3으로 실행하고 프로세스에서 생성된 각 가상 주소가 범위 내에 있는지 또는 범위를 벗어났는지 계산하세요. 범위 내라면 변환을 계산하세요.

2. 다음 플래그로 실행하세요: `-s 0 -n 10`. 생성된 모든 가상 주소가 범위 내에 있도록 하려면 `-l`(한계 레지스터)을 어떤 값으로 설정해야 할까요?

3. 다음 플래그로 실행하세요: `-s 1 -n 10 -l 100`. 주소 공간이 여전히 물리 메모리에 전체적으로 맞도록 기준을 설정할 수 있는 최대값은 얼마인가요?

4. 위와 동일한 문제 중 일부를 실행해 보세요. 단, 더 큰 주소 공간(`-a`)과 물리 메모리(`-p`)를 사용하세요.
