/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     FCONST = 259,
     SCONST = 260,
     BCONST = 261,
     XCONST = 262,
     Op = 263,
     ICONST = 264,
     PARAM = 265,
     TYPECAST = 266,
     DOT_DOT = 267,
     COLON_EQUALS = 268,
     EQUALS_GREATER = 269,
     LESS_EQUALS = 270,
     GREATER_EQUALS = 271,
     NOT_EQUALS = 272,
     ABORT_P = 273,
     ABSOLUTE_P = 274,
     ACCESS = 275,
     ACTION = 276,
     ADD_P = 277,
     ADMIN = 278,
     AFTER = 279,
     AGGREGATE = 280,
     ALL = 281,
     ALSO = 282,
     ALTER = 283,
     ALWAYS = 284,
     ANALYSE = 285,
     ANALYZE = 286,
     AND = 287,
     ANY = 288,
     ARRAY = 289,
     AS = 290,
     ASC_P = 291,
     ASSERTION = 292,
     ASSIGNMENT = 293,
     ASYMMETRIC = 294,
     AT = 295,
     ATTACH = 296,
     ATTRIBUTE = 297,
     AUTHORIZATION = 298,
     BACKWARD = 299,
     BEFORE = 300,
     BEGIN_P = 301,
     BETWEEN = 302,
     BIGINT = 303,
     BINARY = 304,
     BIT = 305,
     BOOLEAN_P = 306,
     BOTH = 307,
     BY = 308,
     CACHE = 309,
     CALLED = 310,
     CASCADE = 311,
     CASCADED = 312,
     CASE = 313,
     CAST = 314,
     CATALOG_P = 315,
     CHAIN = 316,
     CHAR_P = 317,
     CHARACTER = 318,
     CHARACTERISTICS = 319,
     CHECK_P = 320,
     CHECKPOINT = 321,
     CLASS = 322,
     CLOSE = 323,
     CLUSTER = 324,
     COALESCE = 325,
     COLLATE = 326,
     COLLATION = 327,
     COLUMN = 328,
     COLUMNS = 329,
     COMMENT = 330,
     COMMENTS = 331,
     COMMIT = 332,
     COMMITTED = 333,
     CONCURRENTLY = 334,
     CONFIGURATION = 335,
     CONFLICT = 336,
     CONNECTION = 337,
     CONSTRAINT = 338,
     CONSTRAINTS = 339,
     CONTENT_P = 340,
     CONTINUE_P = 341,
     CONVERSION_P = 342,
     COPY = 343,
     COST = 344,
     CREATE_P = 345,
     CROSS = 346,
     CSV = 347,
     CUBE = 348,
     CURRENT_P = 349,
     CURRENT_CATALOG = 350,
     CURRENT_DATE = 351,
     CURRENT_ROLE = 352,
     CURRENT_SCHEMA = 353,
     CURRENT_TIME = 354,
     CURRENT_TIMESTAMP = 355,
     CURRENT_USER = 356,
     CURSOR = 357,
     CYCLE = 358,
     DATA_P = 359,
     DATABASE = 360,
     DAY_P = 361,
     DEALLOCATE = 362,
     DEC = 363,
     DECIMAL_P = 364,
     DECLARE = 365,
     DEFAULT = 366,
     DEFAULTS = 367,
     DEFERRABLE = 368,
     DEFERRED = 369,
     DEFINER = 370,
     DELETE_P = 371,
     DELIMITER = 372,
     DELIMITERS = 373,
     DEPENDS = 374,
     DESC_P = 375,
     DESCRIBE = 376,
     DETACH = 377,
     DICTIONARY = 378,
     DIRECTED = 379,
     DISABLE_P = 380,
     DISCARD = 381,
     DISTINCT = 382,
     DO = 383,
     DOCUMENT_P = 384,
     DOMAIN_P = 385,
     DOUBLE_P = 386,
     DROP = 387,
     EACH = 388,
     ELSE = 389,
     ENABLE_P = 390,
     ENCODING = 391,
     ENCRYPTED = 392,
     END_P = 393,
     ENUM_P = 394,
     ESCAPE = 395,
     EVENT = 396,
     EXCEPT = 397,
     EXCLUDE = 398,
     EXCLUDING = 399,
     EXCLUSIVE = 400,
     EXECUTE = 401,
     EXISTS = 402,
     EXPLAIN = 403,
     EXTENSION = 404,
     EXTERNAL = 405,
     EXTRACT = 406,
     FALSE_P = 407,
     FAMILY = 408,
     FETCH = 409,
     FILTER = 410,
     FIRST_P = 411,
     FLOAT_P = 412,
     FOLLOWING = 413,
     FOR = 414,
     FORCE = 415,
     FOREIGN = 416,
     FORWARD = 417,
     FREEZE = 418,
     FROM = 419,
     FULL = 420,
     FUNCTION = 421,
     FUNCTIONS = 422,
     GENERATED = 423,
     GLOBAL = 424,
     GRANT = 425,
     GRANTED = 426,
     GREATEST = 427,
     GROUP_P = 428,
     GROUPING = 429,
     HANDLER = 430,
     HAVING = 431,
     HEADER_P = 432,
     HOLD = 433,
     HOUR_P = 434,
     IDENTITY_P = 435,
     IF_P = 436,
     ILIKE = 437,
     IMMEDIATE = 438,
     IMMUTABLE = 439,
     IMPLICIT_P = 440,
     IMPORT_P = 441,
     IN_P = 442,
     INCLUDING = 443,
     INCREMENT = 444,
     INDEX = 445,
     INDEXES = 446,
     INHERIT = 447,
     INHERITS = 448,
     INITIALLY = 449,
     INLINE_P = 450,
     INNER_P = 451,
     INOUT = 452,
     INPUT_P = 453,
     INSENSITIVE = 454,
     INSERT = 455,
     INSTEAD = 456,
     INT_P = 457,
     INTEGER = 458,
     INTERSECT = 459,
     INTERVAL = 460,
     INTO = 461,
     INVOKER = 462,
     IS = 463,
     ISNULL = 464,
     ISOLATION = 465,
     JOIN = 466,
     KEY = 467,
     LABEL = 468,
     LANGUAGE = 469,
     LARGE_P = 470,
     LAST_P = 471,
     LATERAL_P = 472,
     LEADING = 473,
     LEAKPROOF = 474,
     LEAST = 475,
     LEFT = 476,
     LEVEL = 477,
     LIKE = 478,
     LIMIT = 479,
     LISTEN = 480,
     LOAD = 481,
     LOCAL = 482,
     LOCALTIME = 483,
     LOCALTIMESTAMP = 484,
     LOCATION = 485,
     LOCK_P = 486,
     LOCKED = 487,
     LOGGED = 488,
     MAPPING = 489,
     MATCH = 490,
     MATERIALIZED = 491,
     MAXVALUE = 492,
     METHOD = 493,
     MINUTE_P = 494,
     MINVALUE = 495,
     MODE = 496,
     MONTH_P = 497,
     MOVE = 498,
     NAME_P = 499,
     NAMES = 500,
     NATIONAL = 501,
     NATURAL = 502,
     NCHAR = 503,
     NEW = 504,
     NEXT = 505,
     NO = 506,
     NONE = 507,
     NOT = 508,
     NOTHING = 509,
     NOTIFY = 510,
     NOTNULL = 511,
     NOWAIT = 512,
     NULL_P = 513,
     NULLIF = 514,
     NULLS_P = 515,
     NUMERIC = 516,
     OBJECT_P = 517,
     OF = 518,
     OFF = 519,
     OFFSET = 520,
     OIDS = 521,
     OLD = 522,
     ON = 523,
     ONLY = 524,
     OPERATOR = 525,
     OPTION = 526,
     OPTIONS = 527,
     OR = 528,
     ORDER = 529,
     ORDINALITY = 530,
     OUT_P = 531,
     OUTER_P = 532,
     OVER = 533,
     OVERLAPS = 534,
     OVERLAY = 535,
     OVERRIDING = 536,
     OWNED = 537,
     OWNER = 538,
     PARALLEL = 539,
     PARSER = 540,
     PARTIAL = 541,
     PARTITION = 542,
     PASSING = 543,
     PASSWORD = 544,
     PKFK = 545,
     PLACING = 546,
     PLANS = 547,
     POLICY = 548,
     POSITION = 549,
     PRAGMA_P = 550,
     PRECEDING = 551,
     PRECISION = 552,
     PREPARE = 553,
     PREPARED = 554,
     PRESERVE = 555,
     PRIMARY = 556,
     PRIOR = 557,
     PRIVILEGES = 558,
     PROCEDURAL = 559,
     PROCEDURE = 560,
     PROGRAM = 561,
     PUBLICATION = 562,
     QUOTE = 563,
     RAI = 564,
     RANGE = 565,
     READ_P = 566,
     REAL = 567,
     REASSIGN = 568,
     RECHECK = 569,
     RECURSIVE = 570,
     REF = 571,
     REFERENCES = 572,
     REFERENCING = 573,
     REFRESH = 574,
     REINDEX = 575,
     RELATIVE_P = 576,
     RELEASE = 577,
     RENAME = 578,
     REPEATABLE = 579,
     REPLACE = 580,
     REPLICA = 581,
     RESET = 582,
     RESTART = 583,
     RESTRICT = 584,
     RETURNING = 585,
     RETURNS = 586,
     REVOKE = 587,
     RIGHT = 588,
     ROLE = 589,
     ROLLBACK = 590,
     ROLLUP = 591,
     ROW = 592,
     ROWS = 593,
     RULE = 594,
     SAVEPOINT = 595,
     SCHEMA = 596,
     SCHEMAS = 597,
     SCROLL = 598,
     SEARCH = 599,
     SECOND_P = 600,
     SECURITY = 601,
     SELECT = 602,
     SELF = 603,
     SEQUENCE = 604,
     SEQUENCES = 605,
     SERIALIZABLE = 606,
     SERVER = 607,
     SESSION = 608,
     SESSION_USER = 609,
     SET = 610,
     SETOF = 611,
     SETS = 612,
     SHARE = 613,
     SHOW = 614,
     SIMILAR = 615,
     SIMPLE = 616,
     SKIP = 617,
     SMALLINT = 618,
     SNAPSHOT = 619,
     SOME = 620,
     SQL_P = 621,
     STABLE = 622,
     STANDALONE_P = 623,
     START = 624,
     STATEMENT = 625,
     STATISTICS = 626,
     STDIN = 627,
     STDOUT = 628,
     STORAGE = 629,
     STRICT_P = 630,
     STRIP_P = 631,
     SUBSCRIPTION = 632,
     SUBSTRING = 633,
     SYMMETRIC = 634,
     SYSID = 635,
     SYSTEM_P = 636,
     TABLE = 637,
     TABLES = 638,
     TABLESAMPLE = 639,
     TABLESPACE = 640,
     TEMP = 641,
     TEMPLATE = 642,
     TEMPORARY = 643,
     TEXT_P = 644,
     THEN = 645,
     TIME = 646,
     TIMESTAMP = 647,
     TO = 648,
     TRAILING = 649,
     TRANSACTION = 650,
     TRANSFORM = 651,
     TREAT = 652,
     TRIGGER = 653,
     TRIM = 654,
     TRUE_P = 655,
     TRUNCATE = 656,
     TRUSTED = 657,
     TYPE_P = 658,
     TYPES_P = 659,
     UNBOUNDED = 660,
     UNCOMMITTED = 661,
     UNDIRECTED = 662,
     UNENCRYPTED = 663,
     UNION = 664,
     UNIQUE = 665,
     UNKNOWN = 666,
     UNLISTEN = 667,
     UNLOGGED = 668,
     UNTIL = 669,
     UPDATE = 670,
     USER = 671,
     USING = 672,
     VACUUM = 673,
     VALID = 674,
     VALIDATE = 675,
     VALIDATOR = 676,
     VALUE_P = 677,
     VALUES = 678,
     VARCHAR = 679,
     VARIADIC = 680,
     VARYING = 681,
     VERBOSE = 682,
     VERSION_P = 683,
     VIEW = 684,
     VIEWS = 685,
     VOLATILE = 686,
     WHEN = 687,
     WHERE = 688,
     WHITESPACE_P = 689,
     WINDOW = 690,
     WITH = 691,
     WITHIN = 692,
     WITHOUT = 693,
     WORK = 694,
     WRAPPER = 695,
     WRITE_P = 696,
     XML_P = 697,
     XMLATTRIBUTES = 698,
     XMLCONCAT = 699,
     XMLELEMENT = 700,
     XMLEXISTS = 701,
     XMLFOREST = 702,
     XMLNAMESPACES = 703,
     XMLPARSE = 704,
     XMLPI = 705,
     XMLROOT = 706,
     XMLSERIALIZE = 707,
     XMLTABLE = 708,
     YEAR_P = 709,
     YES_P = 710,
     ZONE = 711,
     NOT_LA = 712,
     NULLS_LA = 713,
     WITH_LA = 714,
     POSTFIXOP = 715,
     UMINUS = 716
   };
