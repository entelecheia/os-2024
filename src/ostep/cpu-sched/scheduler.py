#!/usr/bin/env python3

import argparse
import random
import sys


def random_seed(seed):
    random.seed(seed)


# 옵션 파서 설정
parser = argparse.ArgumentParser(description="작업 스케줄링 시뮬레이션 프로그램")
parser.add_argument("-s", "--seed", default=0, type=int, help="랜덤 시드")
parser.add_argument("-j", "--jobs", default=3, type=int, help="시스템의 작업 수")
parser.add_argument(
    "-l",
    "--jlist",
    default="",
    type=str,
    help="랜덤 작업 대신 쉼표로 구분된 실행 시간 목록 제공",
)
parser.add_argument("-m", "--maxlen", default=10, type=int, help="작업의 최대 길이")
parser.add_argument(
    "-p", "--policy", default="FIFO", type=str, help="스케줄 정책 사용: SJF, FIFO, RR"
)
parser.add_argument(
    "-q", "--quantum", default=1, type=int, help="RR 정책의 시간 조각 길이"
)
parser.add_argument("-c", action="store_true", default=False, help="답변 계산")

options = parser.parse_args()

random_seed(options.seed)

print("ARG 정책", options.policy)
if options.jlist == "":
    print("ARG 작업 수", options.jobs)
    print("ARG 최대 길이", options.maxlen)
    print("ARG 시드", options.seed)
else:
    print("ARG 작업 목록", options.jlist)
print("")

print("각 작업의 실행 시간을 포함한 작업 목록입니다: ")

joblist = []
if options.jlist == "":
    for jobnum in range(options.jobs):
        runtime = int(options.maxlen * random.random()) + 1
        joblist.append([jobnum, runtime])
        print("  작업", jobnum, "( 길이 = " + str(runtime) + " )")
else:
    jobnum = 0
    for runtime in options.jlist.split(","):
        joblist.append([jobnum, float(runtime)])
        jobnum += 1
    for job in joblist:
        print("  작업", job[0], "( 길이 = " + str(job[1]) + " )")
print("\n")

# 스케줄링 정책에 따른 시뮬레이션 결과 출력 부분입니다. `options.solve`가 참일 경우, 사용자가 지정한 스케줄링 정책에 따라 작업의 실행 순서와 각 작업에 대한 통계를 계산하고 출력합니다. 이 예에서는 FIFO(First In First Out), SJF(Shortest Job First), 그리고 RR(Round Robin) 세 가지 정책을 지원합니다.
#
# 스케줄링 정책 구현
#   FIFO(First In First Out): 작업이 도착한 순서대로 실행합니다.
#   SJF(Shortest Job First): 실행 시간이 가장 짧은 작업부터 먼저 실행합니다.
#   RR(Round Robin): 각 작업에 동일한 시간(quantum)만큼 실행 기회를 주고, 주어진 시간 동안 완료되지 않은 작업은 대기 목록의 끝으로 이동합니다.

if options.solve:
    print("** 해결책 **\n")
    if options.policy == "SJF":
        joblist = sorted(joblist, key=lambda job: job[1])  # 실행 시간 기준으로 정렬
        options.policy = "FIFO"  # SJF는 사실상 정렬된 FIFO입니다.

    if options.policy == "FIFO":
        thetime = 0
        print("실행 흔적:")
        waiting_times = []
        turnaround_times = []
        response_times = []
        for job in joblist:
            response_time = thetime  # FIFO에서는 작업이 대기하는 시간이 응답 시간과 같다
            print(
                "  [ 시간 %3d ] 작업 %d 실행 %.2f 초 ( 완료 시간 %.2f )"
                % (thetime, job[0], job[1], thetime + job[1])
            )
            waiting_time = thetime
            thetime += job[1]
            turnaround_time = thetime
            waiting_times.append(waiting_time)
            turnaround_times.append(turnaround_time)
            response_times.append(response_time)

        # 통계 출력
        print("\n통계:")
        total_jobs = len(joblist)
        average_waiting_time = sum(waiting_times) / total_jobs
        average_turnaround_time = sum(turnaround_times) / total_jobs
        average_response_time = sum(response_times) / total_jobs
        print(f"평균 대기 시간: {average_waiting_time:.2f} 초")
        print(f"평균 회전 시간: {average_turnaround_time:.2f} 초")
        print(f"평균 응답 시간: {average_response_time:.2f} 초")


    elif options.policy == "RR":
        print("실행 흔적:")
        thetime = 0
        queue = joblist.copy()  # 작업 목록을 대기열로 복사
        completions = []  # 완료된 작업 정보 저장

        while queue:
            job = queue.pop(0)  # 대기열에서 첫 번째 작업을 가져옴
            runtime = min(job[1], options.quantum)  # 실행 시간은 작업 시간과 quantum 중 작은 값
            print(f"  [ 시간 {thetime:3d} ] 작업 {job[0]} 실행 {runtime:.2f} 초 ( 남은 시간 {job[1]-runtime:.2f} )")
            thetime += runtime
            job[1] -= runtime
            if job[1] > 0:  # 작업이 완료되지 않았다면 대기열 끝으로
                queue.append(job)
            else:
                completions.append((job[0], thetime))  # 완료된 작업 저장

        # 통계 계산 및 출력
        print("\n통계:")
        turnaround_times = [0] * len(joblist)
        for job, complete_time in completions:
            turnaround_times[job] = complete_time  # 회전 시간 계산

        for i, turnaround in enumerate(turnaround_times):
            print(f"  작업 {i} 회전 시간: {turnaround} 초")
        print(f"평균 회전 시간: {sum(turnaround_times)/len(turnaround_times):.2f} 초")


    else:
        print("오류: 정책", options.policy, "는 사용할 수 없습니다.")
        sys.exit(0)
else:
    print("각 작업에 대한 회전 시간, 응답 시간, 대기 시간을 계산하세요.")
    print(
        "완료되면, 같은 인자를 사용하여 이 프로그램을 다시 실행하되, -c 옵션을 추가하십시오."
    )
    print(
        "이렇게 하면 답변을 제공할 것입니다. 다른 문제를 생성하기 위해 -s <somenumber> 또는"
    )
    print("자신만의 작업 목록(-l 10,15,20 등)을 사용할 수 있습니다.")
    print("")
