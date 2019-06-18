#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include "node.h"
#include "codegenerator.h"

LLVMTypeRef codegenType() {
  return LLVMIntType(32);
}

LLVMTypeRef codegenParam(struct Node tree) {
  return LLVMIntType(32);
}

struct ParamsLLVM {
  LLVMTypeRef *args;
  unsigned int argCount;
};

struct ParamsLLVM codegenParams(struct Node tree) {
  struct ParamsLLVM outStruct;
  outStruct.args = (LLVMTypeRef *) malloc(0);
  int size = 0;
  struct Node current = tree;
  while (current.childCount == 2) {
    size++;
    outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(struct Node));
    outStruct.args[size - 1] = codegenParam(current.children[0]);
    current = current.children[1];
  }
  size++;
  outStruct.args = (LLVMTypeRef *) realloc(outStruct.args, size * sizeof(struct Node));
  outStruct.args[size - 1] = codegenParam(current.children[0]);
  outStruct.argCount = size;
  return outStruct;
}

LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node tree) {
  const char *name = tree.children[0].data;
  struct ParamsLLVM params = codegenParams(tree.children[1]);
  //LLVMTypeRef *args = {LLVMIntType(32)};
  LLVMTypeRef type = LLVMIntType(32);
  LLVMValueRef out = LLVMAddFunction(module, name, LLVMFunctionType(type, params.args, params.argCount, 0));
  LLVMSetLinkage(out, LLVMExternalLinkage);
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(out, "entry");
  return out;
}

LLVMModuleRef codegenTLD(struct Node tree) {
  LLVMModuleRef module = LLVMModuleCreateWithName("grime");
  LLVMBuilderRef builder = LLVMCreateBuilder();
  codegenFunc(module, builder, tree.children[0].children[0]);
  LLVMDisposeBuilder(builder);
  return module;
}

void codegen(struct Node tree) {
  LLVMModuleRef module = codegenTLD(tree);
  printf("%s\n", LLVMPrintModuleToString(module));
}
