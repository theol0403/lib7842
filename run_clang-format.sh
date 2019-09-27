# @echo off
# for /r src\ %%f in (*.cpp) do clang-format -style=file -i %%f
# for /r include\lib7842\ %%f in (*.hpp) do clang-format -style=file -i %%f

# find include/lib7842/ -iname *.hpp | xargs clang-format-6.0 -i -style=file
# find src/ -iname *.cpp | xargs clang-format-6.0 -i -style=file

for i in $(find src/ -iname *.cpp); do clang-format-9 -i -style=file $i || true
done

for i in $(find include/lib7842/ -iname *.hpp); do clang-format-9 -i -style=file $i || true
done


