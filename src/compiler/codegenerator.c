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
  char *mangledName;
  LLVMTypeRef llvmType;
};

struct Var {
  struct Type typeName;
  char *name;
  LLVMValueRef llvmVal;
  bool isParam;
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

struct Var getVarFromName(const char *name) {
  for (int i = 0; i < currentScope[scopeCount - 1].varCount; i++) {
    if (!strcmp(name, currentScope[scopeCount - 1].vars[i].name)) {
      return currentScope[scopeCount - 1].vars[i];
    }
  }
  printf("Error: var %s is not defined\n", name);
  exit(-1);
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

struct Type getTypeFromName(const char *name) {
  for (int i = 0; i < currentScope[scopeCount - 1].typeCount; i++) {
    if (!strcmp(name, currentScope[scopeCount - 1].types[i].typeName)) {
      return currentScope[scopeCount - 1].types[i];
    }
  }
  printf("Error: type %s is not defined\n", name);
  exit(-1);
}

struct Type codegenType(struct Node tree) {
  if (!strcmp(tree.children[0].data, "NSIMPLETYPE")) {
    return getTypeFromName(tree.children[0].children[0].data);
  }
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
  return LLVMBuildAdd(builder, lhs, rhs, "");
}

LLVMValueRef codegenMulExpr(LLVMBuilderRef builder, struct Node tree) {
  LLVMValueRef lhs = codegenExpr(builder, tree.children[0]);
  LLVMValueRef rhs = codegenExpr(builder, tree.children[1]);
  return LLVMBuildMul(builder, lhs, rhs, "");
}

LLVMValueRef codegenInteger(LLVMBuilderRef builder, struct Node tree) {
  return LLVMConstInt(LLVMIntType(32), strtol(tree.children[0].data, NULL, 10), 1);
}

LLVMValueRef codegenLong(LLVMBuilderRef builder, struct Node tree) {
  char substring[strlen(tree.children[0].data)];
  memcpy(substring, tree.children[0].data, strlen(tree.children[0].data) - 1);
  substring[strlen(tree.children[0].data) - 1];
  return LLVMConstInt(LLVMIntType(64), strtol(substring, NULL, 10), 1);
}

LLVMValueRef codegenVarDecl(LLVMBuilderRef builder, struct Node tree) {
  struct Var var;
  var.typeName = codegenType(tree.children[1]);
  var.name = tree.children[0].data;
  var.llvmVal = LLVMBuildAlloca(builder, codegenType(tree.children[1]).llvmType, tree.children[0].data);
  var.isParam = false;
  pushVar(var);
  return var.llvmVal;
}

LLVMValueRef codegenReturn(LLVMBuilderRef builder, struct Node tree) {
  return LLVMBuildRet(builder, codegenExpr(builder, tree.children[0]));
}

LLVMValueRef codegenAssign(LLVMBuilderRef builder, struct Node tree) {
  return LLVMBuildStore(builder, codegenExpr(builder, tree.children[1]), getVarFromName(tree.children[0].data).llvmVal);
}

LLVMValueRef codegenIdExpr(LLVMBuilderRef builder, struct Node tree) {
  struct Var var = getVarFromName(tree.children[0].data);
  if (var.isParam) {
    return var.llvmVal;
  } else {
    return LLVMBuildLoad(builder, getVarFromName(tree.children[0].data).llvmVal, "");
  }
}

LLVMValueRef codegenExpr(LLVMBuilderRef builder, struct Node tree) {
  if (!strcmp(tree.children[0].data, "NBLK")) {
    return codegenBlk(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NADDEXPR")) {
    return codegenAddExpr(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NMULEXPR")) {
    return codegenMulExpr(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NINTEGER")) {
    return codegenInteger(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NLONG")) {
    return codegenLong(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NVALEXPR")) {
    LLVMValueRef ref = codegenVarDecl(builder, tree.children[0]);
    LLVMBuildStore(builder, codegenExpr(builder, tree.children[0].children[2]), ref);
    return ref;
  } else if (!strcmp(tree.children[0].data, "NRETURNEXPR")) {
    return codegenReturn(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NIDEXPR")) {
    return codegenIdExpr(builder, tree.children[0]);
  } else if (!strcmp(tree.children[0].data, "NASSEXPR")) {
    return codegenAssign(builder, tree.children[0]);
  }
}

struct Type codegenParam(struct Node tree) {
  struct Type type = codegenType(tree.children[1]);
  return type;
}

struct ParamsLLVM {
  LLVMTypeRef *args;
  struct Type *type;
  char **names;
  unsigned int argCount;
};

struct ParamsLLVM codegenParams(struct Node tree) {
  struct ParamsLLVM outStruct;
  outStruct.args = (LLVMTypeRef *) malloc(0);
  outStruct.type = (struct Type *) malloc(0);
  outStruct.names = (char **) malloc(0);
  int size = 0;
  struct Node current = tree;
  while (current.childCount == 2) {
    size++;
    outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(LLVMTypeRef));
    outStruct.type = (struct Type *) realloc(outStruct.type, size * sizeof(struct Type));
    outStruct.names = (char **) realloc(outStruct.names, size * sizeof(char *));
    outStruct.args[size - 1] = codegenParam(current.children[0]).llvmType;
    outStruct.type[size - 1] = codegenParam(current.children[0]);
    outStruct.names[size - 1] = current.children[0].children[0].data;
    current = current.children[1];
  }
  size++;
  outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(LLVMTypeRef));
  outStruct.type = (struct Type *) realloc(outStruct.type, size * sizeof(struct Type));
  outStruct.names = (char **) realloc(outStruct.names, size * sizeof(char *));
  outStruct.args[size - 1] = codegenParam(current.children[0]).llvmType;
  outStruct.type[size - 1] = codegenParam(current.children[0]);
  outStruct.names[size - 1] = current.children[0].children[0].data;
  outStruct.argCount = size;
  return outStruct;
}

char *mangleFunctionName(char *baseName, struct Type returnType, int paramTypesCount, struct Type *paramTypes) {
  char *out = (char *) malloc((strlen(baseName) + 4) * sizeof(char *));
  char *current = out;
  memcpy(current, "g_", 2);
  current += 2;
  memcpy(current, baseName, strlen(baseName));
  current += strlen(baseName);
  memcpy(current, "_", 1);
  current += 1;
  memcpy(current, returnType.mangledName, strlen(returnType.mangledName));
  current += strlen(returnType.mangledName);
  for (int i = 0; i < paramTypesCount; i++) {

  }
}

LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node tree) {
  char *name = tree.children[0].data;
  struct ParamsLLVM params = codegenParams(tree.children[1]);
  LLVMTypeRef type = codegenType(tree.children[2]).llvmType;
  LLVMValueRef out = LLVMAddFunction(module, name, LLVMFunctionType(type, params.args, params.argCount, 0));
  LLVMSetLinkage(out, LLVMExternalLinkage);
  for (int i = 0; i < params.argCount; i++) {
    LLVMSetValueName2(LLVMGetParam(out, i), params.names[i], strlen(params.names[i]));
    pushVar((struct Var) {params.type[i], params.names[i], LLVMGetParam(out, i), true});
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

  struct Type boolType;
  boolType.typeName = "Bool";
  boolType.mangledName = "o";
  boolType.llvmType = LLVMIntType(1);

  struct Type byteType;
  byteType.typeName = "Byte";
  byteType.mangledName = "b";
  byteType.llvmType = LLVMIntType(8);

  struct Type shortType;
  shortType.typeName = "Short";
  shortType.mangledName = "s";
  shortType.llvmType = LLVMIntType(16);

  struct Type intType;
  intType.typeName = "Int";
  intType.mangledName = "i";
  intType.llvmType = LLVMIntType(32);

  struct Type longType;
  longType.typeName = "Long";
  longType.mangledName = "l";
  longType.llvmType = LLVMIntType(64);

  struct Type charType;
  charType.typeName = "Char";
  charType.mangledName = "c";
  longType.llvmType = LLVMIntType(8);

  pushType(boolType);
  pushType(byteType);
  pushType(shortType);
  pushType(intType);
  pushType(longType);

  pushType(charType);

  LLVMModuleRef module = codegenTLD(tree);
  printf("%s\n", LLVMPrintModuleToString(module));
}
