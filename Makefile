CC = clang
CXX = clang++# Flag for implicit rules
CFLAGS = -g
# CPPFLAGS = -I./include -I/usr/include/c++/9 -I/usr/include/x86_64-linux-gnu/c++/9 -stdlib=libc++
CPPFLAGS = -I/usr/include -I/usr/local/include -stdlib=libc++
OATPP_SRC = ''

# edit: dir oat_http_router oat_http_connection_handler oat_address oat_server oat_environment
edit: dir oat
	# $(CXX) src/main.cc -o build/main.o $(CPPFLAGS) $(CXXFLAGS)
	$(CXX) ./build/oat.so src/ignis.cpp -o build/ignis.o $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f ./build/*.o

dir:
	@if [ ! -d "./build" ]; then mkdir ./build; fi

main:
	$(CXX) src/main.cc -o build/main.o $(CPPFLAGS) $(CXXFLAGS)

run: edit
#	./build/main.o
	./build/ignis.o

oat:
	@if [ ! -e "./build/oat.so" ]; then $(CXX) --shared -fPIC -o ./build/oat.so $(shell find ./include -name "*.cpp") $(CPPFLAGS) $(CXXFLAGS); fi
    
test:
	$(CXX) src/test.cc -o build/test.o $(CPPFLAGS) $(CXXFLAGS)
	./build/test.o

tmp:
	$(CXX) src/tmp.cc -o build/tmp.o $(CPPFLAGS) $(CXXFLAGS)
	./build/tmp.o
