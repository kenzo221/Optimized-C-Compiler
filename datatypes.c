#include "definitions.h"
#include "data.h"
#include "declaration.h"

//Data Types and type handling

//Return true if a type is an int type of any size, false otherwise
int inttype(int type) {
  return (((type & 0xf) == 0) && (type >= P_CHAR && type <= P_LONG));
}

//Return true if a type is of pointer type
int ptrtype(int type) {
  return ((type & 0xf) != 0);
}

//Given a primitive type, return the type which is a pointer to it
int pointer_to(int type) {
  if ((type & 0xf) == 0xf)
    fatald("Unrecognised in pointer_to: type", type);
  return (type + 1);
}

//Given a primitive pointer type, return the type which it points to
int value_at(int type) {
  if ((type & 0xf) == 0x0)
    fatald("Unrecognised in value_at: type", type);
  return (type - 1);
}

//Given a type and a composite type pointer, return the size of this type in bytes
int typesize(int type, struct symtable *ctype) {
  if (type == P_STRUCT || type == P_UNION)
    return (ctype->size);
  return (genprimsize(type));
}

// Given an AST tree and a type which we want it to become, possibly modify the tree by widening or scaling so that it is compatible with this type. Return the original tree if no changes occurred, a modified tree, or NULL if the tree is not compatible with the given type. If this will be part of a binary operation, the AST op is not zero
struct ASTnode *modify_type(struct ASTnode *tree, int rtype, int op) {
  int ltype;
  int lsize, rsize;

  ltype = tree->type;

  if (ltype == P_STRUCT || ltype == P_UNION)
    fatal("Don't know how to do this yet");
  if (rtype == P_STRUCT || rtype == P_UNION)
    fatal("Don't know how to do this yet");

  //Compare scalar int types
  if (inttype(ltype) && inttype(rtype)) {

    //Both types same, nothing to do
    if (ltype == rtype)
      return (tree);

    //Get the sizes for each type
    lsize = typesize(ltype, NULL);
    rsize = typesize(rtype, NULL);

    //Tree's size is too big
    if (lsize > rsize)
      return (NULL);

    //Widen to the right
    if (rsize > lsize)
      return (mkastunary(A_WIDEN, rtype, tree, NULL, 0));
  }

  //For pointers
  if (ptrtype(ltype) && ptrtype(rtype)) {
    //Compare pointer
    if (op >= A_EQ && op <= A_GE)
      return(tree);

    //A comparison of the same type for a non-binary operation is ok, or when the left tree is of  `void *` type.
    if (op == 0 && (ltype == rtype || ltype == pointer_to(P_VOID)))
      return (tree);
  }

  //Scale on A_ADD or A_SUBTRACT operation
  if (op == A_ADD || op == A_SUBTRACT) {

    //Left is int type, right is pointer type and the size of the original type is >1: scale the left
    if (inttype(ltype) && ptrtype(rtype)) {
      rsize = genprimsize(value_at(rtype));
      if (rsize > 1)
	return (mkastunary(A_SCALE, rtype, tree, NULL, rsize));
      else
	return (tree);		//Size 1, no need to scale
    }
  }
  //Types are not compatible
  return (NULL);
}
