#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>

LLVMModuleRef codegenTLD(struct Node ast);
LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node ast);
LLVMTypeRef *codegenArgs(LLVMModuleRef module, struct Node ast);
LLVMTypeRef codegenArg(LLVMModuleRef module, struct Node ast);
LLVMTypeRef codegenType(LLVMModuleRef module, struct Node ast);
LLVMBasicBlockRef codegenBlk(LLVMValueRef func, LLVMBuilderRef builder, const char *name, struct Node ast);
LLVMValueRef codegenRet(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenExpr(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenInt(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenAdd(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenSub(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenMul(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenDiv(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenGRT(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenLST(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenGTE(LLVMBuilderRef builder, struct Node ast);
LLVMValueRef codegenLTE(LLVMBuilderRef builder, struct Node ast);

int stoi(const char *str) {
  size_t len = strlen(str);
  int a = 0;
  int o = 1;
  int n = 0;
  for (int i = len; i; i--) {
    n = (int)(str[i-1] - '0');
    if (n > 9 | n < 0) {
      printf("failed to get int from string: %s\n", str);
      exit(-1);
    }
    a += o * n;
    o *= 10;
  }
  return a;
}

void codegen(struct Node ast) {
  LLVMModuleRef module = codegenTLD(ast);
  printf("%s\n", LLVMPrintModuleToString(module));
}

LLVMModuleRef codegenTLD(struct Node ast) {
  LLVMModuleRef module = LLVMModuleCreateWithName("grime");
  LLVMBuilderRef builder = LLVMCreateBuilder();
  for (int i = 0; i < ast.childCount; i++) {
    codegenFunc(module, builder, ast.children[i]);
  }
  LLVMDisposeBuilder(builder);
  return module;
}

LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node ast) {
  const char *name = ast.children[0].data.data;
  LLVMTypeRef *args = codegenArgs(module, ast.children[1]);
  LLVMTypeRef type = codegenType(module, ast.children[2]);
  LLVMValueRef out = LLVMAddFunction(module, name, LLVMFunctionType(type, args, 0, 0));
  LLVMSetLinkage(out, LLVMExternalLinkage);
  LLVMBasicBlockRef blk = codegenBlk(out, builder, "entry", ast.children[3]);
  return out;
}

LLVMBasicBlockRef codegenBlk(LLVMValueRef func, LLVMBuilderRef builder, const char *name, struct Node ast) {
  LLVMBasicBlockRef out = LLVMAppendBasicBlock(func, name);
  LLVMPositionBuilderAtEnd(builder, out);
  for (int i = 0; i < ast.childCount; i++) {
    codegenExpr(builder, ast.children[i]);
  }
  return out;
}

LLVMValueRef codegenExpr(LLVMBuilderRef builder, struct Node ast) {
  switch (ast.children->type) {
    case NRETEXPR:
      return codegenRet(builder, ast.children[0]);
    case NINTEXPR:
      return codegenInt(builder, ast.children[0]);
    case NADDEXPR:
      return codegenAdd(builder, ast.children[0]);
    case NMULEXPR:
      return codegenMul(builder, ast.children[0]);
    case NSUBEXPR:
      return codegenSub(builder, ast.children[0]);
    case NDIVEXPR:
      return codegenDiv(builder, ast.children[0]);
    case NGRTEXPR:
      return codegenGRT(builder, ast.children[0]);
    case NLSTEXPR:
      return codegenLST(builder, ast.children[0]);
    case NGTEEXPR:
      return codegenGTE(builder, ast.children[0]);
    case NLTEEXPR:
      return codegenLTE(builder, ast.children[0]);
    default:
      printf("Unexpected node when generating expression!\n");
      exit(-1);
  }
}

LLVMValueRef codegenAdd(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildAdd(builder, rhs, lhs, "tmp");
}

LLVMValueRef codegenSub(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildSub(builder, rhs, lhs, "tmp");
}

LLVMValueRef codegenGRT(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildICmp(builder, LLVMIntSGT, rhs, lhs, "tmp");
}

LLVMValueRef codegenLST(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildICmp(builder, LLVMIntSLT, rhs, lhs, "tmp");
}

LLVMValueRef codegenGTE(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildICmp(builder, LLVMIntSGE, rhs, lhs, "tmp");
}

LLVMValueRef codegenLTE(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildICmp(builder, LLVMIntSLE, rhs, lhs, "tmp");
}


LLVMValueRef codegenDiv(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildUDiv(builder, rhs, lhs, "tmp");
}

LLVMValueRef codegenMul(LLVMBuilderRef builder, struct Node ast) {
  LLVMValueRef rhs = codegenExpr(builder, ast);
  LLVMValueRef lhs = codegenExpr(builder, ast.children[1]);
  return LLVMBuildMul(builder, rhs, lhs, "tmp");
}

LLVMValueRef codegenRet(LLVMBuilderRef builder, struct Node ast) {
  return LLVMBuildRet(builder, codegenExpr(builder, ast.children[0]));
}

LLVMValueRef codegenInt(LLVMBuilderRef builder, struct Node ast) {
  return LLVMConstInt(LLVMIntType(32), stoi(ast.data.data), 1);
}

LLVMTypeRef *codegenArgs(LLVMModuleRef module, struct Node ast) {
  LLVMTypeRef *out = malloc(ast.childCount * sizeof(LLVMTypeRef));
  for (int i = 0; i < ast.childCount; i++) {
      out[i] = codegenArg(module, ast.children[i]);
  }
  return out;
}

LLVMTypeRef codegenArg(LLVMModuleRef module, struct Node ast) {
  return codegenType(module, ast.children[1]);
}

LLVMTypeRef codegenType(LLVMModuleRef module, struct Node ast) {
  if (!strcmp(ast.data.data, "Int")) {
    return LLVMIntType(32);
  } else if (!strcmp(ast.data.data, "Bool")) {
    return LLVMIntType(1);
  } else {
    printf("Unrecognised type: %s\n", ast.data.data);
  }
}
