:: SPDX-License-Identifier: GPL-3.0-only

@echo off
REM Fail early on Windows with a clear message for users/CI
echo ERROR: Can't build. This package is made for MacOS only. 1>&2
echo Please build on macOS (or use a compatible container/runner). 1>&2
exit /b 1