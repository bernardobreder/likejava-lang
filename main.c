#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include "stringbuilder.h"
#include "socket.h"
#include "lexical.h"
#include "compile.h"
#include "linker.h"
#include "syntax.h"
#include "vm.h"
#include "node.h"

int main(void) {
	for (;;) {
		uint8_t size = 1;
		struct lng_node_unit_t* nodes[size];
		struct lng_node_unit_t* node = lng_node((unsigned char*) "package a class a do int main () do return (1+1)*(5-2)/2 end end");
		nodes[0] = node;
		unsigned char* bytes = lng_compile(nodes, size);
		lng_vm(bytes, "a.a", "main", 16 * 1024, 32 * 1024, 256);
		free(bytes);
		free(node->tokens);
		free(node);
        
        lng_lexical_test();
        lng_syntax_test();
        lng_compile_test();
        lng_vm_test();

	}
	return 0;
}

