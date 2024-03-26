# WSL에서 Ubuntu 환경구축 및 GitHub 설정하기

## WSL에서 VS Code 사용하기

### WSL 설치하기

웹사이트: https://learn.microsoft.com/ko-kr/windows/wsl/install

1. 관리자 모드로 PowerShell을 엽니다.

2. `wsl --install` 명령어를 실행합니다.

3. 재시작합니다.

4. `wsl --list --online` 명령어를 실행하여 사용 가능한 Linux 배포판을 확인합니다.

   - WSL(Windows Subsystem for Linux): Windows에서 Linux 환경을 사용할 수 있게 해주는 기능입니다.

5. Ubuntu-22.04 배포판을 설치합니다: `wsl --install Ubuntu-22.04`

   - Ubuntu: 가장 인기 있는 Linux 배포판 중 하나입니다.

6. `username`과 `password`를 생성합니다.

7. `wsl -l -v` 명령어를 실행하여 버전을 확인합니다. 버전 2가 표시되어야 합니다.

### WSL 확장 설치하기

1. VS Code 확장에서 WSL을 설치합니다.

### VS Code에서 WSL 열기

1. Ubuntu 앱을 엽니다 (`Windows 키 > Ubuntu`).

2. `code .` 명령어를 실행합니다.

3. 탐색기를 오른쪽 클릭하여 폴더 경로를 확인하고 저장합니다.

## WSL에서 GitHub 설정하기

1. 자격 증명 설정하기

```bash
git config --global user.name "사용자이름"
git config --global user.email "이메일@gmail.com"
```

2. SSH 키 생성 및 복사하기
   - SSH(Secure Shell): 네트워크 상의 다른 컴퓨터에 로그인하거나 원격 시스템에서 명령을 실행할 때 사용되는 프로토콜입니다.

```bash
ssh-keygen -t rsa -b 4096 -C "이메일@gmail.com"
cat ~/.ssh/id_rsa.pub
```

3. GitHub의 `설정 > SSH 및 GPG 키 > 새 SSH 키`에 키를 붙여넣습니다.

4. 저장소를 클론하고 시작할 준비가 되었습니다!

## [dotfiles](https://dotfiles.entelecheia.ai/)를 이용한 셸 설정

다음 가이드는 [Dotfiles 프로젝트](https://dotfiles.entelecheia.ai/)를 여러분의 컴퓨터에 설치하고 초기화하는 과정을 안내해 드립니다. 이를 통해 다양한 시스템에서 일관된 개발 환경을 설정하여 워크플로우를 간소화하고 다른 사람들과의 협업을 보다 쉽게 할 수 있습니다.

### [Dotfiles 설치](https://dotfiles.entelecheia.ai/usage/)

Dotfiles 프로젝트를 여러분의 컴퓨터에 설치하려면, 설치 스크립트를 가져와 실행해야 합니다. `wget` 또는 `curl`을 사용하여 다음을 수행할 수 있습니다:

**`wget` 사용:**

```sh
sh -c "$(wget -qO- https://dotfiles.entelecheia.ai/install)"
```

**`curl` 사용:**

```sh
sh -c "$(curl -fsSL https://dotfiles.entelecheia.ai/install)"
```

위 명령어들은 설치 스크립트를 다운로드하고 실행합니다. 이 과정에서 Dotfiles 저장소가 여러분의 컴퓨터에 복제되고 필요한 설정이 이루어집니다.

### Dotfiles 초기화

설치가 완료되면, 초기화 과정이 자동으로 시작됩니다. 이 단계는 dotfiles에 정의된 설정을 시스템에 적용하여 일관된 개발 환경을 보장합니다.

dotfiles를 수동으로 다시 초기화해야 하는 경우 다음 명령을 실행할 수 있습니다:

```sh
dotu init
```

이 명령은 dotfiles의 최신 설정으로 시스템을 업데이트하여 여러분이 변경한 사항과 환경을 동기화된 상태로 유지합니다.

초기화 과정이 완료되면 새로 설정된 개발 환경을 사용할 수 있습니다. 제공된 설정을 자유롭게 살펴보고 환경 설정에 맞게 사용자 지정하세요.
