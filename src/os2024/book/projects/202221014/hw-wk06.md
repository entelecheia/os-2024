# Week 6 Homework

- 각 시드에 대해 다음과 같은 명령을 사용하여 실행한다.
- 각 가상 주소에 대해 주어진 기준(register)과 한계(limit)를 참고하여 물리 주소로의 변환 가능 여부를 판단한다.

## 1. 시드 1, 2, 3으로 실행

```
03:07:45 with qhddybuntu in src/ostep-homework/vm-mechanism at G via os2024 …
➜ ./relocation.py -s 1


ARG seed 1
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x0000363c (decimal 13884)
  Limit  : 290

Virtual Address Trace
  VA  0: 0x0000030e (decimal:  782) --> PA or segmentation violation?
  VA  1: 0x00000105 (decimal:  261) --> PA or segmentation violation?
  VA  2: 0x000001fb (decimal:  507) --> PA or segmentation violation?
  VA  3: 0x000001cc (decimal:  460) --> PA or segmentation violation?
  VA  4: 0x0000029b (decimal:  667) --> PA or segmentation violation?

03:07:47 with qhddybuntu in src/ostep-homework/vm-mechanism at G via os2024 …
➜ ./relocation.py -s 2


ARG seed 2
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00003ca9 (decimal 15529)
  Limit  : 500

Virtual Address Trace
  VA  0: 0x00000039 (decimal:   57) --> PA or segmentation violation?
  VA  1: 0x00000056 (decimal:   86) --> PA or segmentation violation?
  VA  2: 0x00000357 (decimal:  855) --> PA or segmentation violation?
  VA  3: 0x000002f1 (decimal:  753) --> PA or segmentation violation?
  VA  4: 0x000002ad (decimal:  685) --> PA or segmentation violation?

03:07:55 with qhddybuntu in src/ostep-homework/vm-mechanism at G via os2024 …
➜ ./relocation.py -s 3


ARG seed 3
ARG address space size 1k
ARG phys mem size 16k

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

### 시드 1의 결과 분석

- **기준 값(Base):** 13884
- **한계 값(Limit):** 290
- **가상 주소(VA) 및 변환 결과:**
  - **VA 0:** 782 (범위 초과: 세그멘테이션 위반)
  - **VA 1:** 261 (유효: 물리 주소 = 13884 + 261 = 14145)
  - **VA 2:** 507 (범위 초과: 세그멘테이션 위반)
  - **VA 3:** 460 (유효: 물리 주소 = 13884 + 460 = 14344)
  - **VA 4:** 667 (범위 초과: 세그멘테이션 위반)

### 시드 2의 결과 분석

- **기준 값(Base):** 15529
- **한계 값(Limit):** 500
- **가상 주소(VA) 및 변환 결과:**
  - **VA 0:** 57 (유효: 물리 주소 = 15529 + 57 = 15586)
  - **VA 1:** 86 (유효: 물리 주소 = 15529 + 86 = 15615)
  - **VA 2:** 855 (범위 초과: 세그멘테이션 위반)
  - **VA 3:** 753 (범위 초과: 세그멘테이션 위반)
  - **VA 4:** 685 (범위 초과: 세그멘테이션 위반)

### 시드 3의 결과 분석

- **기준 값(Base):** 8916
- **한계 값(Limit):** 316
- **가상 주소(VA) 및 변환 결과:**

  - **VA 0:** 378 (범위 초과: 세그멘테이션 위반)
  - **VA 1:** 618 (범위 초과: 세그멘테이션 위반)
  - **VA 2:** 640 (범위 초과: 세그멘테이션 위반)
  - **VA 3:** 67 (유효: 물리 주소 = 8916 + 67 = 8983)
  - **VA 4:** 13 (유효: 물리 주소 = 8916 + 13 = 8929)

- 위 분석을 통해 각 시드에서 생성된 가상 주소의 유효성을 확인하고 해당하는 물리 주소로의 변환을 계산할 수 있다. 각 시드에 대한 변환 결과는 주어진 한계 값 내에서의 유효성 여부에 따라 결정된다. 범위를 초과하는 가상 주소는 세그멘테이션 위반으로 처리되어야 한다.

## 2. 한계 레지스터의 적절한 값 설정

- 10개의 가상 주소를 생성하고 모든 주소가 범위 내에 있도록 설정하려면 다음 명령어를 사용한다.

```
03:07:58 with qhddybuntu in src/ostep-homework/vm-mechanism at G via os2024 …
➜ ./relocation.py -s 0 -n 10 -c


ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x000001ae (decimal:  430) --> VALID: 0x00003230 (decimal: 12848)
  VA  1: 0x00000109 (decimal:  265) --> VALID: 0x0000318b (decimal: 12683)
  VA  2: 0x0000020b (decimal:  523) --> SEGMENTATION VIOLATION
  VA  3: 0x0000019e (decimal:  414) --> VALID: 0x00003220 (decimal: 12832)
  VA  4: 0x00000322 (decimal:  802) --> SEGMENTATION VIOLATION
  VA  5: 0x00000136 (decimal:  310) --> VALID: 0x000031b8 (decimal: 12728)
  VA  6: 0x000001e8 (decimal:  488) --> SEGMENTATION VIOLATION
  VA  7: 0x00000255 (decimal:  597) --> SEGMENTATION VIOLATION
  VA  8: 0x000003a1 (decimal:  929) --> SEGMENTATION VIOLATION
  VA  9: 0x00000204 (decimal:  516) --> SEGMENTATION VIOLATION
