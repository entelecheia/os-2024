# Week 6 Homework

질문 1 시드 1, 2, 3으로 실행하고 프로세스에서 생성된 각 가상 주소가 범위 내에 있는지 또는 범위를 벗어났는지 계산하세요. 범위 내라면 변환을 계산하세요.

```
./relocation.py -s 1 -c
```

```
  Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x00003230 (dec:12848)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x00003220 (dec:12832)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
```

시드 1로 실행한 결과를 분석해보겠습니다:

1. VA 0: 0x01ae (10진수: 430) -> VALID: 0x00003230 (10진수: 12848)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

2. VA 1: 0x0109 (10진수: 265) -> VALID: 0x0000318b (10진수: 12683)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

3. VA 2: 0x020b (10진수: 523) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

4. VA 3: 0x019e (10진수: 414) -> VALID: 0x00003220 (10진수: 12832)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

5. VA 4: 0x0322 (10진수: 802) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

범위 내에 있는 주소의 경우 물리 주소로의 변환이 수행되었으며, 범위를 벗어난 주소의 경우 세그멘테이션 위반이 발생했습니다.

```
  ./relocation.py -s 2 -c
```

```
  Base-and-Bounds register information:

  Base   : 0x00003681 (decimal 13953)
  Limit  : 318

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> SEGMENTATION VIOLATION
  VA  1: 0x0109 (decimal:265) -> VALID: 0x00003712 (dec:14130)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x00003622 (dec:13858)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
```

시드 2로 실행한 결과를 분석해보겠습니다:

1. VA 0: 0x01ae (10진수: 430) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

2. VA 1: 0x0109 (10진수: 265) -> VALID: 0x00003712 (10진수: 14130)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

3. VA 2: 0x020b (10진수: 523) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

4. VA 3: 0x019e (10진수: 414) -> VALID: 0x00003622 (10진수: 13858)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

5. VA 4: 0x0322 (10진수: 802) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

범위 내에 있는 주소의 경우 물리 주소로의 변환이 수행되었으며, 범위를 벗어난 주소의 경우 세그멘테이션 위반이 발생했습니다.

```
  ./relocation.py -s 3 -c
```

```
Base-and-Bounds register information:

  Base   : 0x00003442 (decimal 13314)
  Limit  : 229

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x000035ec (dec:13804)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x000034ab (dec:13419)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x0000353c (dec:13660)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
```

시드 3로 실행한 결과를 분석해보겠습니다:

1. VA 0: 0x01ae (10진수: 430) -> VALID: 0x000035ec (10진수: 13804)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

2. VA 1: 0x0109 (10진수: 265) -> VALID: 0x000034ab (10진수: 13419)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

3. VA 2: 0x020b (10진수: 523) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

4. VA 3: 0x019e (10진수: 414) -> VALID: 0x0000353c (10진수: 13660)

- 주어진 가상 주소는 유효하며, 물리 주소로 변환되었습니다.

5. VA 4: 0x0322 (10진수: 802) -> SEGMENTATION VIOLATION

- 주어진 가상 주소는 세그멘테이션 위반이 발생했습니다. 이는 해당 주소가 허용되지 않는 메모리 영역에 속한다는 것을 나타냅니다.

  범위 내에 있는 주소의 경우 물리 주소로의 변환이 수행되었으며, 범위를 벗어난 주소의 경우 세그멘테이션 위반이 발생했습니다.

질문
2.1 다음 플래그로 실행하세요: `-s 0 -n 10`.

```
  ./relocation.py -s 0 -n 10 -c
```

```
 Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x00003230 (dec:12848)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x00003220 (dec:12832)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
  VA  5: 0x013a (decimal:314) -> VALID: 0x000031bc (dec:12732)
  VA  6: 0x02f8 (decimal:760) -> SEGMENTATION VIOLATION
  VA  7: 0x021e (decimal:542) -> SEGMENTATION VIOLATION
  VA  8: 0x01f7 (decimal:503) -> VALID: 0x00003279 (dec:12921)
  VA  9: 0x00d4 (decimal:212) -> VALID: 0x000030f6 (dec:12406)
```

