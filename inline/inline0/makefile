oj_server:oj_server.cc oj_model.hpp oj_view.hpp
	g++ oj_server.cc -o oj_server -std=c++11 -lpthread\
		-ljson  -L /usr/local/lib -lctemplate -I ~/third_part/include -L ~/third_part/lib
compile_server:compiler_online.cc compiler.hpp
	g++ -std=c++11 $^ -o $@ -pthread -ljsoncpp
test:test.cc
		g++ $^ -o $@ -std=c++11 -pthread -ljsoncpp		
.PHONY:clean
clean:
	rm -rf test

