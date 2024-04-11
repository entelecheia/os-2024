# 프로젝트 환경설정

이 가이드는 운영체제 수업을 위한 프로젝트 환경을 설정하는 방법을 자세히 설명합니다. 프로젝트를 시작하기 전에 아래 단계를 따라 환경을 설정해주세요.

## 리포 복제

먼저, [os-2024](https://github.com/chu-aie/os-2024.git) 저장소를 자신의 GitHub 계정으로 복제(Fork)합니다. 복제한 저장소의 URL은 `https://github.com/[your_github_id]/os-2024.git`와 같은 형식이 될 것입니다.

그 다음, 복제한 저장소를 로컬 컴퓨터로 클론합니다. 아래 명령을 터미널에서 실행하세요:

```bash
mkdir -p ~/workspace/projects \
    && git clone https://github.com/[your_github_id]/os-2024.git ~/workspace/projects/os-2024 \
    && cd ~/workspace/projects/os-2024
```

- `mkdir -p ~/workspace/projects`: `~/workspace/projects` 디렉토리를 생성합니다. 이 디렉토리는 프로젝트를 저장할 공간입니다.
- `git clone https://github.com/[your_github_id]/os-2024.git`: 복제한 저장소를 로컬로 클론합니다. `[your_github_id]` 부분은 본인의 GitHub 아이디로 대체해주세요.
- `cd ~/workspace/projects/os-2024`: 클론한 저장소의 디렉토리로 이동합니다.

## 가상 환경 생성 및 활성화

다음으로, 프로젝트를 위한 Python 가상 환경을 생성하고 활성화합니다. 아래 명령을 실행하세요:

```bash
direnv allow
```

이 명령은 리포 디렉토리에 있는 `.envrc` 파일을 실행합니다. `.envrc` 파일은 프로젝트에 필요한 환경 변수를 설정하고, 가상 환경을 활성화합니다. 파일의 내용은 다음과 같습니다:

```bash
#!/usr/bin/env bash

export PYTHON_KEYRING_BACKEND="keyring.backends.null.Keyring"
export PROJECT_NAME="os2024"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PROJECT_ROOT

VIRTUAL_ENV="${WORKON_HOME}/${PROJECT_NAME}"
if [ -e "${VIRTUAL_ENV}/bin/activate" ]; then
    # shellcheck disable=SC1091
    source "${VIRTUAL_ENV}/bin/activate"
else
    python3 -m venv "${VIRTUAL_ENV}"
    # shellcheck disable=SC1091
    source "${VIRTUAL_ENV}/bin/activate"
    pip install --upgrade pip setuptools wheel
fi
```

이 스크립트는 다음과 같은 작업을 수행합니다:

1. 필요한 환경 변수를 설정합니다.
2. 프로젝트 이름(`os2024`)으로 Python 가상 환경을 생성합니다.
3. 가상 환경을 활성화합니다.
4. 가상 환경에 최신 버전의 `pip`, `setuptools`, `wheel`을 설치합니다.

`direnv allow` 명령을 실행하면, 이제 리포 디렉토리에 진입할 때마다 가상 환경이 자동으로 활성화됩니다.

## 프로젝트 의존성 설치

프로젝트에 필요한 Python 패키지를 설치하기 위해 아래 명령을 실행합니다:

```bash
poetry install --with dev
```

이 명령은 `pyproject.toml` 파일에 정의된 모든 의존성 패키지를 설치합니다. `--with dev` 옵션은 개발에 필요한 패키지도 함께 설치합니다.

설치가 완료되면, 다음 명령을 실행하여 프로젝트가 제대로 설치되었는지 확인합니다:

```bash
os2024
```

이 명령은 프로젝트의 기본 스크립트를 실행합니다. 정상적으로 설치되었다면, 프로젝트 정보가 출력될 것입니다.

## 리포 동기화 및 프로젝트 브랜치 생성

다음으로, 본인이 복제한 리포를 원본 리포([os-2024](https://github.com/chu-aie/os-2024.git))와 동기화합니다. 이를 위해 원본 리포를 `upstream`으로 추가하고, 최신 변경사항을 가져옵니다:

```bash
git remote add upstream https://github.com/chu-aie/os-2024.git \
    && git fetch upstream
```

- `git remote add upstream https://github.com/chu-aie/os-2024.git`: 원본 리포를 `upstream`이라는 이름으로 추가합니다.
- `git fetch upstream`: `upstream` 리포의 최신 변경사항을 가져옵니다.

그런 다음, 프로젝트를 위한 새 브랜치를 생성합니다. 브랜치 이름은 `your_github_id/[project_patch_#]` 형식으로 짓습니다. 예를 들어, GitHub 아이디가 `johndoe`이고 첫 번째 프로젝트라면 브랜치 이름은 `johndoe/project_patch_1`이 됩니다. 아래 명령을 실행하세요:

```bash
git checkout -b your_github_id/[project_patch_#] main
```

이 명령은 `main` 브랜치로부터 새 브랜치를 생성하고, 그 브랜치로 전환합니다.

```{note}
**주의사항:** 주별 과제의 경우 브랜치 이름에 주차 번호를 포함하는 것이 좋습니다. 예를 들어, 6주차 과제라면 브랜치 이름을 `your_github_id/week06`로 지정합니다:

> git checkout -b your_github_id/week06 main
```

## 프로젝트 폴더 생성

이제 프로젝트를 위한 폴더를 생성합니다. 템플릿 폴더를 복사하여 프로젝트 폴더를 생성하세요:

```bash
cp -r src/os2024/book/projects/template src/os2024/book/projects/your_github_id
```

이 명령은 `src/os2024/book/projects/template` 폴더를 `src/os2024/book/projects/your_github_id` 폴더로 복사합니다. `your_github_id` 부분은 본인의 GitHub 아이디로 대체해주세요.

그런 다음, `src/os2024/book/projects/your_github_id/index.md` 파일을 열어 프로젝트 설명을 작성합니다. 이 파일은 프로젝트의 메인 페이지 역할을 합니다. 파일을 열어 다음과 같이 수정하세요:

1. 제목을 본인의 프로젝트 이름으로 변경합니다.
2. 프로젝트 설명을 작성합니다. 프로젝트의 목적, 주요 기능, 사용 기술 등을 설명합니다.

```markdown
# 프로젝트 제목

프로젝트 설명을 여기에 작성합니다. 프로젝트의 목적, 주요 기능, 사용 기술 등을 설명합니다.

## 소제목 추가

내용 작성
```

## 프로젝트 커밋

프로젝트 폴더를 생성하고 설명을 작성했으면, 변경 사항을 커밋합니다:

```bash
git add src/os2024/book/projects/your_github_id \
    && git commit -m "feat: initialize project directory of your_github_id" \
    && git push --set-upstream origin your_github_id/[project_patch_#]
```

이 명령은 다음과 같은 작업을 수행합니다:

1. `src/os2024/book/projects/your_github_id` 폴더를 Git 스테이징 영역에 추가합니다.
2. "feat: initialize project directory of your_github_id"라는 메시지로 커밋을 생성합니다.
3. 커밋을 `your_github_id/[project_patch_#]` 브랜치에 푸시합니다.

## PR 생성

GitHub에서 본인의 리포지토리로 이동하여 `your_github_id/[project_patch_#]` 브랜치를 선택합니다. 그런 다음, "Pull Request" 버튼을 클릭하여 새 PR을 생성합니다.

PR 제목은 "feat: Initialize project directory of your_github_id"로 설정하세요.

## 프로젝트 진행

이제 프로젝트를 진행할 준비가 되었습니다. 프로젝트를 진행하면서 필요한 파일을 생성하고, 코드를 작성하세요.

주기적으로 다음 명령을 실행하여 변경 사항을 커밋하고, PR을 생성하세요:

```bash
git add . \
    && git commit -m "feat: update project progress by your_github_id" \
    && git push origin your_github_id/[project_patch_#] \
    && gh pr create --title "feat: Update project progress by your_github_id" --body "This PR updates project progress by your_github_id"
```

이 명령은 다음과 같은 작업을 수행합니다:

1. 모든 변경 사항을 Git 스테이징 영역에 추가합니다.
2. "feat: update project progress by your_github_id"라는 메시지로 커밋을 생성합니다.
3. 커밋을 `your_github_id/[project_patch_#]` 브랜치에 푸시합니다.
4. "feat: Update project progress by your_github_id"라는 제목과 "This PR updates project progress by your_github_id"라는 내용으로 새 PR을 생성합니다.

## 프로젝트 완료

프로젝트를 완료했으면, 마지막으로 다음 명령을 실행하여 변경 사항을 커밋하고, 최종 PR을 생성합니다:

```bash
git add . \
    && git commit -m "feat: complete project by your_github_id" \
    && git push origin your_github_id/[project_patch_#] \
    && gh pr create --title "feat: Complete project by your_github_id" --body "This PR completes project by your_github_id"
```

이 명령은 앞서 설명한 것과 유사한 작업을 수행하지만, 커밋 메시지와 PR 제목/내용이 프로젝트 완료를 나타낸다는 점이 다릅니다.

이상으로 프로젝트 환경설정 가이드를 마칩니다. 프로젝트를 즐겁게 진행하시기 바랍니다!
