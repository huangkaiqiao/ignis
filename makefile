# makefile:
CXX:= clang++-12
CXXFLAGS:= --include-directory ./include

edit:
	@$(CXX) src/main.cc -o bin/ignis $(CXXFLAGS)

all: ignis

clean:
	$(RM) bin/ignis

run: edit
	@bin/ignis