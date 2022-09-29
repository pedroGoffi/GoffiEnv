" Language: gfsl

if exists("b:current_syntax")
  finish
endif

set iskeyword=a-z,A-Z,-,*,_,!,@
syntax keyword gfslTodos TODO XXX FIXME NOTE

syntax keyword gfslKeywords  if else while do end define static as include 


" Comments
syntax region gfslCommentLine start="//" end="$"   contains=gfslTodos

" String literals
syntax region gfslString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=gfslEscapes

" Char literals
syntax region gfslChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=gfslEscapes

" Escape literals \n, \r, ....
syntax match gfslEscapes display contained "\\[nr\"']"

" Number literals
syntax region gfslNumber start=/\s\d/ skip=/\d/ end=/\s/

" Type names the compiler recognizes
syntax keyword gfslTypeNames mem store8 load8 store64 load64 syscall0 syscall1 syscall2 syscall3 syscall4 syscall5 syscall6
" Set highlights
highlight default link gfslTodos Todo
highlight default link gfslKeywords Keyword
highlight default link gfslCommentLine Comment
highlight default link gfslString String
highlight default link gfslNumber Number
highlight default link gfslTypeNames Type
highlight default link gfslChar Character
highlight default link gfslEscapes SpecialChar

let b:current_syntax = "gfsl"

