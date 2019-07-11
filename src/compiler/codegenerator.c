#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include "node.h"
#include "codegenerator.h"

struct Type {
  char *typeName;
  LLVMTypeRef llvmType;
};

struct Var {
  char *typeName;
  char *name;
};

struct Func {
  unsigned int paramCount;
  char **paramTypeNames;
  char *typeName;
  char *name;
};

struct Scope {
  const char *name;
  unsigned int varCount;
  struct Var *vars;
  unsigned int funcCount;
  struct Func *funcs;
  unsigned int typeCount;
  struct Type *types;
};

unsigned int scopeCount = 0;
struct Scope *currentScope = NULL;

void pushScope(char *name) {
  scopeCount++;
  currentScope = realloc(currentScope, scopeCount * sizeof(struct Scope));
  currentScope[scopeCount - 1] = currentScope[scopeCount - 2];
  //this only works 'cause the top scope is not used again after backtrack
  //this means the buffers don't need to be copyied as we are only adding not
  //removing

  currentScope[scopeCount - 2].name = name;
}

void pushVar(struct Var var) {
  currentScope[scopeCount - 1].varCount++;
  currentScope[scopeCount - 1].vars = realloc(currentScope[scopeCount - 1].vars, currentScope[scopeCount - 1].varCount * sizeof(struct Var));
  currentScope[scopeCount - 1].vars[currentScope[scopeCount - 1].varCount - 1] = var;
}

void pushFunc(struct Func func) {
  currentScope[scopeCount - 1].funcCount++;
  currentScope[scopeCount - 1].funcs = realloc(currentScope[scopeCount - 1].funcs, currentScope[scopeCount - 1].funcCount * sizeof(struct Func));
  currentScope[scopeCount - 1].funcs[currentScope[scopeCount - 1].funcCount - 1] = func;
}

void pushType(struct Type type) {
  currentScope[scopeCount - 1].typeCount++;
  currentScope[scopeCount - 1].types = realloc(currentScope[scopeCount - 1].types, currentScope[scopeCount - 1].typeCount * sizeof(struct Type));
  currentScope[scopeCount - 1].types[currentScope[scopeCount - 1].typeCount - 1] = type;
}

LLVMTypeRef codegenType() {
  return LLVMIntType(32);
}

LLVMValueRef codegenExpr(LLVMBuilderRef builder, struct Node tree);

LLVMValueRef codegenBlkParts(LLVMBuilderRef builder, struct Node tree) {
  if (tree.childCount == 2) {
    codegenBlkParts(builder, tree.children[0]);
    return codegenExpr(builder, tree.children[1]);
  } else if (tree.childCount == 1) {
    return codegenExpr(builder, tree.children[0]);
  } else {

  }
}

LLVMValueRef codegenBlk(LLVMBuilderRef builder, struct Node tree) {
  return codegenBlkParts(builder, tree.children[0]);
}

LLVMValueRef codegenAddExpr(LLVMBuilderRef builder, struct Node tree) {
  LLVMValueRef lhs = codegenExpr(builder, tree.children[0]);
  LLVMValueRef rhs = codegenExpr(builder, tree.children[1]);
  return LLVMBuildAdd(builder, lhs, rhs, "tmp");
}

LLVMValueRef codegenInteger(LLVMBuilderRef builder, struct Node tree) {
  return LLVMConstInt(LLVMIntType(32), 10, 1);
}

LLVMValueRef codegenVarDecl(LLVMBuilderRef builder, struct Node tree) {

}

LLVMValueRef codegenReturn(LLVMBuilderRef builder, struct Node tree) {
  return LLVMBuildRet(builder, codegenExpr(builder, tree.children[0]));
}

LLVMValueRef codegenExpr(LLVMBuilderRef builder, struct Node tree) {
  if (!strcmp(tree.children[0].data, "NBLK")) {
    return codegenBlk(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NADDEXPR")) {
    return codegenAddExpr(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NINTEGER")) {
    return codegenInteger(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NVALEXPR")) {

  } else if (!strcmp(tree.children[0].data, "NRETURNEXPR")) {
    return codegenReturn(builder, tree.children[0]);
  }
}

LLVMTypeRef codegenParam(struct Node tree) {
  pushVar((struct Var) {tree.children[1].data, tree.children[0].data});
  return codegenType();
}

struct ParamsLLVM {
  LLVMTypeRef *args;
  char **names;
  unsigned int argCount;
};

struct ParamsLLVM codegenParams(struct Node tree) {
  struct ParamsLLVM outStruct;
  outStruct.args = (LLVMTypeRef *) malloc(0);
  outStruct.names = (char **) malloc(0);
  int size = 0;
  struct Node current = tree;
  while (current.childCount == 2) {
    size++;
    outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(struct Node));
    outStruct.names = (char **) realloc(outStruct.names, size * sizeof(char *));
    outStruct.args[size - 1] = codegenParam(current.children[0]);
    outStruct.names[size - 1] = current.children[0].children[0].data;
    current = current.children[1];
  }
  size++;
  outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(struct Node));
  outStruct.names = (char **) realloc(outStruct.names, size * sizeof(char *));
  outStruct.args[size - 1] = codegenParam(current.children[0]);
  outStruct.names[size - 1] = current.children[0].children[0].data;
  outStruct.argCount = size;
  return outStruct;
}

LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node tree) {
  char *name = tree.children[0].data;
  struct ParamsLLVM params = codegenParams(tree.children[1]);
  LLVMTypeRef type = LLVMIntType(32);
  LLVMValueRef out = LLVMAddFunction(module, name, LLVMFunctionType(type, params.args, params.argCount, 0));
  LLVMSetLinkage(out, LLVMExternalLinkage);
  for (int i = 0; i < params.argCount; i++) {
    LLVMSetValueName2(LLVMGetParam(out, i), params.names[i], strlen(params.names[i]));
  }
  pushFunc((struct Func) {params.argCount, params.names, "Int", name});
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(out, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);
  codegenExpr(builder, tree.children[3]);
  return out;
}

LLVMModuleRef codegenTLD(struct Node tree) {
  LLVMModuleRef module = LLVMModuleCreateWithName("grime");
  LLVMBuilderRef builder = LLVMCreateBuilder();
  codegenFunc(module, builder, tree.children[0].children[0]);
  //LLVMDisposeBuilder(builder);
  return module;
}

void codegen(struct Node tree) {
  scopeCount = 1;
  currentScope = malloc(scopeCount * sizeof(struct Scope));
  currentScope[scopeCount - 1] = (struct Scope){"", 0, NULL, 0, NULL, 0, NULL};
  LLVMModuleRef module = codegenTLD(tree);
  printf("%s\n", LLVMPrintModuleToString(module));
}
