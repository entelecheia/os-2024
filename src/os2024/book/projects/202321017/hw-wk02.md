# Week 2 Homework

## Q&A

1. 다음 플래그와 함께 process-run.py를 실행하세요: -l 5:100,5:100.
   CPU 활용률(예: CPU가 사용 중인 시간의 백분율)은 어떻게 되어야 할까요?
   A: 100%
   왜 그렇게 알 수 있나요?
   A: CPU 활용률은 CPU가 사용 중인 시간을 총 시간으로 나누어 백분율로 계산한다.
   주어진 결과에서 CPU가 사용 중인 시간은 10 단위이며, 총 시간도 10단위이므로 100%가 된다.
   -c와 -p 플래그를 사용해 여러분의 생각이 맞는지 확인해 보세요.

2. 이제 다음 플래그로 실행해 보세요: ./process-run.py -l 4:100,1:0.
   이 플래그는 4개의 명령어(모두 CPU 사용)를 가진 하나의 프로세스와 I/O를 실행하고 완료될 때까지 기다리는 하나의 프로세스를 지정합니다.
   두 프로세스를 모두 완료하는 데 얼마나 걸리나요?
   A: 11
   -c와 -p를 사용해 여러분의 생각이 맞는지 확인해 보세요.

3. 프로세스의 순서를 바꿔 보세요: -l 1:0,4:100.
   이제 어떻게 되나요?
   A: CPU와 I/O의 활용률이 변한다.
   순서를 바꾸는 것이 중요한가요?
   A: 스케줄링 알고리즘에 따라 중요하다.
   왜 그런가요? (항상 그렇듯이 -c와 -p를 사용해 여러분의 생각이 맞는지 확인해 보세요)
   A: 순서를 결정하는 것은 시스템의 성능과 효율성을 최적화하기 위해 필수적이기 때문이다.

4. 이제 다른 플래그들을 살펴보겠습니다.
   중요한 플래그 중 하나는 -S인데, 이는 프로세스가 I/O를 실행할 때 시스템이 어떻게 반응하는지 결정합니다.
   플래그를 SWITCH_ON_END로 설정하면 한 프로세스가 I/O를 수행하는 동안 시스템은 다른 프로세스로 전환하지 않고 해당 프로세스가 완전히 끝날 때까지 기다립니다.
   다음 두 프로세스를 실행할 때 어떤 일이 일어나나요(-l 1:0,4:100 -c -S SWITCH_ON_END), 하나는 I/O를 수행하고 다른 하나는 CPU 작업을 수행합니다?
   A: 두 번째 프로세스가 I/O를 100%의 확률로 수행하므로, 첫 번째 프로세스가 CPU 작업을 완료하기를 기다리는 동안 시스템은 두 번째 프로세스의 I/O 작업이 완료될 때까지 기다리게 된다.
   두 번째 프로세스의 I/O 작업이 완료된 후에야 첫 번째 프로세스가 CPU 작업을 계속할 수 있게 된다.

5. 이제 같은 프로세스를 실행하되, 한 프로세스가 I/O를 기다리는(WAITING) 동안 다른 프로세스로 전환되도록 switching 동작을 설정해 봅시다(-l 1:0,4:100 -c -S SWITCH_ON_IO).
   이제 어떤 일이 일어나나요? -c와 -p를 사용해 여러분의 생각이 맞는지 확인해 보세요.
   A: -S SWITCH_ON_IO 플래그가 사용되었으므로, 한 프로세스가 I/O를 기다리는 동안에도 다른 프로세스로 전환될 것이다.
   따라서 첫 번째 프로세스가 CPU를 사용하고 있는 동안 두 번째 프로세스가 I/O를 기다리는 동안 다른 프로세스로 전환될 것이다.

