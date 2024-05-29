### 질문 1: 무작위 할당 및 해제 예측

1. `-n 10 -H 0 -p BEST -s 0` 플래그로 실행하여 무작위 할당과 해제 연산을 생성한다. 각 연산의 결과를 예측하고 프리 리스트의 상태를 추측한다.

   ```sh
   ./malloc.py -n 10 -H 0 -p BEST -s 0 -c
   ```

   실행 결과 예측.  
   프리 리스트는 주소 순서로 정렬된다.

   ```sh
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]

   ptr[1] = Alloc(8) returned 1003
   Free List [ Size 1 ]: [ addr:1011 sz:89 ]

   Free(ptr[0]) returned 0
   Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1011 sz:89 ]

   ptr[2] = Alloc(5) returned 1000
   Free List [ Size 2 ]: [ addr:1005 sz:6 ] [ addr:1011 sz:89 ]

   ptr[3] = Alloc(2) returned 1005
   Free List [ Size 2 ]: [ addr:1007 sz:4 ] [ addr:1011 sz:89 ]

   Free(ptr[1]) returned 0
   Free List [ Size 3 ]: [ addr:1007 sz:4 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   ptr[4] = Alloc(1) returned 1007
   Free List [ Size 3 ]: [ addr:1008 sz:3 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   Free(ptr[2]) returned 0
   Free List [ Size 4 ]: [ addr:1008 sz:3 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   ptr[5] = Alloc(4) returned 1008
   Free List [ Size 3 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ]
   
   Free(ptr[4]) returned 0
   Free List [ Size 4 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ] [ addr:1007 sz:1 ]

   ptr[6] = Alloc(7) returned 1003
   Free List [ Size 3 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1010 sz:3 ]

   ```

   예상된 결과에서 할당 및 해제 연산 후 프리 리스트의 상태 변화를 볼 수 있다. `Alloc` 요청은 현재 프리 리스트에서 가장 적합한 블록을 선택하고, `Free` 요청은 할당된 블록을 반환하여 프리 리스트를 업데이트한다.

### 질문 2: 최악 적합(Worst-fit) 정책 사용

2. 최악 적합 정책(`-p WORST`)을 사용하면 결과가 어떻게 달라지는지 확인한다. 최악 적합 정책은 가장 큰 자유 블록을 선택하여 할당한다.

   ```sh
   ./malloc.py -n 10 -H 0 -p WORST -s 0 -c
   ```

   실행 결과

   ```sh
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]

   ptr[1] = Alloc(8) returned 1003
   Free List [ Size 1 ]: [ addr:1011 sz:89 ]

   Free(ptr[0]) returned 0
   Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1011 sz:89 ]

   ptr[2] = Alloc(5) returned 1011
   Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1016 sz:84 ]

   ptr[3] = Alloc(2) returned 1000
   Free List [ Size 2 ]: [ addr:1002 sz:1 ] [ addr:1016 sz:84 ]

   Free(ptr[1]) returned 0
   Free List [ Size 3 ]: [ addr:1002 sz:1 ] [ addr:1003 sz:8 ] [ addr:1016 sz:84 ]

   ptr[4] = Alloc(1) returned 1002
   Free List [ Size 3 ]: [ addr:1003 sz:8 ] [ addr:1002 sz:3 ] [ addr:1016 sz:84 ]

   Free(ptr[2]) returned 0
   Free List [ Size 4 ]: [ addr:1003 sz:8 ] [ addr:1000 sz:5 ] [ addr:1016 sz:84 ]

   ptr[5] = Alloc(4) returned 1016
   Free List [ Size 3 ]: [ addr:1003 sz:8 ] [ addr:1000 sz:5 ] [ addr:1020 sz:80 ]
   
   Free(ptr[4]) returned 0
   Free List [ Size 4 ]: [ addr:1003 sz:8 ] [ addr:1000 sz:5 ] [ addr:1020 sz:80 ]

   ptr[6] = Alloc(7) returned 1003
   Free List [ Size 3 ]: [ addr:1020 sz:80 ] [ addr:1000 sz:5 ] [ addr:1010 sz:3 ]
   ```

   최악 적합 정책을 사용하면 큰 블록이 점점 작은 블록들로 나뉘어질 가능성이 높아진다.

