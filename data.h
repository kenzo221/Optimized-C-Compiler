#ifndef extern_
#define extern_ extern
#endif

//Global data

//Global variables

extern_ int Line;		     			//Current line number
extern_ int Putback;		     			//Character put back by scanner
extern_ struct symtable *Functionid; 			//Current function symbol pointer
extern_ FILE *Infile;		     			//Input file
extern_ FILE *Outfile;					//Output file
extern_ char *Infilename;				//Input file name for parsing
extern_ char *Outfilename;				//Output file name assembly
extern_ char *outsfilename;				//Output file name object
extern_ struct token Token;				//Last token scanned
extern_ char Text[TEXTLEN + 1];			//Last identifier scanned
extern_ int Looplevel;					//Level of depth of nested loops
extern_ int Switchlevel;				//Level of depth of nested switches

//Symbol table lists
extern_ struct symtable *Globhead, *Globtail;		//Global variables and functions
extern_ struct symtable *Loclhead, *Locltail;	  	//Local variables
extern_ struct symtable *Parmhead, *Parmtail;	  	//Local parameters
extern_ struct symtable *Membhead, *Membtail;	  	//Temp list of struct/union members
extern_ struct symtable *Enumhead,  *Enumtail;    	//List of enum types and values
extern_ struct symtable *Typehead,  *Typetail;    	//List of typedefs
extern_ struct symtable *Structhead, *Structtail; 	//List of struct types
extern_ struct symtable *Unionhead, *Uniontail;  	//List of union types


//Command-line flags
extern_ int O_verbose;					//Print information on compilation stages
extern_ int O_dumpAST;					//Display current AST tree
extern_ int O_keepasm;					//Keep assembly files
extern_ int O_assemble;				//Assemble assembly files
extern_ int O_dolink;					//Link the object files
