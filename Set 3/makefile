FLAGS = -std=c++11

files = HashTable.o Treap.o DisjointSet.o LinkedList.o

Final : ${files}

HashTable.o : ./HashTable/Example.cpp ./HashTable/HashTable.h ./HashTable/HashTable.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o ./executables/$@

Treap.o : ./Treap/Example.cpp ./Treap/Treap.h ./Treap/Treap.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o ./executables/$@

DisjointSet.o : ./DisjointSet/Example.cpp ./DisjointSet/DisjointSet.h ./DisjointSet/DisjointSet.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o ./executables/$@

LinkedList.o : ./LinkedList/Example.cpp ./LinkedList/LinkedList.h ./LinkedList/LinkedList.cpp
	${CXX} $(FLAGS) $(CXXFLAGS) $< -o ./executables/$@

clean:
	@echo "Cleaning...";
	@cd executables && pwd && rm ${files} && cd ..