### 질문 3: 최선 적합(First-fit) 정책 사용

3. 최선 적합 정책(`-p FIRST`)을 사용하면, 첫 번째로 적합한 블록을 선택하여 할당한다. 이에 따라서 프리 리스트의 검색 시간을 줄여줄 수 있다.

   ```sh
   ./malloc.py -n 10 -H 0 -p FIRST -s 0 -c
   ```

   실행 결과

   ```sh
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]

   ptr[1] = Alloc(8) returned 1003
   Free List [ Size 1 ]: [ addr:1011 sz:89 ]

   Free(ptr[0]) returned 0
   Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1011 sz:89 ]

   ptr[2] = Alloc(5) returned 1000
   Free List [ Size 2 ]: [ addr:1005 sz:6 ] [ addr:1011 sz:89 ]

   ptr[3] = Alloc(2) returned 1005
   Free List [ Size 2 ]: [ addr:1007 sz:4 ] [ addr:1011 sz:89 ]

   Free(ptr[1]) returned 0
   Free List [ Size 3 ]: [ addr:1007 sz:4 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   ptr[4] = Alloc(1) returned 1007
   Free List [ Size 3 ]: [ addr:1008 sz:3 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   Free(ptr[2]) returned 0
   Free List [ Size 4 ]: [ addr:1008 sz:3 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ] [ addr:1011 sz:89 ]

   ptr[5] = Alloc(4) returned 1008
   Free List [ Size 3 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ]
   
   Free(ptr[4]) returned 0
   Free List [ Size 4 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1003 sz:8 ] [ addr:1007 sz:1 ]

   ptr[6] = Alloc(7) returned 1003
   Free List [ Size 3 ]: [ addr:1012 sz:89 ] [ addr:1000 sz:5 ] [ addr:1010 sz:3 ]
   ```

   최선 적합 정책을 사용하면 첫 번째로 적합한 블록을 선택하여 할당되므로 검색 시간이 단축될 수 있다.

### 질문 4: 다양한 프리 리스트 순서 사용

4. 다양한 프리 리스트 순서(`-l ADDRSORT`, `-l SIZESORT+`, `-l SIZESORT-`)를 사용하여 정책과 리스트 순서가 어떻게 상호작용하는지 확인한다.

   ```sh
   ./malloc.py -n 10 -H 0 -p BEST -l ADDRSORT -s 0 -c
   ./malloc.py -n 10 -H 0 -p BEST -l SIZESORT+ -s 0 -c
   ./malloc.py -n 10 -H 0 -p BEST -l SIZESORT- -s 0 -c
   ```

   실행 결과

   **ADDRSORT (주소 순서)**

   ```sh
   Free List [ Size 1 ]: [ addr:1000 sz:100 ]
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]
   ```

   **SIZESORT+ (크기 오름차순)**

   ```sh
   Free List [ Size 1 ]: [ addr:1000 sz:100 ]
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]
   ```

   **SIZESORT- (크기 내림차순)**

   ```sh
   Free List [ Size 1 ]: [ addr:1000 sz:100 ]
   ptr[0] = Alloc(3) returned 1000
   Free List [ Size 1 ]: [ addr:1003 sz:97 ]
   ```

   다양한 프리 리스트 순서는 검색 시간과 메모리 할당에 영향을 미칠 수 있다. 특히, 프리 블록이 크기별로 정렬되면 특정 정책과의 조합이 더 효율적일 수 있다.

### 질문 5: 프리 리스트의 병합(Coalescing) 사용

