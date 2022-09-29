#ifndef __VM
#define __VM
#include "../../common/utils.cpp"
#include "./instruction.cpp"
#include <vector>
namespace VM{
  void simulate(operation** program){
    std::vector<Value> stack;
    size_t ip = 0;
    operation* op;
    while((op = program[ip]) != NULL){
    //operation* op = program[i];
      switch(op->kind){
      case operation_kind::noop:
	printf("this is a noop.\n");
	ip++;
	break;
	
      case operation_kind::push:
	stack.push_back(op->as);
	ip++;
	break;	

      case operation_kind::add_INT: {
	auto a = stack.back(); stack.pop_back();
	auto b = stack.back(); stack.pop_back();
        Value result = b;
	result.INT += a.INT;
	stack.push_back(result);
	ip++;
      } break;
	
      case operation_kind::dump_INT: {
	auto a = stack.back(); stack.pop_back();
	printf("%i", a.INT);
	ip++;
      } break;
	
      case operation_kind::dump_STRING: {
	auto a = stack.back(); stack.pop_back();
	printf("%s", a.STRING);
	ip++;
      } break;
      case operation_kind::cmp_lt: {
	auto a = stack.back(); stack.pop_back();
	auto b = stack.back(); stack.pop_back();
	// a < b
	Value result = a;
	result.BOOL  = (a.INT < b.INT);
	stack.push_back(result);
	ip++;
      } break;
      case operation_kind::jmp: {
	ip += op->as.INT;
      } break;
      case operation_kind::jmp_if: {
	auto a = stack.back(); stack.pop_back();
	if(a.BOOL){
	  ip += op->as.INT;
	}
	else {
	  ip++;
	}
      } break;
      case operation_kind::_abort: {
	printf("ABORT not implemented yet.\n");
	exit(1);
	ip++;
      } break;
      case operation_kind::halt: {
	goto end_of_simulation;
      } break;
      default:
	fprintf(stderr,
		"ERROR: unexpected instruction kind.\n");
	exit(1);
      }
    }
  end_of_simulation:
    printf("END OF SIM\n");
  }

  void test_vm(){
    operation** test = nullptr;
  
#define make_op(...)				\
    buf__push(test, new operation{__VA_ARGS__});

    make_op(.kind=push,		.as={.INT = 12});
    make_op(.kind=push,		.as={.INT = 10});
    make_op(.kind=cmp_lt);
    make_op(.kind=jmp_if,	.as={.INT=4});
    make_op(.kind=push,		.as={.STRING="hello"});
    make_op(.kind=dump_STRING);
    make_op(.kind=jmp,          .as={.INT=-8});
    make_op(.kind=halt);
    
    VM::simulate(test);
  }
}
#endif /* __VM */
