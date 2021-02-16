#include "definitions.h"
#define extern_
#include "data.h"
#undef extern_
#include "declaration.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>

//Compiler setup and top-level execution

//Alter suffix to given character
char *alter_suffix(char *str, char suffix) {
  char *posn;
  char *newstr;

  //Clone the string
  if ((newstr = strdup(str)) == NULL)
    return (NULL);

  //Find the '.'
  if ((posn = strrchr(newstr, '.')) == NULL)
    return (NULL);

  //Ensure there is a suffix
  posn++;
  if (*posn == '\0')
    return (NULL);

  //Change the suffix and NUL-terminate the string
  *posn++ = suffix;
  *posn = '\0';
  return (newstr);
}
int success = 0;
//Given an input filename, compile the source file to assembly code. Return the assembly file name
static char *do_compile(char *filename) {
  char cmd[TEXTLEN];

  Outfilename = alter_suffix(filename, 's');
  if (Outfilename == NULL) {
    fprintf(stderr, "Error: %s has no suffix, use .c on the end\n", filename);
    exit(1);
  }
  //Generate pre-processor command
  snprintf(cmd, TEXTLEN, "%s %s %s", CPPCMD, INCDIR, filename);

  //Open the pre-processor pipe
  if ((Infile = popen(cmd, "r")) == NULL) {
    fprintf(stderr, "Unable to open %s: %s\n", filename, strerror(errno));
    exit(1);
  }
  Infilename = filename;

  if ((Outfile = fopen(Outfilename, "w")) == NULL) {
    fprintf(stderr, "Unable to create %s: %s\n", Outfilename, strerror(errno));
      success = 1;
    exit(1);
  }

  Line = 1;			//Reset the scanner
  Putback = '\n';		//Putback
  clear_symtable();		//Clear the symbol table
  scan(&Token);		//Scan for token from the input
  genpreamble();		//Generate preamble
  global_declarations();	//Parse global declarations
  genpostamble();		//Generate postamble
  fclose(Outfile);		//Close output file
  return (Outfilename);

}

//Given an input filename, assemble that file down to object code. Return the object filename
char *do_assemble(char *filename) {
  char cmd[TEXTLEN];
  int err;
  char *outfilename = alter_suffix(filename, 'o');
  if (outfilename == NULL) {
    fprintf(stderr, "Error: %s has no suffix, try .s on the end\n", filename);
    exit(1);
  }
  //Build the assembly command and run it
  snprintf(cmd, TEXTLEN, "%s %s %s", ASCMD, outfilename, filename);
  err = system(cmd);
  if (err != 0) {
    fprintf(stderr, "Assembly of %s failed\n", filename);
    exit(1);
  }
  return (outfilename);
}

//Given a list of object files and an output filename, link the object files into an executable file
void do_link(char *outfilename, char *objlist[]) {
  int cnt, size = TEXTLEN;
  char cmd[TEXTLEN], *cptr;
  int err;

  //Build the linker command and run it
  cptr = cmd;
  cnt = snprintf(cptr, size, "%s %s ", LDCMD, outfilename);
  cptr += cnt;
  size -= cnt;

  //Append each object file
  while (*objlist != NULL) {
    cnt = snprintf(cptr, size, "%s ", *objlist);
    cptr += cnt;
    size -= cnt;
    objlist++;
  }

  err = system(cmd);
  if (err != 0) {
    fprintf(stderr, "Linking failed\n");
    exit(1);
  }

}

//Print out a usage if started incorrectly
static void usage(char *prog) {
  fprintf(stderr, "OCC Usage: %s [-as] [-o outputfile] [-b outputfile] inputfile \n", prog);
  fprintf(stderr, "       -a generate assembly file only\n");
  fprintf(stderr, "       -s generate object file only\n");
  fprintf(stderr, "       -o generate executable file only\n");
  fprintf(stderr, "       -b generate all files (assembly,object,output)\n");
  exit(1);
}

//Main program: check arguments and print a usage if we don't have an argument. Open up the input file and call scanfile() to scan the tokens in it.
enum { MAXOBJ = 100 };
int main(int argc, char *argv[]) {
  clock_t start = clock();
  char *outfilename = AOUT;
  char *asmfile, *objfile;
  char *objlist[MAXOBJ];
  int i, objcnt = 0;

  //Initialise our variables
  O_keepasm = 0;
  O_assemble = 0;
  O_dolink = 1;

  //Scan for command-line options
  for (i = 1; i < argc; i++) {
    //No leading '-', stop scanning for options
    if (*argv[i] != '-')
      break;

    //For each option in this argument
    for (int j = 1; (*argv[i] == '-') && argv[i][j]; j++) {
      switch (argv[i][j]) {
        case 'a':
	  O_keepasm = 1;
	  O_assemble = 0;
	  O_dolink = 0;
	  break;
	case 's':
	  O_assemble = 1;
	  O_keepasm = 0;
	  O_dolink = 0;
	  break;
	case 'o':
	  outfilename = argv[++i];	
	  break;
	case 'b':
	  outfilename = argv[++i];	
	  O_assemble = 1;
	  O_keepasm = 1;
	  break;
	default:
	  usage(argv[0]);
      }
    }
  }

  //Ensure there is at least one input file argument
  if (i >= argc)
    usage(argv[0]);

  //Work on each input file in turn
  while (i < argc) {
    asmfile = do_compile(argv[i]);	//Compile the source file
    if (O_dolink || O_assemble) {
      objfile = do_assemble(asmfile);	//Assemble it to object format
      if (objcnt == (MAXOBJ - 2)) {
	fprintf(stderr, "Too many object files for the compiler to handle\n");
	exit(1);
      }
      objlist[objcnt++] = objfile;	//Add the object file's name to the list of object files
      objlist[objcnt] = NULL;	
    
    }

    if (!O_keepasm)		//If no assembly file need to be stored, then remove them
      unlink(asmfile);		
    i++;
  }

  //Link all the object files together
  if (O_dolink) {
    do_link(outfilename, objlist);

    //If no object file need to be stored, then remove them
    if (!O_assemble) {
      for (i = 0; objlist[i] != NULL; i++)
	unlink(objlist[i]);
    }
  }
     clock_t stop = clock();
     double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
     if(success == 0) {
     printf("Compilation is successful! \n");
    if (O_keepasm)
      printf("Assembly file is generated: %s \n", Outfilename);
    if (O_assemble){
      char *outsfilename = alter_suffix(Outfilename, 'o');
      printf("Object file is generated: %s \n", outsfilename);
      }
    if(O_dolink)
      printf("Executable file is generated: %s \n",outfilename);
    printf("Time elapsed in ms: %f \n", elapsed);
    printf("\n");
    }
  return (0);
}
