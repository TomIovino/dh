#define F_IF 257
#define F_ELSE 258
#define F_CASE 259
#define F_DEFAULT 260
#define F_WHILE 261
#define F_DO 262
#define F_FOR 263
#define F_CONTINUE 264
#define F_INT 265
#define F_STATUS 266
#define F_STRING_DECL 267
#define F_FLOAT_DECL 268
#define F_OBJECT 269
#define F_CLOSURE_DECL 270
#define F_SYMBOL_DECL 271
#define F_VOID 272
#define F_MIXED 273
#define F_STATIC 274
#define F_PRIVATE 275
#define F_NO_MASK 276
#define F_PROTECTED 277
#define F_PUBLIC 278
#define F_VARARGS 279
#define F_VIRTUAL 280
#define F_INHERIT 281
#define F_COLON_COLON 282
#define F_ARROW 283
#define F_QUOTED_AGGREGATE 284
#define F_ILLEGAL 285
#define F_DUMMY 286
#define F_ESCAPE 287
#define F_TEFUN 288
#define F_VEFUN 289
#define F_IDENTIFIER 290
#define F_RETURN 291
#define F_STRING 292
#define F_NUMBER 293
#define F_FLOAT 294
#define F_MAPPING 295
#define F_CLOSURE 296
#define F_SYMBOL 297
#define F_INC 298
#define F_DEC 299
#define F_LAND 300
#define F_LOR 301
#define F_ASSIGN 302
#define F_ADD 303
#define F_SUBTRACT 304
#define F_MULTIPLY 305
#define F_DIVIDE 306
#define F_LT 307
#define F_GT 308
#define F_EQ 309
#define F_GE 310
#define F_LE 311
#define F_NE 312
#define F_BREAK 313
#define F_SWITCH 314
#define F_SSCANF 315
#define F_LOCAL 316
#define F_MOD 317
#define F_COMPL 318
#define F_AND 319
#define F_OR 320
#define F_XOR 321
#define F_LSH 322
#define F_RSH 323
#define F_CATCH 324
#define F_NOT 325
#define F_RANGE 326
#define F_POP_VALUE 327
#define F_DUP 328
#define F_CALL_FUNCTION_BY_ADDRESS 329
#define F_CALL_EXPLICIT_INHERITED 330
#define F_PUSH_IDENTIFIER_LVALUE 331
#define F_VIRTUAL_VARIABLE 332
#define F_PUSH_VIRTUAL_VARIABLE_LVALUE 333
#define F_IDENTIFIER16 334
#define F_PUSH_IDENTIFIER16_LVALUE 335
#define F_PUSH_LOCAL_VARIABLE_LVALUE 336
#define F_PUSH_INDEXED_LVALUE 337
#define F_PUSH_RINDEXED_LVALUE 338
#define F_INDEX_LVALUE 339
#define F_RINDEX_LVALUE 340
#define F_INDEX 341
#define F_RINDEX 342
#define F_CONST0 343
#define F_CONST1 344
#define F_LBRANCH 345
#define F_LBRANCH_WHEN_ZERO 346
#define F_LBRANCH_WHEN_NON_ZERO 347
#define F_BRANCH 348
#define F_BRANCH_WHEN_ZERO 349
#define F_BRANCH_WHEN_NON_ZERO 350
#define F_BBRANCH_WHEN_ZERO 351
#define F_BBRANCH_WHEN_NON_ZERO 352
#define F_SIMUL_EFUN 353
#define F_RETURN0 354
#define F_PRE_INC 355
#define F_PRE_DEC 356
#define F_POST_INC 357
#define F_POST_DEC 358
#define F_CSTRING0 359
#define F_CSTRING1 360
#define F_CSTRING2 361
#define F_CSTRING3 362
#define F_CLIT 363
#define F_NCLIT 364
#define F_VOID_ASSIGN 365
#define F_VOID_ADD_EQ 366
#define F_ADD_EQ 367
#define F_SUB_EQ 368
#define F_DIV_EQ 369
#define F_MULT_EQ 370
#define F_MOD_EQ 371
#define F_AND_EQ 372
#define F_OR_EQ 373
#define F_XOR_EQ 374
#define F_LSH_EQ 375
#define F_RSH_EQ 376
#define F_RANGE_LVALUE 377
#define F_AGGREGATE 378
#define F_EXTRACT2 379
#define F_PREVIOUS_OBJECT0 380
#define F_LAMBDA_CCONSTANT 381
#define F_LAMBDA_CONSTANT 382
#define F_M_AGGREGATE 383
#define F_M_CAGGREGATE 384
#define F_MAP_INDEX 385
#define F_PUSH_INDEXED_MAP_LVALUE 386
#define F_JUMP 387
typedef union
{
#line 1288 "prolang.y"
	p_int number;
	p_uint address;	/* Address of an instruction */
	char *string;
	char *shared_string;
	short type;
	struct { p_int key; int numeric; } case_label;
	struct { int simul_efun; short start; } function_call_head;
	struct svalue svalue;



	struct svalue *initialized;
	struct {
	    int function;
	    struct svalue *initialized;
	} const_call_head;
	p_int numbers[2];
	struct {
	    p_int length;
	    struct const_list * l;
	} const_list;
	struct {
	    char *p;
	    unsigned short length;
	    unsigned short line;
	} expression;
	struct {
	    union {
		char *p, simple[2];
	    } u;
	    unsigned short length;
	    short type;
	} lvalue;
	struct {
	    char *super;
	    struct ident *real;
	} function_name;
	struct ident *ident;
	double float_number;
	struct {
	    p_int number;



	} closure;
	struct {
	    char *name;
	    int quotes;
	} symbol;
	struct s_lrvalue lrvalue;
} YYSTYPE;
extern YYSTYPE yylval;
