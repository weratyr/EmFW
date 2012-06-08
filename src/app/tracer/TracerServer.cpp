//============================================================================
// Name        : TracerServer.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TracerServer.h"
using namespace std;

int main() {

//	printf("Start Tracer-Server");

	CInetAddr cAddr;
	cAddr.set(1234, "localhost");

	CSockAcceptor cAcc;
	Int8 *memPtr;
	cAcc.open(cAddr);
	CBinarySemaphore *sem = new(memPtr) CBinarySemaphore(false, true);

	CCommQueue *CD_Queue = new(memPtr) CCommQueue(100, *sem);

	CSockStream cStream;
	int newsockfd;
	CMessage tempMsg;
	int pid;


	while(true) {
		newsockfd = cAcc.accept(cStream);
		if((pid = fork()) == 0) {
			while(cStream.read(&tempMsg, sizeof(CMessage))) {
				switch(tempMsg.getReceiverID()) {
				case CD_INDEX : CD_Queue.add(tempMsg);
								break;
				default : break;
				}
			}
		}
	}

}
