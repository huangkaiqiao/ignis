CC = clang
CXX = clang++ # Flag for implicit rules
CFLAGS = -g
# CPPFLAGS = -I./include -I/usr/include/c++/9 -I/usr/include/x86_64-linux-gnu/c++/9 -stdlib=libc++
CPPFLAGS = -I./include -stdlib=libc++

clean:
	rm -f ./build/*.o

dir:
	@if [ ! -d "./build" ]; then mkdir ./build; fi

edit: dir
	$(CXX) src/main.cpp -o build/main.o $(CPPFLAGS) $(CXXFLAGS)

run: edit
	./build/main.o

test:
	$(CXX) src/test.cc -o build/test.o $(CPPFLAGS) $(CXXFLAGS)
	./build/test.o

tmp:
	$(CXX) src/tmp.cc -o build/tmp.o $(CPPFLAGS) $(CXXFLAGS)
	./build/tmp.o