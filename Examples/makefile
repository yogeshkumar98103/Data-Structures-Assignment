FLAGS = -std=c++11

files = Stack.o Queue.o

Final : ${files}

Stack.o : Stack.cpp ../Library/stack.h ../Library/stack.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o executables/$@

Queue.o : Queue.cpp ../Library/queue.h ../Library/queue.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o executables/$@

clean:
	cd ./executables
	pwd
	# @echo "Cleaning..."
	# @rm ${files}
	# @cd ..
