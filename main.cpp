#include <iostream>
#include <ucontext.h>
#include <memory>
#include <vector>

using context = ucontext_t;

struct ContextStack {
	std::vector<context> contexts;
	std::size_t contextIndex = 0;
};

struct logger {
	logger() { std::cout<<"creating local variable: "<<this<<std::endl; }
	~logger() { std::cout<<"destroying local variable: "<<this<<std::endl; }
};

ContextStack contextStack;

#define registerContext() \
	std::cout<<"registering new context"<<std::endl; \
	contextStack.contexts.emplace_back(); \
	getcontext(&(contextStack.contexts.back())); \
	++contextStack.contextIndex; \

#define goUp() \
	std::cout << "registered contexts: " << contextStack.contexts.size() << \
			" current index: " << contextStack.contextIndex << std::endl; \
	swapcontext(&contextStack.contexts[contextStack.contextIndex], \
			&contextStack.contexts[--contextStack.contextIndex]); \

#define goDown() \
	std::cout<<"Entering stack frame "<<contextStack.contextIndex+1<<std::endl; \
	setcontext(&contextStack.contexts[contextStack.contextIndex+1]); \

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

}