6. 또 다른 중요한 동작은 I/O가 완료될 때 수행할 작업입니다.
   -I IO_RUN_LATER를 사용하면 I/O가 완료될 때 그것을 실행한 프로세스가 반드시 즉시 실행되는 것은 아닙니다.
   오히려 그 시점에 실행 중이던 프로세스가 계속 실행됩니다.
   이러한 프로세스 조합을 실행하면 어떤 일이 일어나나요? (./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p)
   A:-I IO_RUN_LATER 플래그가 사용되었으므로, I/O가 완료될 때 해당 I/O를 실행한 프로세스가 즉시 실행되지 않고, 대신 실행 중이던 프로세스가 계속 실행된다.
   따라서 모든 I/O 작업이 완료될 때까지 해당 프로세스가 계속 실행된다.
   시스템 자원이 효율적으로 활용되고 있나요?
   A:여러 프로세스가 동시에 실행되고 있지만, -I IO_RUN_LATER 플래그로 인해 I/O 작업이 완료될 때 해당 프로세스가 즉시 실행되지 않는다.
   특히 CPU가 대기하고 있는 동안에도 실행 중인 프로세스가 계속해서 CPU를 사용하게 되므로, CPU의 유휴 시간이 발생할 수 있다.
   따라서 시스템 자원이 효율적으로 활용되고 있는 것으로 보이지 않는다.

7. 이제 같은 프로세스를 실행하되, -I IO_RUN_IMMEDIATE를 설정해 I/O를 실행한 프로세스를 즉시 실행하도록 해 봅시다.
   이 동작은 어떻게 다른가요?
   A: -I IO_RUN_IMMEDIATE 플래그가 설정되면 I/O 작업을 완료한 프로세스가 즉시 실행된다.
   이는 CPU 작업과 I/O 작업이 번갈아가며 실행되는 동안, I/O 작업을 완료한 프로세스가 대기하지 않고 즉시 CPU 작업을 다시 시작할 수 있음을 의미한다.
   이로 인해 시스템의 반응성이 향상되고, 자원이 더 효율적으로 활용된다.
   방금 I/O를 완료한 프로세스를 다시 실행하는 것이 왜 좋은 생각일 수 있을까요?
   A: 시스템의 반응성을 향상시키고 자원을 효율적으로 활용하기 때문이다.
   특히 I/O 작업이 CPU 작업보다 오래 걸리는 경우, CPU가 대기하는 동안에도 I/O 작업을 완료한 프로세스가 즉시 CPU 작업을 다시 시작하여 시스템의 처리량을 증가시킬 수 있다.
   또한, 프로세스를 바로 다시 실행함으로써 I/O 작업을 기다리는 시간을 최소화하여 전체 시스템의 처리 시간을 단축할 수 있다.
   따라서 I/O를 완료한 프로세스를 즉시 다시 실행하는 것은 시스템 성능을 향상시키는데 도움이 된다.

8. 이제 무작위로 생성된 프로세스로 실행해 봅시다: -s 1 -l 3:50,3:50 또는 -s 2 -l 3:50,3:50 또는 -s 3 -l 3:50,3:50.
   추적 결과가 어떻게 될지 예측해 보세요.
   A: CPU와 I/O를 번갈아가며 사용할 것이다.
   -I IO_RUN_IMMEDIATE 플래그와 -I IO_RUN_LATER 플래그를 사용할 때 어떤 일이 일어나나요?
   A: -I IO_RUN_IMMEDIATE 플래그를 사용할 때에는 CPU 작업 중에도 I/O작업이 요청되면 CPU가 중단되고 I/O 작업이 즉시 실행된다.
   -I IO_RUN-LATER 플래그를 사용할 때에는 CPU 작업이 완료된 후에 I/O작업이 실행된다.
   -S SWITCH_ON_IO와 -S SWITCH_ON_END를 사용할 때 어떤 일이 일어나나요?
   A: -S SWITCH_ON_IO 플래그 사용시 CPU와 I/O간의 전환이 I/O 작업이 발생할 때마다 발생한다.
   -S SWITCH_ON_END 플래그를 사용하면 CPU작업이 완료된 후에만 발생한다.
