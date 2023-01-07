
# GFL Goffi-Language
GFL is a personal language that can transpile to asm x86_64

then with nasm compile to binary code

The idea is to make a native language that can do C stuff without C

For example, read/write files

# examples
## GFL->asm
### write_file 
"""
 
	#import "std/std.gfl" // putf defined here, inside std/string
	ATFDCWD   :: -100
	WRITEONLY :: 1
	SYSopenat :: 257
	proc main(): void {
		var filedescriptor: i64 = SYSCALL3(ATFDCWD, filepath, WRITEONLY, SYSopenat);	
		putsf(filedescriptor, "some text");
	}
"""
### fibonacci
"""

	#import "std/std.gfl" // stoi defined here, inside std/string
	proc main(): void {
	  // stoi = string to i64
	  var lim: i64 = stoi(input("Input the max fib num: "));
	  var x0:  i64  = 0;
	  var x1:  i64  = 1;
	  var acc: i64  = 0;
	  while x0 < lim {
	    x0  = x1 + acc;
	    x1  = acc;
	    acc = x0;
	    // fib num inside x0
	  }
	}
"""
### recursion
"""

	proc factorial(x: i64):*i64 {
	  // allocate some stuff to return  
	  var v:i64 =  1 if x < 2 else factorial(x-1);
	  return x*v;
	}  
	proc main(): void {
	  __print__(
	    factorial(5) // 5! = 120
	  );
	}


"""
### objective 
For now i want to compile the code to C/C++ then slowly make it compiled itself
TODOS:
1. Self compiled Allocator
2. Structs

"""

	GFL -> C -> Binary
	  \-------> Binary
"""