```

- 위에서 제공된 정보에 따르면, 시드 0을 사용할 때 가장 큰 생성된 가상 주소는 929다. 따라서 한계 레지스터 값으로는 최소 929 이상을 설정해야 모든 주소가 유효 범위 내에 포함된다.

## 3. 주소 공간이 여전히 물리 메모리에 전체적으로 맞도록 기준을 설정할 수 있는 최대값

```
03:11:38 with qhddybuntu in src/ostep-homework/vm-mechanism at G via os2024 …
➜ ./relocation.py -s 1 -n 10 -l 100 -c


ARG seed 1
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00000899 (decimal 2201)
  Limit  : 100

Virtual Address Trace
  VA  0: 0x00000363 (decimal:  867) --> SEGMENTATION VIOLATION
  VA  1: 0x0000030e (decimal:  782) --> SEGMENTATION VIOLATION
  VA  2: 0x00000105 (decimal:  261) --> SEGMENTATION VIOLATION
  VA  3: 0x000001fb (decimal:  507) --> SEGMENTATION VIOLATION
  VA  4: 0x000001cc (decimal:  460) --> SEGMENTATION VIOLATION
  VA  5: 0x0000029b (decimal:  667) --> SEGMENTATION VIOLATION
  VA  6: 0x00000327 (decimal:  807) --> SEGMENTATION VIOLATION
  VA  7: 0x00000060 (decimal:   96) --> VALID: 0x000008f9 (decimal: 2297)
  VA  8: 0x0000001d (decimal:   29) --> VALID: 0x000008b6 (decimal: 2230)
  VA  9: 0x00000357 (decimal:  855) --> SEGMENTATION VIOLATION
```

물리 메모리가 16k (즉, 16384 바이트)이고 한계 레지스터 값이 100으로 설정되어 있으므로, 가상 주소 공간의 최대 기준 값을 계산할 수 있다. 기준 값은 한계 값이 주어진 물리 메모리 내에 포함되도록 설정되어야 한다. 따라서 최대 기준 값은 물리 메모리의 크기에서 한계 레지스터 값을 뺀 값인 16284 (16384 - 100)가 된다. 이 값은 물리 메모리 범위 내에서 주어진 한계에 따라 모든 가상 주소를 유효하게 매핑할 수 있는 최대 기준 값을 나타낸다.

## 4. 위와 동일한 문제 중 일부를 실행해 보세요. 단, 더 큰 주소 공간(-a)과 물리 메모리(-p)를 사용하세요.

```bash
./relocation.py -a 64k -p 128k -s 1 -n 10 -c
```

- 이 명령은 64k 주소 공간과 128k 물리 메모리를 설정하여 시뮬레이션을 수행한다.

```
ARG seed 1
ARG address space size 64k
ARG phys mem size 128k

Base-and-Bounds register information:

  Base   : 0x0001b1e2 (decimal 111074)
  Limit  : 18585

Virtual Address Trace
  VA  0: 0x0000c386 (decimal: 50054) --> SEGMENTATION VIOLATION
  VA  1: 0x0000414c (decimal: 16716) --> VALID: 0x0001f32e (decimal: 127790)
  VA  2: 0x00007ed4 (decimal: 32468) --> SEGMENTATION VIOLATION
  VA  3: 0x00007311 (decimal: 29457) --> SEGMENTATION VIOLATION
  VA  4: 0x0000a6ce (decimal: 42702) --> SEGMENTATION VIOLATION
  VA  5: 0x0000c9e9 (decimal: 51689) --> SEGMENTATION VIOLATION
  VA  6: 0x00001807 (decimal: 6151) --> VALID: 0x0001c9e9 (decimal: 117225)
  VA  7: 0x00000741 (decimal: 1857) --> VALID: 0x0001b923 (decimal: 112931)
  VA  8: 0x0000d5f4 (decimal: 54772) --> SEGMENTATION VIOLATION
  VA  9: 0x00006ec9 (decimal: 28361) --> SEGMENTATION VIOLATION
```

### 가상 주소에 대한 분석

- **VA 0:** 50054 -> 범위 초과 (세그멘테이션 위반)
- **VA 1:** 16716 -> 유효: 물리 주소 = 111074 + 16716 = 127790
- **VA 2:** 32468 -> 범위 초과 (세그멘테이션 위반)
- **VA 3:** 29457 -> 범위 초과 (세그멘테이션 위반)
- **VA 4:** 42702 -> 범위 초과 (세그멘테이션 위반)
- **VA 5:** 51689 -> 범위 초과 (세그멘테이션 위반)
- **VA 6:** 6151 -> 유효: 물리 주소 = 111074 + 6151 = 117225
- **VA 7:** 1857 -> 유효: 물리 주소 = 111074 + 1857 = 112931
- **VA 8:** 54772 -> 범위 초과 (세그멘테이션 위반)
- **VA 9:** 28361 -> 범위 초과 (세그멘테이션 위반)

### 결과

위 결과에서, 대부분의 가상 주소는 한계 레지스터 값 18585를 초과하여 세그멘테이션 위반을 일으켰다. 유효한 변환은 VA 1, VA 6, VA 7에서만 발생했다. 이는 주어진 설정에서 가상 주소의 대부분이 한계 값을 초과하여 물리 메모리 범위를 벗어난 것을 나타낸다.

### 중요점

1. **주소 공간 및 메모리 크기:** 더 큰 주소 공간과 물리 메모리 설정은 메모리 관리의 복잡성을 증가한다.
2. **한계 레지스터의 중요성:** 한계 레지스터 값이 너무 낮으면, 유효한 가상 주소가 물리 메모리의 실제 가능 범위를 초과할 수 있다.
