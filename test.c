#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "platform.h"
#include "io.h"
#include "lexical.h"
#include "syntax.h"
#include "compile.h"
#include "linker.h"

static void lng_compile_test_code(const char* text) {
	printf(".");
//	unsigned char* bytes = lng_compile((const unsigned char*) text);
//	assert(bytes);
//	free(bytes);
}

void lng_compile_test() {
	printf("lng_compile_test");
	{
		lng_compile_test_code("package a class a do end");
		lng_compile_test_code("package a class a do int a () do end end");
		lng_compile_test_code("package a class a do int a () do return 1 end end");
		lng_compile_test_code("package a class a do int a () do return 1 end int b () do return 2 end end");
	}
	printf("\n");
}

void lng_lexical_test() {
	printf("lng_lexical_test");
	size_t size;
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 1);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a b";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 2);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a b c";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a(a)";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a.1.3";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a==1";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "string int class";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "1>1";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do 1>1 end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do ++a end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do --a end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a++ end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a-- end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a++-- end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do -1 end";
		struct lng_token_t* tokens = lng_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	printf("\n");
}

static void lng_syntax_test_code(const char* text) {
	printf(".");
	struct lng_syntax_t* self = lng_syntax_new((unsigned char*) text);
	struct lng_node_unit_t* node = lng_syntax_unit(self);
	if (!node) {
		printf("%s\n", text);
	}
	assert(node);
	free(node);
	lng_syntax_free(self);
}

void lng_syntax_test() {
	printf("lng_syntax_test");
	lng_syntax_test_code("package a class b do end");
	lng_syntax_test_code("package a import b.c class d do end");
	lng_syntax_test_code("package a import b.c import d.e class d do end");
	lng_syntax_test_code("package a import b.c import d.e class d do static int a end");
	lng_syntax_test_code("package a import b.c import d.e class d do int main () do return 0 end end");
	lng_syntax_test_code("package a import b.c import d.e class d do int main (int b) do end end");
	lng_syntax_test_code("package a import b.c import d.e class d do int main (int b, int c) do end end");
	lng_syntax_test_code("package a class b do static int a () do ; end end");
	lng_syntax_test_code("package a class b do static int a () do 1 end end");
	lng_syntax_test_code("package a class b do static int a () do -1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1+1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1+1+1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1+1-1*1/1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1+1-1 or 1*1/1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1+1-1 and 1*1/1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1?1:1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1==1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1!=1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1>1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1>=1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1<1 end end");
	lng_syntax_test_code("package a class b do static int a () do 1<=1 end end");
	lng_syntax_test_code("package a class b do static int a () do !true end end");
	lng_syntax_test_code("package a class b do static int a () do true end end");
	lng_syntax_test_code("package a class b do static int a () do false end end");
	lng_syntax_test_code("package a class b do static int a () do (1!=1)==1 end end");
	lng_syntax_test_code("package a class b do static int a () do a end end");
	lng_syntax_test_code("package a class b do static int a () do \"a\" end end");
	lng_syntax_test_code("package a class b do static int a () do a = 1 end end");
	lng_syntax_test_code("package a class b do static int a () do a = b = 2 end end");
	lng_syntax_test_code("package a class b do static int a () do ++a end end");
	lng_syntax_test_code("package a class b do static int a () do --a end end");
	lng_syntax_test_code("package a class b do static int a () do a++ end end");
	lng_syntax_test_code("package a class b do static int a () do a-- end end");
	lng_syntax_test_code("package a class b do static int a () do a++++ end end");
	lng_syntax_test_code("package a class b do static int a () do a---- end end");
	lng_syntax_test_code("package a class b do static int a () do a++-- end end");
	lng_syntax_test_code("package a class b do static int a () do a--++ end end");
	lng_syntax_test_code("package a class b do static int a () do return a+b end end");
	lng_syntax_test_code("package a class b do static int a () do a[1] end end");
	lng_syntax_test_code("package a class b do static int a () do a = b = c[1+1] end end");
	lng_syntax_test_code("package a class b do static int a () do int a end end");
	lng_syntax_test_code("package a class b do static int a () do int a = 1 end end");
	lng_syntax_test_code("package a class b do static int a () do int a = 1 int b = 2 end end");
	lng_syntax_test_code("package a class b do static int a () do if true do end end end");
	lng_syntax_test_code("package a class b do static int a () do while true do end end end");
	lng_syntax_test_code("package a class b do static int a () do repeat do end while true end end");
	lng_syntax_test_code("package a class b do static int a () do for n = 1, 2 do end end end");
	lng_syntax_test_code("package a class b do static int a () do for (1;1;1) do end end end");
	lng_syntax_test_code("package a class b do static int a () do for (1;1;) do end end end");
	lng_syntax_test_code("package a class b do static int a () do for (1;;) do end end end");
	lng_syntax_test_code("package a class b do static int a () do for (;;) do end end end");
	lng_syntax_test_code("package a class b do static int a () do end int b () do end end");
	lng_syntax_test_code("package a class b do int c static int a () do end int d int b () do end end");
	lng_syntax_test_code("package a class a do static int a () do return 1 end static int b () do return 2 end end");
	printf("\n");
}

