g++ -c bagofword.cpp `pkg-config --cflags opencv`
g++ -o bagword bagofword.o Kmeans.o `pkg-config --libs opencv`