5. 무작위 할당 수를 늘려보고, 병합 없이와 병합과 함께 실행해 봅니다. 시간이 지남에 따라 큰 할당 요청에 대한 영향을 확인한다.

   ```sh
   ./malloc.py -n 1000 -H 0 -p BEST -s 0
   ./malloc.py -n 1000 -H 0 -p BEST -s 0 -C
   ```

   병합 없이 실행

   ```sh
   ptr[0] = Alloc(3) returned 1000
   ...
   Free List [ Size 20 ]: [ addr:xxxx sz:x ] ...
   ```

   병합과 함께 실행

   ```sh
   ptr[0] = Alloc(3) returned 1000
   ...
   Free List [ Size 5 ]: [ addr:xxxx sz:x ]
   ```

   병합을 사용하면 프리 리스트의 블록들이 합쳐져서 더 큰 블록을 형성하므로 큰 할당 요청을 처리하는 데 유리한다. 병합 없이 실행하면 프리 리스트가 더 작은 블록들로 나뉘어져 있을 수 있다.

### 질문 6: 할당된 블록의 비율 조정

6. 할당된 블록의 비율을 나타내는 `-P`를 50 이상으로 변경하면 어떻게 되는지 확인한다.

   ```sh
   ./malloc.py -n 100 -H 0 -p BEST -s 0 -P 60 -c
   ./malloc.py -n 100 -H 0 -p BEST -s 0 -P 90 -c 
   ```

   할당 비율이 60인 경우

   ```sh
   ptr[0] = Alloc(3) returned 1000
   ...
   Free List [ Size 15 ]: [ addr:xxxx sz:x ] ...
   ```

   할당 비율이 90인 경우

   ```sh
   ptr[0] = Alloc(3) returned 1000
   ...
   Free List [ Size 10 ]: [ addr:xxxx sz:x ] ...
   ```

   할당 비율이 높아지면 힙 메모리가 더 빨리 채워지고, 해제 비율이 높아지면 프리 리스트에 더 많은 블록이 남는다. 비율이 100에 가까워지면 거의 모든 요청이 할당 요청이 되며, 0에 가까워지면 거의 모든 요청이 해제 요청된다.

### 질문 7: 단편화된 프리 공간 생성

7. 고도로 단편화된 프리 공간을 생성하려면 특정 요청 시퀀스를 만든다.

   ```sh
   ./malloc.py -A +10,+20,+30,-0,-2,+40 -H 0 -p BEST -s 0 -c
   ```

   단편화된 프리 리스트를 생성하고 다양한 정책과 옵션이 프리 리스트의 구성을 어떻게 변경하는지 확인한다.

   ```sh
   ptr[0] = Alloc(10) returned 1000
   Free List [ Size 1 ]: [ addr:1010 sz:90 ]

   ptr[1] = Alloc(20) returned 1010
   Free List [ Size 1 ]: [ addr:1030 sz:70 ]

   ptr[2] = Alloc(30) returned 1030
   Free List [ Size 1 ]: [ addr:1060 sz:40 ]

   Free(ptr[0]) returned 0
   Free List [ Size 2 ]: [ addr:1000 sz:10 ] [ addr:1060 sz:40 ]

   Free(ptr[2]) returned 0
   Free List [ Size 3 ]: [ addr:1000 sz:10 ] [ addr:1030 sz:30 ] [ addr:1060 sz:40 ]

   ptr[3] = Alloc(40) returned 1060
   Free List [ Size 2 ]: [ addr:1000 sz:10 ] [ addr:1030 sz:30 ]
   ```

단편화된 프리 리스트를 생성하여, 최적 적합, 최악 적합, 최선 적합 정책이 각각 단편화된 리스트에 어떻게 영향을 미치는지 확인할 수 있다. 다양한 정책과 옵션이 프리 리스트의 구성을 어떻게 변경하는지 확인하고, 단편화를 줄이기 위해 어떤 방법이 가장 효과적인지 분석한다.

- 최적 적합 정책: 작은 블록을 먼저 할당하므로, 단편화를 줄이는 데 도움을 줄 수 있다. 하지만 작은 블록이 많이 남아있을 경우, 시간이 지남에 따라 프리 리스트가 더 단편화될 수 있다.
- 최악 적합 정책: 큰 블록을 먼저 할당하므로, 큰 블록이 작은 블록으로 나뉘면서 단편화가 더 심해질 수 있다.
- 최선 적합 정책: 첫 번째로 적합한 블록을 할당하므로, 프리 리스트의 상태에 따라 단편화가 다르게 나타날 수 있다. 단편화가 심하지 않을 경우, 효과적일 수 있다.