2.2 생성된 모든 가상 주소가 범위 내에 있도록 하려면 `-l`(한계 레지스터)을 어떤 값으로 설정해야 할까요?

- 생성된 모든 가상 주소가 범위 내에 있도록 하려면 한계 레지스터(Limit register) 값을 주어진 가상 주소의 최대값으로 설정해야 합니다. 가상 주소의 범위는 0부터 472까지입니다. 따라서 한계 레지스터의 값은 472가 되어야 합니다.

질문
3.1 다음 플래그로 실행하세요: `-s 1 -n 10 -l 100`.

```
  ./relocation.py -s 1 -n 10 -l 100 -c
```

```
  Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 100

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> SEGMENTATION VIOLATION
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> SEGMENTATION VIOLATION
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
  VA  5: 0x013a (decimal:314) -> VALID: 0x000031bc (dec:12732)
  VA  6: 0x02f8 (decimal:760) -> SEGMENTATION VIOLATION
  VA  7: 0x021e (decimal:542) -> SEGMENTATION VIOLATION
  VA  8: 0x01f7 (decimal:503) -> SEGMENTATION VIOLATION
  VA  9: 0x00d4 (decimal:212) -> VALID: 0x000030f6 (dec:12406)
```

이 설정에서는 한계 레지스터의 값이 100으로 설정되어 있으므로, 가상 주소의 범위는 0부터 100까지입니다. 따라서 이 범위를 벗어나는 가상 주소에 대해서는 모두 세그멘테이션 위반이 발생합니다.

3.2 주소 공간이 여전히 물리 메모리에 전체적으로 맞도록 기준을 설정할 수 있는 최대값은 얼마인가요?

해설
물리 메모리에 전체적으로 맞도록 기준을 설정하려면, 한계 레지스터의 값이 물리 메모리의 크기와 일치해야 합니다.

한계 레지스터의 값은 472입니다. 물리 메모리의 크기를 472로 설정하면 주소 공간과 물리 메모리가 완전히 일치하게 됩니다. 따라서 이 값이 주소 공간을 물리 메모리에 전체적으로 맞도록 설정할 수 있는 최대값입니다.

질문 4 위와 동일한 문제 중 일부를 실행해 보세요. 단, 더 큰 주소 공간(-a)과 물리 메모리(-p)를 사용하세요.

```
  ./relocation.py -s 1 -n 10 -l 100 -a 1024 -p 1024 -c
```

```
  Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 100

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x00003230 (dec:12848)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> SEGMENTATION VIOLATION
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
  VA  5: 0x013a (decimal:314) -> VALID: 0x000031bc (dec:12732)
  VA  6: 0x02f8 (decimal:760) -> SEGMENTATION VIOLATION
  VA  7: 0x021e (decimal:542) -> SEGMENTATION VIOLATION
  VA  8: 0x01f7 (decimal:503) -> SEGMENTATION VIOLATION
  VA  9: 0x00d4 (decimal:212) -> VALID: 0x000030f6 (dec:12406)
```

주어진 결과를 분석하겠습니다.

이 설정에서는 주어진 한계 레지스터의 값이 100으로 설정되어 있습니다. 따라서 가상 주소의 범위는 0부터 100까지입니다.
이 범위를 벗어나는 가상 주소에 대해서는 모두 세그멘테이션 위반이 발생합니다. 유효한 주소의 경우에는 변환을 확인할 수 있습니다.
또한 주어진 옵션으로 주소 공간과 물리 메모리의 크기가 각각 1024로 설정되어 있습니다. 이는 주어진 문제의 더 큰 주소 공간과 물리 메모리를 반영합니다.
