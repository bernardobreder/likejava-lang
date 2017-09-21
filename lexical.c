#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "lexical.h"

struct lng_token_t* lng_lexical(const unsigned char* text, size_t* tokenSize) {
	unsigned size = 0;
	{
		unsigned char* pc = (unsigned char*) text;
		unsigned char c = *pc;
		while (c) {
			if (c <= ' ') {
			} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
				long begin = pc - text;
				do {
					c = *(++pc);
				} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
				if ((pc - text) - begin > 0xFFFFFFFF) {
					return 0;
				}
				size++;
				pc--;
			} else if (c == '0' && pc[1] == 'x') {
				long begin = pc - text;
				pc++;
				c = *(++pc);
				do {
					c = *(++pc);
				} while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
				if ((pc - text) - begin > 10) {
					return 0;
				}
				size++;
				pc--;
			} else if (c >= '0' && c <= '9') {
				long begin = pc - text;
				c = *(++pc);
				while (c >= '0' && c <= '9') {
					c = *(++pc);
				}
				if ((pc - text) - begin > 20) {
					return 0;
				}
				size++;
				pc--;
			} else if (c == '\"' || c == '\'') {
				long begin = pc - text;
				c = *(++pc);
				while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
				if ((pc - text) - begin > 0xFFFFFFFF) {
					return 0;
				}
				size++;
			} else if (c == '/' && pc[1] == '*') {
				pc += 3;
				c = *pc;
				while (c != '/' || pc[-1] != '*') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else if (c == '/' && pc[1] == '/') {
				pc += 2;
				c = *pc;
				while (c != '\n') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else {
				switch (c) {
                    case '>': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                            case '>': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '<': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                            case '<': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '!': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '=': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '+': {
                        switch (pc[1]) {
                            case '+': {
                                pc++;
                                break;
                            }
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '-': {
                        switch (pc[1]) {
                            case '-': {
                                pc++;
                                break;
                            }
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '*': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
                    case '/': {
                        switch (pc[1]) {
                            case '=': {
                                pc++;
                                break;
                            }
                        }
                        break;
                    }
				}
				size++;
			}
			c = *(++pc);
		}
	}
	if (tokenSize) {
		*tokenSize = size;
	}
	struct lng_token_t* tokens = (struct lng_token_t*) malloc((size + 1) * sizeof(struct lng_token_t));
	if (!tokens) {
		return 0;
	}
	tokens[size].type = LNG_LEXICAL_EOF;
	struct lng_token_t* token = tokens;
	unsigned int lin = 1;
	unsigned int col = 1;
	unsigned char* pc = (unsigned char*) text;
	unsigned char c = *pc;
	while (c) {
		if (c <= ' ') {
			if (c == '\n') {
				lin++;
				col = 1;
			} else {
				col++;
			}
		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			unsigned short type = LNG_LEXICAL_ID;
			long begin = pc - text;
			do {
				c = *(++pc);
			} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
			uint32 size = (uint32) ((pc - text) - begin);
			char* word = (char*) text + begin;
			switch (word[0]) {
                case 'a':
                    switch (size) {
                        case 3:
                            if (word[1] =='n' && word[2] == 'd') {
                                type = LNG_LEXICAL_AND_WORD;
                            }
                            break;
                    }
                    break;
                case 'b':
                    switch (size) {
                        case 7:
                            if (word[1] =='o' && word[2] == 'o' && word[3] == 'l' && word[4] == 'e' && word[5] == 'a' && word[6] == 'n') {
                                type = LNG_LEXICAL_BOOLEAN_WORD;
                            }
                            break;
                    }
                    break;
                case 'c':
                    switch (size) {
                        case 5:
                            if (word[1] =='l' && word[2] == 'a' && word[3] == 's' && word[4] == 's') {
                                type = LNG_LEXICAL_CLASS_WORD;
                            }
                            break;
                    }
                    break;
                case 'd':
                    switch (size) {
                        case 2:
                            if (word[1] =='o') {
                                type = LNG_LEXICAL_DO_WORD;
                            }
                            break;
                        case 6:
                            if (word[1] =='o' && word[2] == 'u' && word[3] == 'b' && word[4] == 'l' && word[5] == 'e') {
                                type = LNG_LEXICAL_DOUBLE_WORD;
                            }
                            break;
                    }
                    break;
                case 'e':
                    switch (size) {
                        case 3:
                            if (word[1] =='n' && word[2] =='d') {
                                type = LNG_LEXICAL_END_WORD;
                            }
                            break;
                        case 4:
                            if (word[1] =='l' && word[2] =='s' && word[3] =='e') {
                                type = LNG_LEXICAL_ELSE_WORD;
                            }
                            break;
                        case 7:
                            if (word[1] =='x' && word[2] == 't' && word[3] == 'e' && word[4] == 'n' && word[5] == 'd' && word[6] == 's') {
                                type = LNG_LEXICAL_EXTENDS_WORD;
                            }
                            break;
                    }
                    break;
                case 'f':
                    switch (size) {
                        case 3:
                            if (word[1] =='o' && word[2] =='r') {
                                type = LNG_LEXICAL_FOR_WORD;
                            }
                            break;
                        case 5:
                            if (word[1] =='l' && word[2] == 'o' && word[3] == 'a' && word[4] == 't') {
                                type = LNG_LEXICAL_FLOAT_WORD;
                            } else if (word[1] =='a' && word[2] == 'l' && word[3] == 's' && word[4] == 'e') {
                                type = LNG_LEXICAL_FALSE_WORD;
                            }
                            break;
                    }
                    break;
                case 'i':
                    switch (size) {
                        case 2:
                            if (word[1] =='f') {
                                type = LNG_LEXICAL_IF_WORD;
                            }
                            break;
                        case 3:
                            if (word[1] =='n' && word[2] =='t') {
                                type = LNG_LEXICAL_INT_WORD;
                            }
                            break;
                        case 6:
                            if (word[1] =='m' && word[2] == 'p' && word[3] == 'o' && word[4] == 'r' && word[5] == 't') {
                                type = LNG_LEXICAL_IMPORT_WORD;
                            }
                            break;
                        case 9:
                            if (word[1] =='n' && word[2] == 't' && word[3] == 'e' && word[4] == 'r' && word[5] == 'f' && word[6] == 'a' && word[7] == 'c' && word[8] == 'e') {
                                type = LNG_LEXICAL_INTERFACE_WORD;
                            }
                            break;
                        case 10:
                            if (word[1] =='m' && word[2] == 'p' && word[3] == 'l' && word[4] == 'e' && word[5] == 'm' && word[6] == 'e' && word[7] == 'n' && word[8] == 't' && word[9] == 's') {
                                type = LNG_LEXICAL_IMPLEMENTS_WORD;
                            } else if (word[1] =='n' && word[2] == 's' && word[3] == 't' && word[4] == 'a' && word[5] == 'n' && word[6] == 'c' && word[7] == 'e' && word[8] == 'o' && word[9] == 'f') {
                                type = LNG_LEXICAL_INSTANCEOF_WORD;
                            }
                            break;
                    }
                    break;
                case 'l':
                    switch (size) {
                        case 4:
                            if (word[1] =='o' && word[2] == 'n' && word[3] == 'g') {
                                type = LNG_LEXICAL_LONG_WORD;
                            }
                            break;
                    }
                    break;
                case 'o':
                    switch (size) {
                        case 2:
                            if (word[1] =='r') {
                                type = LNG_LEXICAL_OR_WORD;
                            }
                            break;
                    }
                    break;
                case 'p':
                    switch (size) {
                        case 6:
                            if (word[1] =='u' && word[2] == 'b' && word[3] == 'l' && word[4] == 'i' && word[5] == 'c') {
                                type = LNG_LEXICAL_PUBLIC_WORD;
                            }
                            break;
                        case 7:
                            if (word[1] =='a' && word[2] == 'c' && word[3] == 'k' && word[4] == 'a' && word[5] == 'g' && word[6] == 'e') {
                                type = LNG_LEXICAL_PACKAGE_WORD;
                            } else if (word[1] =='r' && word[2] == 'i' && word[3] == 'v' && word[4] == 'a' && word[5] == 't' && word[6] == 'e') {
                                type = LNG_LEXICAL_PRIVATE_WORD;
                            }
                            break;
                        case 9:
                            if (word[1] =='r' && word[2] == 'o' && word[3] == 't' && word[4] == 'e' && word[5] == 'c' && word[6] == 't' && word[7] == 'e' && word[8] == 'd') {
                                type = LNG_LEXICAL_PROTECTED_WORD;
                            }
                            break;
                    }
                    break;
                case 'r':
                    switch (size) {
                        case 6:
                            if (word[1] =='e' && word[2] == 'p' && word[3] == 'e' && word[4] == 'a' && word[5] == 't') {
                                type = LNG_LEXICAL_REPEAT_WORD;
                            } else if (word[1] =='e' && word[2] == 't' && word[3] == 'u' && word[4] == 'r' && word[5] == 'n') {
                                type = LNG_LEXICAL_RETURN_WORD;
                            }
                            break;
                    }
                    break;
                case 's':
                    switch (size) {
                        case 6:
                            if (word[1] =='t' && word[2] == 'r' && word[3] == 'i' && word[4] == 'n' && word[5] == 'g') {
                                type = LNG_LEXICAL_STRING_WORD;
                            } else if (word[1] =='t' && word[2] == 'a' && word[3] == 't' && word[4] == 'i' && word[5] == 'c') {
                                type = LNG_LEXICAL_STATIC_WORD;
                            }
                            break;
                        case 11:
                            if (word[1] =='y' && word[2] == 'n' && word[3] == 'c' && word[4] == 'r' && word[5] == 'o' && word[6] == 'n' && word[7] == 'i' && word[8] == 'z' && word[9] == 'e' && word[10] == 'd') {
                                type = LNG_LEXICAL_SYNCRONIZED_WORD;
                            }
                            break;
                    }
                    break;
                case 't':
                    switch (size) {
                        case 4:
                            if (word[1] =='r' && word[2] == 'u' && word[3] == 'e') {
                                type = LNG_LEXICAL_TRUE_WORD;
                            }
                            break;
                        case 5:
                            if (word[1] =='h' && word[2] == 'r' && word[3] == 'o' && word[4] == 'w') {
                                type = LNG_LEXICAL_THROW_WORD;
                            }
                            break;
                        case 6:
                            if (word[1] =='h' && word[2] == 'r' && word[3] == 'o' && word[4] == 'w' && word[5] == 's') {
                                type = LNG_LEXICAL_THROWS_WORD;
                            }
                            break;
                    }
                    break;
                case 'v':
                    switch (size) {
                        case 4:
                            if (word[1] =='o' && word[2] == 'i' && word[3] == 'd') {
                                type = LNG_LEXICAL_VOID_WORD;
                            }
                            break;
                    }
                    break;
                case 'w':
                    switch (size) {
                        case 5:
                            if (word[1] =='h' && word[2] == 'i' && word[3] == 'l' && word[4] == 'e') {
                                type = LNG_LEXICAL_WHILE_WORD;
                            }
                            break;
                    }
                    break;
			}
			token->begin = begin;
			token->size = size;
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = (unsigned char*) text + begin;
			token++;
			col += token->size;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			unsigned short type = LNG_LEXICAL_HEX;
			long begin = pc - text;
			pc++;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin);
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = (unsigned char*) text + begin;
			token++;
			col += token->size;
			pc--;
		} else if ((c >= '0' && c <= '9')) {
			unsigned short type = LNG_LEXICAL_NUMBER;
			long begin = pc - text;
			c = *(++pc);
			col++;
			while (c >= '0' && c <= '9') {
				c = *(++pc);
				col++;
			}
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin);
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = (unsigned char*) text + begin;
			token++;
			col += token->size;
			pc--;
		} else if (c == '\"' || c == '\'') {
			int clin = lin;
			int ccol = col;
			unsigned short type = LNG_LEXICAL_STRING;
			long begin = pc - text;
			c = *(++pc);
			col++;
			while ((c != '\"' && c != '\'') || (pc != text && pc[-1] == '\\')) {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin + 1);
			token->type = type;
			token->line = clin;
			token->column = ccol;
			token->word = (unsigned char*) text + begin;
			token++;
		} else if (c == '/' && pc[1] == '*') {
			pc += 3;
			col += 3;
			c = *pc;
			while (c != '/' || pc[-1] != '*') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
		} else if (c == '/' && pc[1] == '/') {
			pc += 2;
			c = *pc;
			while (c != '\n') {
				if (c <= 0x7F) {
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
			}
			lin++;
			col = 1;
		} else {
			switch (c) {
                case '>': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_GREATER_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        case '>': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_GREATER_GREATER;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '<': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_LOWER_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        case '<': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_LOWER_LOWER;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '!': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_DIFERENT;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '=': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_EQUAL_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '+': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_SUM_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        case '+': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_SUM_SUM;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '-': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_SUB_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        case '-': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_SUB_SUB;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '*': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_MUL_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                case '/': {
                    switch (pc[1]) {
                        case '=': {
                            token->begin = ++pc - text + 1;
                            token->size = 2;
                            token->type = LNG_LEXICAL_DIV_EQUAL;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col += 2;
                            break;
                        }
                        default: {
                            token->begin = pc - text;
                            token->size = 1;
                            token->type = c;
                            token->line = lin;
                            token->column = col;
                            token++;
                            col++;
                        }
                    }
                    break;
                }
                default: {
                    token->begin = pc - text;
                    token->size = 1;
                    token->type = c;
                    token->line = lin;
                    token->column = col;
                    token++;
                    col++;
                }
			}
		}
		c = *(++pc);
	}
	return tokens;
}
