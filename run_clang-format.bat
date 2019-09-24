@echo off
for /r src\ %%f in (*.cpp) do clang-format -style=file -i %%f
for /r include\lib7842\ %%f in (*.hpp) do clang-format -style=file -i %%f


