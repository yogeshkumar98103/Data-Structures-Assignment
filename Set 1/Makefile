COMP = g++
FLAGS = -std=c++11
executables = executables

files1 = BinaryTree.cpp BinarySearchTree.cpp
files2 = AVLTree.cpp Stack.cpp

objectFiles1 = ${subst .cpp,.o, ${files1}}
objectFiles2 = ${subst .cpp,.o, ${files2}}
Objects1 = ${patsubst %, ${executables}/%, ${objectFiles1}} 
Objects2 = ${patsubst %, ${executables}/%, ${objectFiles2}}
# headerFiles = ${subst .cpp,.h, ${files}}
# folders = ${subst .cpp,, ${files}}

Final : ${Objects1} ${Objects2}

${executables}/BinarySearchTree.o : ./BinarySearchTree/Example.cpp  BinarySearchTree/BinarySearchTree.cpp
	${COMP} $(FLAGS) $^ -o $@

${executables}/BinaryTree.o : ./BinaryTree/Example.cpp  BinaryTree/BinaryTree.cpp
	${COMP} $(FLAGS) $^ -o $@

${executables}/AVLTree.o : ./AVLTree/Example.cpp  AVLTree/AVLTree.cpp AVLTree/AVLTree.h AVLTree/Sort.cpp AVLTree/Sort.h
	${COMP} $(FLAGS) -o ${executables}/AVLTree.o $< 

${executables}/Stack.o : ./Stack/Example.cpp  Stack/Stack.cpp Stack/Stack.h
	${COMP} $(FLAGS) -o ${executables}/Stack.o $< 


# ${executables}/%.o : ./%/Example.cpp  %.oo
# 	#${COMP} $(FLAGS) $^ -o $@
# 	@echo "Build Without Header" ./%/Example.cpp

# %.oo : 
# 	@echo "Builiding" %.o

${executables}/%.oh : ./%/Example.cpp  ./%/%.cpp ./%/%.h
	${COMP} $(FLAGS) ./%/Example.cpp  ./%/%.cpp -o ${executables}/%.o

clean:
	@echo "Cleaning...";
	@cd ${executables} && rm ${objectFiles} && cd ..
