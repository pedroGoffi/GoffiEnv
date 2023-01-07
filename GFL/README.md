
# GFL Goffi-Language
GFL é uma linguagem pessoal, que pode transpilar para assembly x86 64 bits

e então com NASM pode compiler para codigo binario

a ideia é fazer uma linguagem NATIVA com coisas que C possui, mas sem usar C

por exemplo, ler/escrever arquivos

OBS: NEM TUDO VAI SER TRADUZIDO! como algumas palavras ja são tão comuns em programação, como INPUT/OUTPUT/PRINT irei evitar traduzi-las para evitar algoritmos com nomes muito extensos na biblioteca padrao.

# exemplos
## GFL->asm
### escrever arquivos
"""
 
	#incluir "std/padrao.gfl" // putf defined here, inside std/string
	ATFDCWD   :: -100
	WRITEONLY :: 1
	SYSopenat :: 257
	algoritmo main(): void {
		var filedescriptor: i64 = SYSCALL3(ATFDCWD, filepath, WRITEONLY, SYSopenat);	
		putsf(filedescriptor, "some text");
	}
"""
### fibonacci
"""

	#incluir "std/padrao.gfl" // stoi defined here, inside std/string
	algoritmo main(): void {
	  var lim: i64 = stoi(input("Digite o numero maxio, para calcular fibonacci: "));
	  var x0:  i64  = 0;
	  var x1:  i64  = 1;
	  var acc: i64  = 0;
	  enquanto x0 < lim {
	    x0  = x1 + acc;
	    x1  = acc;
	    acc = x0;
	  }
	}
"""
### recurssão
"""

	algoritmo factorial(x: i64):*i64 {
	  var v:i64 =  1 se x < 2 senao factorial(x-1);
	  retorne x*v;
	}  
	algoritmo main(): void {
	  __print__(
	    factorial(5) // 5! = 120
	  );
	}


"""
### objetivo
Por hora meu objetivo é implementar:
1. Procedimentos pre-processadors, sintaxe desejada: "NomeAlgoritmo::(Variaveis)"
2. Arrays
3. Malloc/Free
4. Self hosting

### GFL 'Boas praticas'
Coloquei entre aspas, pois não considero boas praticas em uma linguagem algo serio.

1. Espaço entre algoritmos 1 linha
2. Priorizar uso cedo para evitar muitos 'se/senao'
3. 
