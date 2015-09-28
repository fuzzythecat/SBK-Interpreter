# SBK-Interpreter

==Description==
The SBK(쌀보리콩) Interpreter in C++ is a spin off from the glorious Brainfuck language.  
It reads commands from a user-input text file, and ignores every character except 
'쌀', '보리' and '콩', which represents the binary numbers '0' and '1', and a delimiter
respectively. The SBK interpreter reads strings of binary numbers (i.e. 쌀보리보리쌀), 
each tokenized by the delimiter '콩', and turns the string into its corresponding decimal 
number to carry out the designated operation. Any number that does not have its corresponding
operation is ignored, and therefore will not affect the output. "취사" marks the end of the 
text, and every character that comes afterwards will be ignored.   

Its usage and grammar follow those of Brainfuck's. 

==Commands==
The eight language commands, each consisting of a string of characters

String of Characters	|	Decimal Number	|	C equivalent
					          	|			        		|
(Program Start)			  |					        |	char array[100000] = {0};
						          |		        			|	char *ptr = array;
						          |			        		|
쌀						        |	0		        		|	++ptr;
보리					        |	1		        		|	--ptr;
보리쌀				      	|	2		        		|	++*ptr;
보리보리				      |	3		        		|	--*ptr;
보리쌀쌀				      |	4		        		|	putchar(*ptr);
보리쌀보리				    |	5		        		|	*ptr = getchar();
보리보리쌀				    | 6		        		|	while (*ptr) {
보리보리보리			    |	7		        		|	}

==Example==
Check the file "helloworld.txt"

==Alerts==
The program currently comes with a bug: at the end of the text, put "보리보리보리콩" right
before you end it with "취사". For some reason it self-goes into while-loop before it closes.

==Developer(s)==
Younjoon Chung <fuzzy0427@gmail.com>

please contact through the above e-mail address for any inquiries about the project. 