#endif
/* Tokens.  */
#define IDENT 258
#define FCONST 259
#define SCONST 260
#define BCONST 261
#define XCONST 262
#define Op 263
#define ICONST 264
#define PARAM 265
#define TYPECAST 266
#define DOT_DOT 267
#define COLON_EQUALS 268
#define EQUALS_GREATER 269
#define LESS_EQUALS 270
#define GREATER_EQUALS 271
#define NOT_EQUALS 272
#define ABORT_P 273
#define ABSOLUTE_P 274
#define ACCESS 275
#define ACTION 276
#define ADD_P 277
#define ADMIN 278
#define AFTER 279
#define AGGREGATE 280
#define ALL 281
#define ALSO 282
#define ALTER 283
#define ALWAYS 284
#define ANALYSE 285
#define ANALYZE 286
#define AND 287
#define ANY 288
#define ARRAY 289
#define AS 290
#define ASC_P 291
#define ASSERTION 292
#define ASSIGNMENT 293
#define ASYMMETRIC 294
#define AT 295
#define ATTACH 296
#define ATTRIBUTE 297
#define AUTHORIZATION 298
#define BACKWARD 299
#define BEFORE 300
#define BEGIN_P 301
#define BETWEEN 302
#define BIGINT 303
#define BINARY 304
#define BIT 305
#define BOOLEAN_P 306
#define BOTH 307
#define BY 308
#define CACHE 309
#define CALLED 310
#define CASCADE 311
#define CASCADED 312
#define CASE 313
#define CAST 314
#define CATALOG_P 315
#define CHAIN 316
#define CHAR_P 317
#define CHARACTER 318
#define CHARACTERISTICS 319
#define CHECK_P 320
#define CHECKPOINT 321
#define CLASS 322
#define CLOSE 323
#define CLUSTER 324
#define COALESCE 325
#define COLLATE 326
#define COLLATION 327
#define COLUMN 328
#define COLUMNS 329
#define COMMENT 330
#define COMMENTS 331
#define COMMIT 332
#define COMMITTED 333
#define CONCURRENTLY 334
#define CONFIGURATION 335
#define CONFLICT 336
#define CONNECTION 337
#define CONSTRAINT 338
#define CONSTRAINTS 339
#define CONTENT_P 340
#define CONTINUE_P 341
#define CONVERSION_P 342
#define COPY 343
#define COST 344
#define CREATE_P 345
#define CROSS 346
#define CSV 347
#define CUBE 348
#define CURRENT_P 349
#define CURRENT_CATALOG 350
#define CURRENT_DATE 351
#define CURRENT_ROLE 352
#define CURRENT_SCHEMA 353
#define CURRENT_TIME 354
#define CURRENT_TIMESTAMP 355
#define CURRENT_USER 356
#define CURSOR 357
#define CYCLE 358
#define DATA_P 359
#define DATABASE 360
#define DAY_P 361
#define DEALLOCATE 362
#define DEC 363
#define DECIMAL_P 364
#define DECLARE 365
#define DEFAULT 366
#define DEFAULTS 367
#define DEFERRABLE 368
#define DEFERRED 369
#define DEFINER 370
#define DELETE_P 371
#define DELIMITER 372
#define DELIMITERS 373
#define DEPENDS 374
#define DESC_P 375
#define DESCRIBE 376
#define DETACH 377
#define DICTIONARY 378
#define DIRECTED 379
#define DISABLE_P 380
#define DISCARD 381
#define DISTINCT 382
#define DO 383
#define DOCUMENT_P 384
#define DOMAIN_P 385
#define DOUBLE_P 386
#define DROP 387
#define EACH 388
#define ELSE 389
#define ENABLE_P 390
#define ENCODING 391
#define ENCRYPTED 392
#define END_P 393
#define ENUM_P 394
#define ESCAPE 395
#define EVENT 396
#define EXCEPT 397
#define EXCLUDE 398
#define EXCLUDING 399
#define EXCLUSIVE 400
#define EXECUTE 401
#define EXISTS 402
#define EXPLAIN 403
#define EXTENSION 404
#define EXTERNAL 405
#define EXTRACT 406
#define FALSE_P 407
#define FAMILY 408
#define FETCH 409
#define FILTER 410
#define FIRST_P 411
#define FLOAT_P 412
#define FOLLOWING 413
#define FOR 414
#define FORCE 415
#define FOREIGN 416
#define FORWARD 417
#define FREEZE 418
#define FROM 419
#define FULL 420
#define FUNCTION 421
#define FUNCTIONS 422
#define GENERATED 423
#define GLOBAL 424
#define GRANT 425
#define GRANTED 426
#define GREATEST 427
#define GROUP_P 428
#define GROUPING 429
#define HANDLER 430
#define HAVING 431
#define HEADER_P 432
#define HOLD 433
#define HOUR_P 434
#define IDENTITY_P 435
#define IF_P 436
#define ILIKE 437
#define IMMEDIATE 438
#define IMMUTABLE 439
#define IMPLICIT_P 440
#define IMPORT_P 441
#define IN_P 442
#define INCLUDING 443
#define INCREMENT 444
#define INDEX 445
#define INDEXES 446
#define INHERIT 447
#define INHERITS 448
#define INITIALLY 449
#define INLINE_P 450
#define INNER_P 451
#define INOUT 452
#define INPUT_P 453
#define INSENSITIVE 454
#define INSERT 455
#define INSTEAD 456
#define INT_P 457
#define INTEGER 458
#define INTERSECT 459
#define INTERVAL 460
#define INTO 461
#define INVOKER 462
#define IS 463
#define ISNULL 464
#define ISOLATION 465
#define JOIN 466
#define KEY 467
#define LABEL 468
#define LANGUAGE 469
#define LARGE_P 470
#define LAST_P 471
#define LATERAL_P 472
#define LEADING 473
#define LEAKPROOF 474
#define LEAST 475
#define LEFT 476
#define LEVEL 477
#define LIKE 478
#define LIMIT 479
#define LISTEN 480
#define LOAD 481
#define LOCAL 482
#define LOCALTIME 483
#define LOCALTIMESTAMP 484
#define LOCATION 485
#define LOCK_P 486
#define LOCKED 487
#define LOGGED 488
#define MAPPING 489
#define MATCH 490
#define MATERIALIZED 491
#define MAXVALUE 492
#define METHOD 493
#define MINUTE_P 494
#define MINVALUE 495
#define MODE 496
#define MONTH_P 497
#define MOVE 498
#define NAME_P 499
#define NAMES 500
#define NATIONAL 501
#define NATURAL 502
#define NCHAR 503
#define NEW 504
#define NEXT 505
#define NO 506
#define NONE 507
#define NOT 508
#define NOTHING 509
#define NOTIFY 510
#define NOTNULL 511
#define NOWAIT 512
#define NULL_P 513
#define NULLIF 514
#define NULLS_P 515
#define NUMERIC 516
#define OBJECT_P 517
#define OF 518
#define OFF 519
#define OFFSET 520
#define OIDS 521
#define OLD 522
#define ON 523
#define ONLY 524
#define OPERATOR 525
#define OPTION 526
#define OPTIONS 527
#define OR 528
#define ORDER 529
#define ORDINALITY 530
#define OUT_P 531
#define OUTER_P 532
#define OVER 533
#define OVERLAPS 534
#define OVERLAY 535
#define OVERRIDING 536
#define OWNED 537
#define OWNER 538
#define PARALLEL 539
#define PARSER 540
#define PARTIAL 541
#define PARTITION 542
#define PASSING 543
#define PASSWORD 544
#define PKFK 545
#define PLACING 546
#define PLANS 547
#define POLICY 548
#define POSITION 549
#define PRAGMA_P 550
#define PRECEDING 551
#define PRECISION 552
#define PREPARE 553
#define PREPARED 554
#define PRESERVE 555
#define PRIMARY 556
#define PRIOR 557
#define PRIVILEGES 558
#define PROCEDURAL 559
#define PROCEDURE 560
#define PROGRAM 561
#define PUBLICATION 562
#define QUOTE 563
#define RAI 564
#define RANGE 565
#define READ_P 566
#define REAL 567
#define REASSIGN 568
#define RECHECK 569
#define RECURSIVE 570
#define REF 571
#define REFERENCES 572
#define REFERENCING 573
#define REFRESH 574
#define REINDEX 575
#define RELATIVE_P 576
#define RELEASE 577
#define RENAME 578
#define REPEATABLE 579
#define REPLACE 580
#define REPLICA 581
#define RESET 582
#define RESTART 583
#define RESTRICT 584
#define RETURNING 585
#define RETURNS 586
#define REVOKE 587
#define RIGHT 588
#define ROLE 589
#define ROLLBACK 590
#define ROLLUP 591
#define ROW 592
#define ROWS 593
#define RULE 594
#define SAVEPOINT 595
#define SCHEMA 596
#define SCHEMAS 597
#define SCROLL 598
#define SEARCH 599
#define SECOND_P 600
#define SECURITY 601
#define SELECT 602
#define SELF 603
#define SEQUENCE 604
#define SEQUENCES 605
#define SERIALIZABLE 606
#define SERVER 607
#define SESSION 608
#define SESSION_USER 609
#define SET 610
#define SETOF 611
#define SETS 612
#define SHARE 613
#define SHOW 614
#define SIMILAR 615
#define SIMPLE 616
#define SKIP 617
#define SMALLINT 618
#define SNAPSHOT 619
#define SOME 620
#define SQL_P 621
#define STABLE 622
#define STANDALONE_P 623
#define START 624
#define STATEMENT 625
#define STATISTICS 626
#define STDIN 627
#define STDOUT 628
#define STORAGE 629
#define STRICT_P 630
#define STRIP_P 631
#define SUBSCRIPTION 632
#define SUBSTRING 633
#define SYMMETRIC 634
#define SYSID 635
#define SYSTEM_P 636
#define TABLE 637
#define TABLES 638
#define TABLESAMPLE 639
#define TABLESPACE 640
#define TEMP 641
#define TEMPLATE 642
#define TEMPORARY 643
#define TEXT_P 644
#define THEN 645
#define TIME 646
#define TIMESTAMP 647
#define TO 648
#define TRAILING 649
#define TRANSACTION 650
#define TRANSFORM 651
#define TREAT 652
#define TRIGGER 653
#define TRIM 654
#define TRUE_P 655
#define TRUNCATE 656
#define TRUSTED 657
#define TYPE_P 658
#define TYPES_P 659
#define UNBOUNDED 660
#define UNCOMMITTED 661
#define UNDIRECTED 662
#define UNENCRYPTED 663
#define UNION 664
#define UNIQUE 665
#define UNKNOWN 666
#define UNLISTEN 667
#define UNLOGGED 668
#define UNTIL 669
#define UPDATE 670
#define USER 671
#define USING 672
#define VACUUM 673
#define VALID 674
#define VALIDATE 675
#define VALIDATOR 676
#define VALUE_P 677
#define VALUES 678
#define VARCHAR 679
#define VARIADIC 680
#define VARYING 681
#define VERBOSE 682
#define VERSION_P 683
#define VIEW 684
#define VIEWS 685
#define VOLATILE 686
#define WHEN 687
#define WHERE 688
#define WHITESPACE_P 689
#define WINDOW 690
#define WITH 691
#define WITHIN 692
#define WITHOUT 693
#define WORK 694
#define WRAPPER 695
#define WRITE_P 696
#define XML_P 697
#define XMLATTRIBUTES 698
#define XMLCONCAT 699
#define XMLELEMENT 700
#define XMLEXISTS 701
#define XMLFOREST 702
#define XMLNAMESPACES 703
#define XMLPARSE 704
#define XMLPI 705
#define XMLROOT 706
#define XMLSERIALIZE 707
#define XMLTABLE 708
#define YEAR_P 709
#define YES_P 710
#define ZONE 711
#define NOT_LA 712
#define NULLS_LA 713
#define WITH_LA 714
#define POSTFIXOP 715
#define UMINUS 716




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 14 "third_party/libpg_query/grammar/grammar.y"
{
	core_YYSTYPE		core_yystype;
	/* these fields must match core_YYSTYPE: */
	int					ival;
	char				*str;
	const char			*keyword;
	const char          *conststr;

	char				chr;
	bool				boolean;
	PGJoinType			jtype;
	PGDropBehavior		dbehavior;
	PGOnCommitAction		oncommit;
	PGList				*list;
	PGNode				*node;
	PGValue				*value;
	PGObjectType			objtype;
	PGTypeName			*typnam;
	PGObjectWithArgs		*objwithargs;
	PGDefElem				*defelt;
	PGSortBy				*sortby;
	PGWindowDef			*windef;
	PGJoinExpr			*jexpr;
	PGIndexElem			*ielem;
	PGAlias				*alias;
	PGRangeVar			*range;
	PGIntoClause			*into;
	PGWithClause			*with;
	PGInferClause			*infer;
	PGOnConflictClause	*onconflict;
	PGAIndices			*aind;
	PGResTarget			*target;
	PGInsertStmt			*istmt;
	PGVariableSetStmt		*vsetstmt;
	PGOverridingKind       override;
	PGSortByDir            sortorder;
	PGSortByNulls          nullorder;
	PGLockClauseStrength lockstrength;
	PGLockWaitPolicy lockwaitpolicy;
	PGSubLinkType subquerytype;
	PGViewCheckOption viewcheckoption;
}
/* Line 1529 of yacc.c.  */
#line 1014 "third_party/libpg_query/src_backend_parser_gram.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


