# Homework: 빈 공간 관리

| [빈 공간 관리](http://www.cs.wisc.edu/~remzi/OSTEP/vm-freespace.pdf) | [malloc 실행](https://github.com/entelecheia/os-2024/tree/main/src/ostep-homework/vm-freespace) |

```{note}
본 과제는 프로젝트를 위해 생성한 본인의 개별 폴더에 제출하기 바랍니다. 제출기한은 5/15(수)입니다.
```

## 개요

이 프로그램인 malloc.py는 간단한 메모리 할당자가 어떻게 작동하는지 보여줍니다. 사용할 수 있는 옵션은 다음과 같습니다:

```sh
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -S HEAPSIZE, --size=HEAPSIZE
                        size of the heap
  -b BASEADDR, --baseAddr=BASEADDR
                        base address of heap
  -H HEADERSIZE, --headerSize=HEADERSIZE
                        size of the header
  -a ALIGNMENT, --alignment=ALIGNMENT
                        align allocated units to size; -1->no align
  -p POLICY, --policy=POLICY
                        list search (BEST, WORST, FIRST)
  -l ORDER, --listOrder=ORDER
                        list order (ADDRSORT, SIZESORT+, SIZESORT-, INSERT-FRONT, INSERT-BACK)
  -C, --coalesce        coalesce the free list?
  -n OPSNUM, --numOps=OPSNUM
                        number of random ops to generate
  -r OPSRANGE, --range=OPSRANGE
                        max alloc size
  -P OPSPALLOC, --percentAlloc=OPSPALLOC
                        percent of ops that are allocs
  -A OPSLIST, --allocList=OPSLIST
                        instead of random, list of ops (+10,-0,etc)
  -c, --compute         compute answers for me
```

이 프로그램을 사용하는 한 가지 방법은 프로그램이 무작위로 할당/해제 연산을 생성하도록 하고, 여러분이 각 연산의 성공 또는 실패뿐만 아니라 프리 리스트가 어떻게 보일지 알아내는 것입니다.

다음은 간단한 예시입니다:

```sh
./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5
```

```sh
ptr[0] = Alloc(3)  returned ?
List?

Free(ptr[0]) returned ?
List?

ptr[1] = Alloc(5)  returned ?
List?

Free(ptr[1]) returned ?
List?

ptr[2] = Alloc(8)  returned ?
List?
```

이 예시에서는 100 바이트 크기의 힙(-S 100)을 지정하고, 시작 주소는 1000(-b 1000)입니다. 할당된 블록당 4 바이트의 추가 헤더(-H 4)를 지정하고, 각 할당된 공간이 가장 가까운 4 바이트 자유 청크 크기로 반올림되도록 합니다(-a 4). 프리 리스트를 주소별로 정렬(오름차순)하도록 지정합니다. 마지막으로 "최적 적합" 프리 리스트 검색 정책(-p BEST)을 지정하고, 5개의 무작위 연산을 생성하도록 요청합니다(-n 5). 이를 실행한 결과는 위와 같습니다. 여러분의 임무는 각 할당/해제 연산이 무엇을 반환하는지, 그리고 각 연산 후에 프리 리스트의 상태가 어떤지 알아내는 것입니다.

-c 옵션을 사용하여 결과를 살펴보겠습니다.

```sh
./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5 -c
```

```sh
ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]

ptr[1] = Alloc(5)  returned 1012 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:12 ] [ addr:1020 sz:80 ]

ptr[2] = Alloc(8)  returned 1012 (searched 3 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]
```

보시다시피, 첫 번째 할당 연산은 다음 정보를 반환합니다:

```sh
ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]
```

프리 리스트의 초기 상태가 하나의 큰 요소일 뿐이므로 Alloc(3) 요청이 성공할 것이라는 것을 쉽게 추측할 수 있습니다. 더욱이, 그것은 단순히 첫 번째 메모리 청크를 반환하고 나머지를 프리 리스트로 만들 것입니다. 반환된 포인터는 헤더(주소:1004) 바로 다음에 있을 것이며, 할당된 공간은 4 바이트로 반올림되어 1008부터 시작하는 92 바이트의 프리 리스트가 남습니다.

다음 연산은 이전 할당 요청의 결과를 저장하는 "ptr[0]"에 대한 Free입니다. 예상할 수 있듯이 이 해제는 성공할 것이고(따라서 "0"을 반환), 프리 리스트는 이제 조금 더 복잡해 보입니다:

```sh
Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]
```

실제로 우리는 프리 리스트를 병합하지 않기 때문에, 이제 프리 리스트에 두 개의 요소가 있습니다. 첫 번째는 8 바이트 크기이고 방금 반환된 공간을 보유하고 있으며, 두 번째는 92 바이트 청크입니다.

-C 플래그를 통해 병합을 실제로 켤 수 있으며, 그 결과는 다음과 같습니다:

```sh
./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5 -c -C
```

```sh
ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[1] = Alloc(5)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]

Free(ptr[1]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[2] = Alloc(8)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]
```

Free 연산이 이루어질 때 예상대로 프리 리스트가 병합되는 것을 볼 수 있습니다.

살펴볼 만한 다른 흥미로운 옵션들도 있습니다:

- `-p BEST` 또는 `-p WORST` 또는 `-p FIRST`: 이 옵션을 사용하면 할당 요청 중에 사용할 메모리 청크를 찾기 위해 이 세 가지 다른 전략 중 하나를 사용할 수 있습니다.
- `-l ADDRSORT` 또는 `-l SIZESORT+` 또는 `-l SIZESORT-` 또는 `-l INSERT-FRONT` 또는 `-l INSERT-BACK`: 이 옵션을 사용하면 프리 리스트를 특정 순서로 유지할 수 있습니다. 예를 들어 자유 청크의 주소별로 정렬하거나, 자유 청크의 크기별로 정렬(+는 오름차순, -는 내림차순)하거나, 단순히 해제된 청크를 프리 리스트의 앞(INSERT-FRONT) 또는 뒤(INSERT-BACK)로 반환할 수 있습니다.
- `-A list_of_ops`: 이 옵션을 사용하면 무작위로 생성된 요청 대신 정확한 요청 시퀀스를 지정할 수 있습니다. 예를 들어 "-A +10,+10,+10,-0,-2" 플래그로 실행하면 10 바이트 크기의 청크 3개(헤더 제외)를 할당한 다음, 첫 번째 청크("-0")를 해제하고 세 번째 청크("-2")를 해제합니다. 그러면 프리 리스트는 어떻게 보일까요?

이것이 기본입니다. 책 챕터의 질문을 사용하여 더 많은 것을 탐색하거나, 할당자가 어떻게 작동하는지 더 잘 이해하기 위해 스스로 새롭고 흥미로운 질문을 만들어 보세요.

## 과제 (시뮬레이션)

### 질문

1. 먼저 `-n 10 -H 0 -p BEST -s 0` 플래그로 실행하여 몇 개의 무작위 할당과 해제를 생성하세요. alloc()/free()가 무엇을 반환할지 예측할 수 있나요? 각 요청 후에 프리 리스트의 상태를 추측할 수 있나요? 시간이 지남에 따라 프리 리스트에서 무엇을 알 수 있나요?

2. 프리 리스트를 검색할 때 최악 적합(Worst-fit) 정책(`-p WORST`)을 사용하면 결과가 어떻게 달라지나요? 무엇이 바뀌나요?

3. 최선 적합(First-fit) 정책(`-p FIRST`)은 어떤가요? 최선 적합을 사용하면 무엇이 빨라지나요?

4. 위의 질문들에서, 리스트를 어떤 순서로 유지하느냐에 따라 일부 정책의 자유 블록 탐색 시간이 달라질 수 있습니다. 다양한 프리 리스트 순서(`-l ADDRSORT,-l SIZESORT+,-l SIZESORT-`)를 사용하여 정책과 리스트 순서가 어떻게 상호작용하는지 확인하세요.

5. 프리 리스트의 병합(Coalescing)은 매우 중요할 수 있습니다. 무작위 할당 수를 늘려보세요(예: -n 1000). 시간이 지남에 따라 큰 할당 요청에는 어떤 일이 일어나나요? 병합 없이(즉, -C 플래그 없이) 그리고 병합과 함께 실행해 보세요. 결과에 어떤 차이가 있나요? 각 경우에 시간에 따른 프리 리스트의 크기는 어떻게 되나요? 이 경우 리스트의 순서가 중요한가요?

6. 할당된 블록의 비율을 나타내는 `-P`를 50 이상으로 변경하면 어떻게 되나요? 이 비율이 100에 가까워지면 할당은 어떻게 되나요? 0에 가까워지면요?

7. 고도로 단편화된 프리 공간을 생성하려면 어떤 종류의 특정 요청을 만들 수 있나요? -A 플래그를 사용하여 단편화된 프리 리스트를 생성하고, 다양한 정책과 옵션이 프리 리스트의 구성을 어떻게 변경하는지 확인하세요.
