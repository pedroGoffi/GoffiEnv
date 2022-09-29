# Goffi-SLang
Goffi-SLang is a [stack oriented language](https://en.wikipedia.org/wiki/Stack-oriented_programming) inspired in [Porth](https://gitlab.com/tsoding/porth) writed in C++
that can be compiled or simulated.

## Summary
* [Simple math](#Aritmetric-Operators)
* [Built-in Procedures](#Build-in-Procedures)
* [Flow Control](#Flow-Control)
* [Macros](#Macro-definition)
* [standart lib](#stdlib)
* [argc and argv](#argc-argv)
* [static alocation](#static-keyword)
## <div id="Aritmetric-Operators">Aritmetric-Operators</div>	
    result of any operation will be pushed automatically ot the top of the stack
	In comparasions `<` or `>` will push 1 if true and 0 if false.

`=`	EQUALS operand, Usage: `1 1 =` is equivalent to (1 == 1)

`+`	PLUS operand. Usage:  `2 1 +` is equivalent to (2+1) 

`-`	MINUS operand, Usage: `2 1 -` is equivalent to (2-1) 

`<`	LESS THAN operand, Usage:  `2 1 <` is equivalent to (2<1) 

`>`	GREATER THAN operand, Usage:  `2 1 >` is equivalent to (2>1) 

`*`	MULTIPLY operand, example: `2 1 *` is equivalent to (2*1)




# <div id="Build-in-Procedures">Build-in Procedures </div>

`dump` print the last (int) digit in the stack

--`64->` ptr -- *ptr
--`64<-` value ptr --- *ptr = value


# <div id="Flow-Control">Flow Control</div>



`if` if->expression->do-block will goto next do-block or jump the whole do-block.
    if 1 do 
	69 dump 
    end

`do` Always close end blocks this will perfome instructions jumps in some scenarios.
    do 1 dump end

`while` while->expr->do-block if expr is false will jump the do-block, otherwise will evaluate the do-block.
    0 while dup 10 < do
	dup dump
	1 +
    end


# <div id="Macro-definition">Macros</div>

--`define`  is used to created a macro, that will be expanded when used 

    example:
      define N as 69 end
      N dump 
    will dump 69

More examples in examples folder, kinda obvious.


# <div id="stdlib"> standart library </div>
## Some procedures in std
    All proc are currently macros

-- `print`  dup dump

-- `puts`   stdout fputs

-- `eputs`  stderr fputs

-- `fputs`  write drop

-- `write`  SYS_write syscall3 

-- `exit`   SYS_exit syscall1 end



# <div id="argc-argv">Argument count and argument value</div>

To access command argc/argv you can use the built-in keyword __argc and __argv

__argc and __argv

NOTE: __argc is a pointer so to access you have to dereference the ptr. 

Example: __argc 64-> dump

OBS: 64-> is inside std.gfsl but is equivalent to load64


# <div id="static-keyword">Static-alocation</div>
-- `static` keyword will try to parse in compiler-time the value of the expression

Example:

:: static my-static 10 end

will reserve 10 bytes in the bss

you can chain static with macros

define X as sizeof(str) end

static my-bss as X 10 + end

will evaluate it as

static my-bss 16 end
