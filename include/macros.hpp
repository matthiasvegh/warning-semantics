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
	struct _SG__LINE__{ ~_SG__LINE__() { std::cout<<"unwinding stack."<<std::endl;contextStack.contexts.pop_back(); --contextStack.contextIndex; }} _sG__LINE__; \

#define goUp() \
	std::cout << "registered contexts: " << contextStack.contexts.size() << \
			" current index: " << contextStack.contextIndex << std::endl; \
	--contextStack.contextIndex; \
	swapcontext(&contextStack.contexts[contextStack.contextIndex+1], \
			&contextStack.contexts[contextStack.contextIndex]); \

#define goDown() \
	std::cout<<"Entering stack frame "<<contextStack.contextIndex+1<<std::endl; \
	setcontext(&contextStack.contexts[contextStack.contextIndex+1]); \


