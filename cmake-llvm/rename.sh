find . -name "*.cpp.o" -exec bash -c 'mv "$1" "${1%.cpp.o}".ll' - '{}' \;
