# Week 4 Homework


1. 3개의 작업과 난수 시드 1, 2, 3으로 시뮬레이션한 솔루션을 계산하세요.

- 이 시뮬레이션에서는 세 개의 작업이 시드 1에 따라 실행되며, 각 작업의 랜덤한 선정 과정과 완료 시간이 출력됩니다.


2. 이제 두 개의 특정 작업으로 실행해 보세요: 각각 길이는 10이지만 하나(작업 0)는 티켓이 1장이고 다른 하나(작업 1)는 100장입니다(예: -l 10:1,10:100). 티켓의 수가 이렇게 불균형할 때 어떤 일이 일어날까요? 작업 1이 완료되기 전에 작업 0이 실행될 수 있을까요? 얼마나 자주 그럴까요? 일반적으로 이러한 티켓 불균형이 복권 스케줄링의 동작에 어떤 영향을 미칠까요?

- 이 시뮬레이션에서는 작업 0은 티켓이 1장으로 훨씬 적지만, 때때로 랜덤 선택에 의해 실행됩니다. 하지만 티켓이 많은 작업 1이 자주 선정됩니다. 티켓 불균형은 스케줄링의 공정성에 영향을 미치며, 티켓이 적은 작업은 상대적으로 자주 선택되지 않을 수 있습니다.


3. 길이가 100이고 티켓 할당이 100으로 동일한 두 개의 작업(-l 100:100,100:100)으로 실행할 때 스케줄러는 얼마나 불공정할까요? 몇 가지 다른 난수 시드로 실행하여 (확률적) 답을 결정하세요; 한 작업이 다른 작업보다 얼마나 일찍 끝나는지에 따라 불공정성이 결정됩니다.

- 길이가 100이고 티켓 할당이 100으로 동일한 두 개의 작업을 실행할 때, 스케줄러는 난수 시드에 따라 작업 완료 순서가 달라지며, 더 짧은 작업이 일반적으로 먼저 완료됩니다. 이는 복권 스케줄링의 불공정성을 나타내며, 다양한 시드로 시뮬레이션하면 이 차이가 확실히 드러납니다.