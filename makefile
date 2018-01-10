all: server

clean:
	@rm -rf *.o
	@rm -rf server


ThreadPool.o: ThreadPool.cpp ThreadPool.h 
	g++ -g -c -o ThreadPool.o ThreadPool.cpp 

main.o: main.c httpd.h
	g++ -g -c -o main.o main.c

CondVar.o: CondVar.cpp 
	g++ -g -c -o CondVar.o CondVar.cpp

MsgQueue.o: MsgQueue.cpp 
	g++ -g -c -o MsgQueue.o MsgQueue.cpp

PriorityHandler.o: PriorityHandler.cpp 
	g++ -g -c -o PriorityHandler.o PriorityHandler.cpp

RequestLock.o: RequestLock.cpp 
	g++ -g -c -o RequestLock.o RequestLock.cpp

WorkerThread.o: WorkerThread.cpp 
	g++ -g -c -o WorkerThread.o WorkerThread.cpp

RequestWorkerThread.o: RequestWorkerThread.cpp 
	g++ -g -c -o RequestWorkerThread.o RequestWorkerThread.cpp

httpd.o: httpd.c
	gcc -g -c -o httpd.o httpd.c

server: main.o httpd.o  ThreadPool.o RequestWorkerThread.o RequestLock.o CondVar.o MsgQueue.o WorkerThread.o
	g++ -g -o server -lpthread -lrt $^
