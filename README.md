# 운영체제 2024

[![halla-img]][halla-url]
[![course-img]][course-url]
[![lecture-img]][lecture-url]
[![pypi-image]][pypi-url]
[![version-image]][release-url]
[![release-date-image]][release-url]
[![license-image]][license-url]
[![codecov][codecov-image]][codecov-url]
[![jupyter-book-image]][docs-url]

<!-- Links: -->

[halla-img]: https://img.shields.io/badge/CHU-halla.ai-blue
[halla-url]: https://halla.ai
[course-img]: https://img.shields.io/badge/course-entelecheia.ai-blue
[course-url]: https://course.entelecheia.ai
[lecture-img]: https://img.shields.io/badge/lecture-entelecheia.ai-blue
[lecture-url]: https://lecture.entelecheia.ai
[codecov-image]: https://codecov.io/gh/chu-aie/os-2024/branch/main/graph/badge.svg?token=WvzaewFR3I
[codecov-url]: https://codecov.io/gh/chu-aie/os-2024
[pypi-image]: https://img.shields.io/pypi/v/os2024
[license-image]: https://img.shields.io/github/license/chu-aie/os-2024
[license-url]: https://github.com/chu-aie/os-2024/blob/main/LICENSE
[version-image]: https://img.shields.io/github/v/release/chu-aie/os-2024?sort=semver
[release-date-image]: https://img.shields.io/github/release-date/chu-aie/os-2024
[release-url]: https://github.com/chu-aie/os-2024/releases
[jupyter-book-image]: https://jupyterbook.org/en/stable/_images/badge.svg
[repo-url]: https://github.com/chu-aie/os-2024
[pypi-url]: https://pypi.org/project/os2024
[docs-url]: https://os2024.halla.ai
[changelog]: https://github.com/chu-aie/os-2024/blob/main/CHANGELOG.md
[contributing guidelines]: https://github.com/chu-aie/os-2024/blob/main/CONTRIBUTING.md

<!-- Links: -->

Operating Systems 2024 Class

- Documentation: [https://os2024.halla.ai][docs-url]
- GitHub: [https://github.com/chu-aie/os-2024][repo-url]
- PyPI: [https://pypi.org/project/os2024][pypi-url]

이 수업에서는 컴퓨터 시스템의 작동에 필수적인 운영체제의 설계, 기능, 관리에 대해 포괄적으로 다룹니다. 프로세스 관리, 메모리 관리, 파일 시스템 등 운영체제의 이론적・실질적 측면과 리눅스 운영체제에 대한 깊은 이해를 얻게 됩니다. 강의, 실습, 프로젝트를 통해 운영체제의 작동 방식, 하드웨어 및 소프트웨어와의 상호 작용, 운영체제 리소스의 효율적인 관리 방법을 배울 것입니다.

## Installation

To install the OS 2024 package, use the following command:

```
pip install os2024
```

Or

```
pip install --user os2024
```

The `--user` flag is optional and can be used to install the package in the user's home directory instead of the system-wide location.

## Usage

To use the OS 2024 CLI, run the following command:

```
os2024 [OPTIONS]
```

If no option is provided, the website of the book will open in the default web browser.

### Options

The following options are available:

- `--version`: Show the version of the package and exit.
- `-b`, `--build`: Build the book.
- `-l`, `--local`: Open the locally built HTML version of the book in the browser.
- `--help`: Show the help message and exit.

### Examples

1. To build the book, use the following command:

   ```
   os2024 --build
   ```

   This will trigger the build process for the book.

2. To open the locally built HTML version of the book in the browser, use the following command:

   ```
   os2024 --local
   ```

   This will open the book's HTML file in your default web browser.

3. To view the version of the package, use the following command:

   ```
   os2024 --version
   ```

   This will display the version number of the OS 2024 package.

For more information and additional options, run `os2024 --help` to see the help message.

## Changelog

See the [CHANGELOG] for more information.

## Contributing

Contributions are welcome! Please see the [contributing guidelines] for more information.

## License

This project is released under the [CC-BY-4.0 License][license-url].
