# Tip: Linux 기본 명령어

- 명령어(Command): 하나의 작업을 수행하는 명령어 또는 프로그램
- 옵션(Options): 명령어가 작업을 수행하는 방식을 변경
  - 단일 문자 옵션: 하이픈 하나와 한 글자 (예: `ls -a`)
  - 단어 옵션: 하이픈 두 개와 단어 (예: `ls --all`)
- 인자(Argument): 명령어가 상호작용할 입력 또는 출력을 제공

명령어에 대한 더 자세한 정보는 `man` 또는 `info` 명령어를 사용 (예: `man ls`)

```
[사용자명@scc1 ~]$ 명령어 --옵션 인자
```

## 명령어 실습

- 접속 후 다음 명령어들을 입력해 보세요.
  - `whoami` # 현재 로그인 사용자 이름 출력
  - `hostname` # 이 컴퓨터의 이름 출력
  - `echo "Hello, world"` # 화면에 문자 출력
  - `echo $HOME` # 환경 변수 출력
  - `echo 내 로그인은 $(whoami)` # `$(xx)`를 명령어 출력으로 대체
  - `date` # 현재 날짜/시간 출력
  - `cal` # 이번 달 달력 출력
  - `shazam` # 잘못된 명령어

## 명령어 옵션 실습

- 명령어는 세 부분으로 구성됩니다: 명령어, 옵션, 인자/매개변수.
  예: `cal -j 3 1999`. `cal`은 명령어, `-j`는 옵션(또는 스위치), `3`과 `1999`는 인자/매개변수입니다.
- 프롬프트의 역할은 무엇일까요?
- 명령어에 대한 시스템의 응답은 어떤가요?

```
[사용자명@scc1 ~]$ cal -j 3 1999
```

## 주요 텍스트 처리 유틸리티

- `awk` 패턴 검색 및 처리 언어
- `cat` 파일 내용 화면에 출력
- `cut` 파일의 각 줄에서 선택한 필드 추출
- `diff` 두 파일 비교
- `grep` 텍스트에서 패턴 검색
- `head` 파일의 앞부분 출력
- `less` 페이지 단위로 파일 내용 출력
- `sed` 스트림 편집기 (특히 검색 및 대체)
- `sort` 텍스트 파일 정렬
- `split` 파일 분할
- `tail` 파일의 끝부분 출력
- `tr` 문자 변환/삭제
- `uniq` 파일에서 중복 행 제거
- `wc` 행, 단어, 문자 수 계산

## 변수와 환경 변수

- 변수는 이름이 붙은 저장 공간입니다.
  - `USER=augustin`
  - `foo="this is foo's value"`
- "환경 변수"는 셸에서 사용하고 공유하는 변수입니다.
  - 예를 들어, `$PATH`는 시스템에서 명령어를 찾을 위치를 알려줍니다.
- 환경 변수는 셸이 실행하는 프로그램과 공유됩니다.

## Bash 변수

- 새 변수를 만들려면 할당 연산자 `=`를 사용
- `foo` 변수는 `echo`로 출력 가능
- `$foo`를 셸에서 실행되는 프로그램에서도 볼 수 있게 하려면 (즉, "환경 변수"로 만들려면) `export`를 사용:

```
[사용자명@scc1 ~]$ foo="this is foo's value"
[사용자명@scc1 ~]$ echo $foo
this is foo's value
[사용자명@scc1 ~]$ export foo
```

## 환경 변수

- 현재 정의된 모든 환경 변수를 보려면 `printenv`를 사용:

```
[사용자명@scc1 ~]$ printenv
```

```
MallocNanoZone=0
USER=yj.lee
SECURITYSESSIONID=186a2
COMMAND_MODE=unix2003
__CFBundleIdentifier=com.microsoft.VSCode
PATH=/Users/yj.lee/.venvs/os2024/bin:/Users/yj.lee/.gem/bin:/Users/yj.lee/.rbenv/shims:/Users/yj.lee/.sdkman/candidates/java/current/bin:/Users/yj.lee/.nvm/versions/node/v20.11.1/bin:/Users/yj.lee/go/bin:/Users/yj.lee/.local/bin:/usr/local/bin:/System/Cryptexes/App/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/local/bin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/bin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/appleinternal/bin:/opt/X11/bin:/usr/local/MacGPG2/bin:/Library/TeX/texbin:/Users/yj.lee/.cargo/bin
HOME=/Users/yj.lee
SHELL=/usr/local/bin/zsh
LaunchInstanceID=3D178438-A306-4DE4-8678-109656B36948
__CF_USER_TEXT_ENCODING=0x1F5:0x0:0x0
XPC_SERVICE_NAME=0
DISPLAY=/private/tmp/com.apple.launchd.Kk6SLkPxBE/org.xquartz:0
SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.t56iutli5W/Listeners
XPC_FLAGS=0x0
LOGNAME=yj.lee
```

## 명령어 기록 및 명령줄 편집

- `history` 명령어를 실행해 보세요.
- 위쪽 `↑` 및 아래쪽 `↓` 화살표로 명령어 기록에서 선택
- 마지막 명령어를 다시 실행하려면 `!!`를 입력
- 명령어 기록에서 더 이전으로 가려면 `!`와 `history`에 표시된 번호 (예: `!132`)를 입력하세요. 또는 예를 들어 가장 최근의 `ls` 명령어와 일치시키려면 `!ls`를 입력하세요.
- 명령줄에서 왼쪽 `←` 및 오른쪽 `→` 화살표는 어떤 역할을 할까요?
- `<Del>` 및 `<Backspace>` 키를 눌러보세요.

## 명령어 도움말

- 다음을 입력해 보세요.
  - `date --help`
  - `man date`
  - `info date`
- BASH 내장 명령어
  - 다른 명령어와 조금 다릅니다.
  - `help` 명령어만 입력하세요.
  - 또는 `man bash`

## `less`와 함께 `man` 사용하기

- `man` 명령어는 `less`라는 페이저로 출력하며, 이는 텍스트 스크롤을 다양한 방식으로 지원합니다:
  - `Space`, `f` # 앞으로 페이지 이동
  - `b` # 뒤로 페이지 이동
  - `<` # 파일의 첫 줄로 이동
  - `>` # 파일의 마지막 줄로 이동
  - `/` # 앞으로 검색 (반복은 `n`)
  - `?` # 뒤로 검색 (반복은 `N`)
  - `h` # 도움말 표시
  - `q` # 도움말 종료
