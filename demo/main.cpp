#include <iostream>
#include <ucontext.h>
#include <memory>
#include <vector>
#include "macros.hpp"

volatile bool* bored = new bool(false);
volatile int* warn = new int;

void child() {
	logger localVal;
	std::cout<<"Entered child"<<std::endl;
	*bored = true;
	registerContext();
	std::cout<<"warn is: "<<*warn << std::endl;
	if(!(*warn)++) {
		std::cout<<"warn is: "<<*warn << std::endl;
		goUp();
	}
}

void parent() {
	logger localVal;
	registerContext();
	if(!*bored)
		child();
	if(!*bored){
		std::cout<<"going back in"<<std::endl;
		goDown();
	}
}


int main() {
	(*warn) = 0;
	parent();
	std::cout<<"Registered contexts: "<<contextStack.contexts.size()<<std::endl;
}