static int lng_vm_test_lng_code(char* text) {
	printf(".");
	struct lng_node_unit_t* unit = lng_node((const unsigned char*) text);
	struct lng_node_unit_t* nodes[1];
	nodes[0] = unit;
	unsigned char* binary = lng_compile(nodes, 1);
	lng_node_free(unit);
	int result = lng_vm(binary, "a.b", "main", 16 * 1024, 32 * 1024, 256);
	free(binary);
	return result;
}

void lng_vm_test() {
	printf("lng_vm_test");
	{
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 1 end end") == 1);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 10 end end") == 10);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 50 end end") == 50);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return -1 end end") == -1);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return -10 end end") == -10);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return -50 end end") == -50);
	}
	{
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 1+1 end end") == 2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 3-2 end end") == 1);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 2*3 end end") == 6);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 10/5 end end") == 2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 1024+1024 end end") == 1024 + 1024);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return -1024+2048 end end") == 1024);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (2) end end") == 2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return 1+2+3 end end") == 6);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)+3 end end") == 6);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3) end end") == 6);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3+4) end end") == 10);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3+(4+5)) end end") == 15);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)+((3+1)+(4+5)) end end") == 16);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return ((1+2)+2)+((3+1)+(4+5)) end end") == 18);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return ((1+2)+(2+3))+((3+1)+(4+5)) end end") == 21);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1-2)+(3-4) end end") == -2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (-1+2)+(-3+4) end end") == 2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (2*3)+(3*4) end end") == 18);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)*(3+4) end end") == 21);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (1+2)-(3+4) end end") == -4);
		assert(lng_vm_test_lng_code("package a class b do static int main () do return (4+5)/(2+1) end end") == 3);
		assert(lng_vm_test_lng_code("package a class b do static int main () do if true do return 1 end return 2 end end") == 1);
		assert(lng_vm_test_lng_code("package a class b do static int main () do if false do return 1 end return 2 end end") == 2);
		assert(lng_vm_test_lng_code("package a class b do static int main () do if true do return 1 end else do return 2 end end end") == 1);
		assert(lng_vm_test_lng_code("package a class b do static int main () do if false do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return a end end") == 5);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 return a end end") == 5);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 return a + b end end") == 7);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 int c = 4 return a + b + c end end") == 11);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 int c = 4 return (a + b) + c end end") == 11);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 int c = 4 return a + (b + c) end end") == 11);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do boolean a = true if a do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do boolean a = false if a do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 a++ return a end end") == 6);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 a-- return a end end") == 4);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return ++a end end") == 6);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return ++a + a end end") == 12);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return --a end end") == 4);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return --a + a end end") == 8);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return a++ end end") == 5);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return a++ + a end end") == 11);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return a-- end end") == 5);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 return a-- + a end end") == 9);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 == 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 0 != 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 >= 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 <= 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 2 >= 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 0 <= 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 2 > 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 0 < 1 do return 1 end else do return 2 end end end") == 1);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 == 2 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 != 1 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 1 >= 2 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 2 <= 1 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 2 < 1 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do if 0 > 1 do return 1 end else do return 2 end end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 while a > 0 do a-- end return a end end") == 0);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 while a > 0 do a-- end return a end end") == 0);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 while a > 0 do a-- end return b end end") == 2);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 while a > 0 do a-- b++ end return b end end") == 7);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 if a > 0 do a-- b++ end return a end end") == 4);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 if a > 0 do a-- b++ end return b end end") == 3);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 repeat do a-- b++ end while a > 0 return b end end") == 7);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 for (; a > 0 ;) do a-- b++ end return b end end") == 7);
//		assert(lng_vm_test_lng_code("package a class b do static int main () do int a = 5 int b = 2 for (; a > 0 ; a--) do b++ end return b end end") == 7);
		assert(lng_vm_test_lng_code("package a class b do static int main () do int b = 2 for (int a = 5; a > 0 ; a--) do b++ end return b end end") == 7);
	}
	printf("\n");
}

//void lng_ihashmap_test() {
//	printf("lng_ihashmap_test");
//	{
//		printf(".");
//		struct ihashmap* map = lng_ihashmap_new(0);
//		lng_ihashmap_add(map, 1, map);
//		lng_ihashmap_add(map, 10, map);
//		lng_ihashmap_add(map, 20, map);
//		lng_ihashmap_add(map, 500, map);
//		assert(lng_ihashmap_get(map, 1) == map);
//		assert(lng_ihashmap_get(map, 10) == map);
//		assert(lng_ihashmap_get(map, 20) == map);
//		assert(lng_ihashmap_get(map, 500) == map);
//		assert(!lng_ihashmap_get(map, 30));
//		assert(!lng_ihashmap_get(map, 0xFFFF));
//		lng_ihashmap_free(map);
//	}
//	{
//		printf(".");
//		struct ihashmap* map = lng_ihashmap_new(0);
//		unsigned long n;
//		for (n = 0; n < 0xFFFF; n += 2) {
//			lng_ihashmap_add(map, (uint32) n, map);
//		}
//		for (n = 0; n < 0xFFFF; n += 2) {
//			assert(lng_ihashmap_get(map, (uint32 )n) == map);
//		}
//		lng_ihashmap_free(map);
//	}
//	printf("\n");
//}
