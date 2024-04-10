"""
이 모듈은 가상 주소와 물리 메모리 관리를 시뮬레이션합니다. 사용자는 주소 공간 크기, 물리 메모리 크기,
베이스 및 리밋 레지스터 값 등을 설정할 수 있으며, 모듈은 이러한 입력에 기반하여 가상 주소를 물리 주소로 변환하는
과정을 시뮬레이션하고 세그먼테이션 위반을 확인합니다. 사용자는 명령줄 인터페이스를 통해 다양한 옵션을 설정할 수 있습니다.

사용 방법:
    스크립트는 명령줄 옵션을 통해 여러 매개변수를 입력받아 처리합니다. 예를 들어, 주소 공간 크기(asize),
    물리 메모리 크기(psize), 베이스 및 리밋 레지스터 값(base, limit), 생성할 가상 주소의 수(num) 등을
    사용자가 직접 지정할 수 있습니다. 사용자는 `-c` 옵션을 사용하여 계산 과정을 직접 실행시킬 수 있습니다.

예제 사용법:
    python3 relocation.py -a 32k -p 64k -n 10 -b 1000 -l 1000 -c

옵션:
    -s, --seed       : 랜덤 시드 값 (기본값: 0)
    -a, --asize      : 주소 공간 크기 (예: 16, 64k, 32m, 1g, 기본값: 1k)
    -p, --physmem    : 물리 메모리 크기 (예: 16, 64k, 32m, 1g, 기본값: 16k)
    -n, --addresses  : 생성할 가상 주소의 수 (기본값: 5)
    -b               : 베이스 레지스터 값 (기본값: -1, 자동 계산)
    -l               : 리밋 레지스터 값 (기본값: -1, 자동 계산)
    -c, --compute    : 계산 실행 여부 (기본값: False, 계산 실행)
"""

import random
from optparse import OptionParser


def random_seed(seed):
    """랜덤 시드를 설정하는 함수"""
    random.seed(seed)


def convert(size):
    """크기 단위를 문자열에서 바이트 단위의 정수로 변환하는 함수"""
    length = len(size)
    lastchar = size[length - 1]
    if lastchar in ["k", "K"]:
        m = 1024
        return int(size[: length - 1]) * m
    elif lastchar in ["m", "M"]:
        m = 1024 * 1024
        return int(size[: length - 1]) * m
    elif lastchar in ["g", "G"]:
        m = 1024 * 1024 * 1024
        return int(size[: length - 1]) * m
    else:
        return int(size)


# 옵션 파서 설정 및 옵션 파싱 과정
parser = OptionParser()
parser.add_option("-s", "--seed", default=0, type=int, help="랜덤 시드 값", dest="seed")
parser.add_option(
    "-a",
    "--asize",
    default="1k",
    type="string",
    help="주소 공간 크기 (예: 16, 64k, 32m, 1g)",
    dest="asize",
)
parser.add_option(
    "-p",
    "--physmem",
    default="16k",
    type="string",
    help="물리 메모리 크기 (예: 16, 64k, 32m, 1g)",
    dest="psize",
)
parser.add_option(
    "-n", "--addresses", default=5, type=int, help="생성할 가상 주소의 수", dest="num"
)
parser.add_option(
    "-b", default="-1", type="string", help="베이스 레지스터 값", dest="base"
)
parser.add_option(
    "-l", default="-1", type="string", help="리밋 레지스터 값", dest="limit"
)
parser.add_option(
    "-c",
    "--compute",
    action="store_true",
    default=False,
    help="계산 실행 여부",
    dest="solve",
)
options, args = parser.parse_args()

# 이하 로직은 위에서 설명한대로 동일하게 작동합니다.
# 코드 실행 및 결과 출력 등의 로직은 변경 없이 기존의 방식을 따릅니다.

# 옵션 파싱
(options, args) = parser.parse_args()

print("ARG 시드", options.seed)
print("ARG 주소 공간 크기", options.asize)
print("ARG 물리 메모리 크기", options.psize)
print("")

# 랜덤 시드 설정
random_seed(options.seed)

# 주소 공간과 물리 메모리 크기를 바이트 단위로 변환
asize = convert(options.asize)
psize = convert(options.psize)

# 물리 메모리 크기가 0 이하인 경우 오류 처리
if psize <= 1:
    print("오류: 물리 메모리 크기는 0보다 커야 합니다.")
    exit(1)

# 주소 공간 크기가 0인 경우 오류 처리
if asize == 0:
    print("오류: 주소 공간 크기는 0보다 커야 합니다.")
    exit(1)

# 물리 메모리 크기가 주소 공간 크기보다 작은 경우 오류 처리
if psize <= asize:
    print("오류: 물리 메모리 크기는 주소 공간 크기보다 커야 합니다.")
    exit(1)

# 세그먼트 레지스터의 base와 limit 값 설정
limit = convert(options.limit)
base = convert(options.base)

# limit 값이 -1인 경우 주소 공간 크기의 1/4에서 1/2 사이의 랜덤값으로 설정
if limit == -1:
    limit = int(asize / 4.0 + (asize / 4.0 * random.random()))

# base 값이 -1인 경우 물리 메모리 내에서 적절한 위치를 찾아 설정
if base == -1:
    done = 0
    while not done:
        base = int(psize * random.random())
        if (base + limit) < psize:
            done = 1

print("Base-and-Bounds 레지스터 정보:")
print("  Base   : 0x%08x (decimal %d)" % (base, base))
print("  Limit  : %d" % (limit))
print("")

# base와 limit 합이 물리 메모리 크기를 초과하는 경우 오류 처리
if base + limit > psize:
    print("오류: 주어진 base/bounds 값으로 물리 메모리에 주소 공간이 맞지 않습니다.")
    print("Base + Limit:", base + limit, "  Psize:", psize)
    exit(1)

# 가상 주소 트레이스 생성
print("가상 주소 트레이스")
for i in range(options.num):
    vaddr = int(asize * random.random())
    if not options.solve:
        print(
            "  VA %2d: 0x%08x (decimal: %d) --> PA 또는 세그먼테이션 위반?"
            % (i, vaddr, vaddr)
        )
    else:
        paddr = 0
        if vaddr >= limit:
            print(
                "  VA %2d: 0x%08x (decimal: %d) --> 세그먼테이션 위반"
                % (i, vaddr, vaddr)
            )
        else:
            paddr = vaddr + base
            print(
                "  VA %2d: 0x%08x (decimal: %d) --> 유효: 0x%08x (decimal: %d)"
                % (i, vaddr, vaddr, paddr, paddr)
            )

if not options.solve:
    print("각 가상 주소에 대해 해당하는 물리 주소를 작성하거나")
    print(
        "세그먼테이션 위반을 기록하세요. 이 시뮬레이션에서는 주어진 크기의 간단한 가상 주소 공간을 가정합니다."
    )
    print("")
