#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include "node.h"
#include "codegen.h"

struct Type {
  char *typeName;
  char *mangledName;
  LLVMTypeRef llvmType;
};

struct Var {
  struct Type typeName;
  char *name;
  bool mutable;
  LLVMValueRef llvmVal;
};

struct Func {
  char *name;
  char *mangledName;
  LLVMValueRef llvmFunc;
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
  fprintf(stderr, "Error in variable name resolution: Variable %s does not exist!\n", name);
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
  fprintf(stderr, "Error in type name resolution: Type %s does not exist!\n", name);
  exit(-1);
}

struct Func getFuncFromName(const char *name) {
	for (int i = 0; i < currentScope[scopeCount - 1].funcCount; i++) {
		if (!strcmp(name, currentScope[scopeCount - 1].funcs[i].name)) {
			return currentScope[scopeCount - 1].funcs[i];
		}
	}
	fprintf(stderr, "Error in function name resolution: Function %s does not exist!\n", name);
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
	struct Func func = getFuncFromName("gc_literal_construct_g_xyz_grime_core_Int");
	LLVMValueRef strct= LLVMBuildAlloca(builder, getTypeFromName("Int").llvmType, "");
	LLVMBuildCall(builder, func.llvmFunc, (LLVMValueRef []) {strct, LLVMConstInt(LLVMIntType(32), strtol(tree.children[0].data, NULL, 10), 1)}, 2, "");
	return LLVMBuildLoad(builder, strct, "");
}

LLVMValueRef codegenChar(LLVMBuilderRef builder, struct Node tree) {
	struct Func func = getFuncFromName("gc_literal_construct_g_xyz_grime_core_Char");
	LLVMValueRef strct = LLVMBuildAlloca(builder, getTypeFromName("Char").llvmType, "");
	LLVMBuildCall(builder, func.llvmFunc, (LLVMValueRef []) {strct, LLVMConstInt(LLVMIntType(8), tree.children[0].data[1], 1)}, 2, "");
	return LLVMBuildLoad(builder, strct, "");
}

LLVMValueRef codegenLong(LLVMBuilderRef builder, struct Node tree) {
	char substring[strlen(tree.children[0].data)];
	memcpy(substring, tree.children[0].data, strlen(tree.children[0].data) - 1);
	substring[strlen(tree.children[0].data) - 1];

	struct Func func = getFuncFromName("gc_literal_construct_g_xyz_grime_core_Long");
	LLVMValueRef strct = LLVMBuildAlloca(builder, getTypeFromName("Long").llvmType, "");
	LLVMBuildCall(builder, func.llvmFunc, (LLVMValueRef []) {strct, LLVMConstInt(LLVMIntType(64), strtol(substring, NULL, 10), 1)}, 2, "");


	return LLVMBuildLoad(builder, strct, "");
}

LLVMValueRef codegenVarDecl(LLVMBuilderRef builder, struct Node tree) {
	struct Var var;
	var.typeName = codegenType(tree.children[1]);
	var.name = tree.children[0].data;
	var.mutable = true;
	var.llvmVal = LLVMBuildAlloca(builder, codegenType(tree.children[1]).llvmType, "");
	LLVMBuildStore(builder, codegenExpr(builder, tree.children[2]), var.llvmVal);
	pushVar(var);
	return var.llvmVal;
}

LLVMValueRef codegenValDecl(LLVMBuilderRef builder, struct Node tree) {
	struct Var var;
	var.name = tree.children[0].data;
	var.mutable = false;
	var.llvmVal = codegenExpr(builder, tree.children[2]);
	pushVar(var);
	return var.llvmVal;
}

LLVMValueRef codegenReturn(LLVMBuilderRef builder, struct Node tree) {
  return LLVMBuildRet(builder, codegenExpr(builder, tree.children[0]));
}

LLVMValueRef codegenAssign(LLVMBuilderRef builder, struct Node tree) {
	struct Var var = getVarFromName(tree.children[0].data);
	if (var.mutable) {
		return LLVMBuildStore(builder, codegenExpr(builder, tree.children[1]), var.llvmVal);
	} else {
		fprintf(stderr, "Error in assignment: Variable %s is not mutable!\n", var.name);
		exit(-1);
	}
}

LLVMValueRef codegenIdExpr(LLVMBuilderRef builder, struct Node tree) {
	struct Var var = getVarFromName(tree.children[0].data);
	if (!var.mutable) {
		return var.llvmVal;
	} else {
		return LLVMBuildLoad(builder, getVarFromName(tree.children[0].data).llvmVal, "");
	}
}

LLVMValueRef codegenFuncCall(LLVMBuilderRef builder, struct Node tree) {
	struct Func func = getFuncFromName(tree.children[0].data);
	return LLVMBuildCall(builder, func.llvmFunc, NULL, 0, "");
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
	} else if (!strcmp(tree.children[0].data, "NCHAR")) {
		return codegenChar(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NVALEXPR")) {
		return codegenValDecl(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NVAREXPR")) {
		return codegenVarDecl(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NRETURNEXPR")) {
		return codegenReturn(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NIDEXPR")) {
		return codegenIdExpr(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NASSEXPR")) {
		return codegenAssign(builder, tree.children[0]);
	} else if (!strcmp(tree.children[0].data, "NFUNCCALLEXPR")) {
		return codegenFuncCall(builder, tree.children[0]);
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
	if (tree.childCount == 0) {
		struct ParamsLLVM out;
		out.args = NULL;
		out.type = NULL;
		out.names = NULL;
		out.argCount = 0;
		return out;
	}
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

char *mangleFunctionName(char *baseName) {
  char *out = (char *) malloc((strlen(baseName) + 3) * sizeof(char));
  memcpy(out, "g_", 2);
  memcpy(out + 2, baseName, strlen(baseName));
  out[strlen(baseName) + 2] = '\0';
  return out;
}

LLVMValueRef codegenExternFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node tree) { 
	char *name = tree.children[0].data;
	struct ParamsLLVM params = codegenParams(tree.children[1]);
	LLVMTypeRef type = codegenType(tree.children[2]).llvmType;
	LLVMValueRef out = LLVMAddFunction(module, name, LLVMFunctionType(type, params.args, params.argCount, 0));
	LLVMSetLinkage(out, LLVMExternalLinkage);
	for (int i = 0; i < params.argCount; i++) {
		pushVar((struct Var) {params.type[i], params.names[i], false, LLVMGetParam(out, i)});
	}
	pushFunc((struct Func) {name, mangleFunctionName(name), out});
	return out;
}

LLVMValueRef codegenFunc(LLVMModuleRef module, LLVMBuilderRef builder, struct Node tree) {
  char *name = tree.children[0].data;
  struct ParamsLLVM params = codegenParams(tree.children[1]);
  LLVMTypeRef type = codegenType(tree.children[2]).llvmType;
  LLVMValueRef out = LLVMAddFunction(module, mangleFunctionName(name), LLVMFunctionType(type, params.args, params.argCount, 0));
  LLVMSetLinkage(out, LLVMExternalLinkage);
  for (int i = 0; i < params.argCount; i++) {
    pushVar((struct Var) {params.type[i], params.names[i], false, LLVMGetParam(out, i)});
  }
  pushFunc((struct Func) {name, mangleFunctionName(name), out});
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(out, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);
  codegenExpr(builder, tree.children[3]);
  return out;
}

LLVMModuleRef codegenTLD(struct Node tree) {
	LLVMModuleRef module = LLVMModuleCreateWithName("grime");
	LLVMBuilderRef builder = LLVMCreateBuilder();

	struct Func intConstruct;
	intConstruct.name = "gc_literal_construct_g_xyz_grime_core_Int";
 	intConstruct.llvmFunc = LLVMAddFunction(module, "gc_literal_construct_g_xyz_grime_core_Int",
		  LLVMFunctionType(LLVMVoidType(), (LLVMTypeRef []) {LLVMPointerType(getTypeFromName("Int").llvmType, 0), LLVMIntType(32)}, 2, 0));

	struct Func longConstruct;
	longConstruct.name = "gc_literal_construct_g_xyz_grime_core_Long";
	longConstruct.llvmFunc = LLVMAddFunction(module, "gc_literal_construct_g_xyz_grime_core_Long",
		  LLVMFunctionType(LLVMVoidType(), (LLVMTypeRef []) {LLVMPointerType(getTypeFromName("Long").llvmType, 0), LLVMIntType(64)}, 2, 0));

	struct Func charConstruct;
	charConstruct.name = "gc_literal_construct_g_xyz_grime_core_Char";
	charConstruct.llvmFunc = LLVMAddFunction(module, "gc_literal_construct_g_xyz_grime_core_Char",
		  LLVMFunctionType(LLVMVoidType(), (LLVMTypeRef []) {LLVMPointerType(getTypeFromName("Char").llvmType, 0), LLVMIntType(8)}, 2, 0));

	pushFunc(charConstruct);
	pushFunc(intConstruct);
	pushFunc(longConstruct);

	codegenFunc(module, builder, tree.children[0].children[0]);

	LLVMValueRef mainFunc = LLVMAddFunction(module, "main", LLVMFunctionType(LLVMIntType(32), NULL, 0, 0));
	LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	LLVMValueRef objIntP = LLVMBuildAlloca(builder, getTypeFromName("Int").llvmType, "");
	LLVMValueRef objInt = LLVMBuildCall(builder, getFuncFromName("main").llvmFunc, NULL, 0, "");
	LLVMBuildStore(builder, objInt, objIntP);
	LLVMValueRef fp = LLVMBuildStructGEP2(builder, getTypeFromName("Int").llvmType, objIntP, 2, "");
	LLVMValueRef exit = LLVMBuildCall(builder, LLVMBuildLoad(builder, fp, ""), (LLVMValueRef []) {objInt}, 1, "");
	fp = LLVMBuildStructGEP2(builder, getTypeFromName("Int").llvmType, objIntP, 1, "");
	LLVMBuildCall(builder, LLVMBuildLoad(builder, fp, ""), (LLVMValueRef[]) {objInt}, 1, "");
	LLVMBuildRet(builder, exit);
	LLVMDisposeBuilder(builder);
	return module;
}

void codegen(struct Node tree) {
	scopeCount = 1;
	currentScope = malloc(scopeCount * sizeof(struct Scope));
	currentScope[scopeCount - 1] = (struct Scope){"TLD", 0, NULL, 0, NULL, 0, NULL};

  	struct Type voidType;
	voidType.typeName = "Void";
	voidType.mangledName = "g_xyz_grime_core_Void";
	voidType.llvmType = LLVMVoidType();
  
	struct Type boolType;
	boolType.typeName = "Bool";
	boolType.mangledName = "g_xyz_grime_core_Bool";
	boolType.llvmType = LLVMIntType(1);

	struct Type byteType;
	byteType.typeName = "Byte";
	byteType.mangledName = "g_xyz_grime_core_Byte";
	byteType.llvmType = LLVMIntType(8);

	struct Type shortType;
	shortType.typeName = "Short";
	shortType.mangledName = "g_xyz_grime_core_Short";
	shortType.llvmType = LLVMIntType(16);

	struct Type intType;
	intType.typeName = "Int";
	intType.mangledName = "g_xyz_grime_core_Int";
	intType.llvmType = LLVMStructCreateNamed(LLVMGetGlobalContext(), "g_xyz_grime_core_Int");
	LLVMStructSetBody(intType.llvmType, (LLVMTypeRef []) {
		LLVMPointerType(LLVMIntType(8), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMVoidType(),
			(LLVMTypeRef[]) {
				intType.llvmType
			},
			1,
			0
		), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMIntType(32),
			(LLVMTypeRef[]) {
				intType.llvmType
			},
			1,
			0			
		), 0),
		LLVMPointerType(LLVMFunctionType(
			intType.llvmType,
			(LLVMTypeRef[]) {
				intType.llvmType,
				intType.llvmType
			},
			2,
			0
		), 0)
	}, 3, 0);
 
	struct Type longType;
	longType.typeName = "Long";
	longType.mangledName = "g_xyz_grime_core_Long";
	longType.llvmType = LLVMStructCreateNamed(LLVMGetGlobalContext(), "g_xyz_grime_core_Long");
	LLVMStructSetBody(longType.llvmType, (LLVMTypeRef []) {
		LLVMPointerType(LLVMIntType(8), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMVoidType(),
			(LLVMTypeRef[]) {
				longType.llvmType
			},
			1,
			0
		), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMIntType(64),
			(LLVMTypeRef[]) {
				longType.llvmType
			},
			1,
			0			
		), 0)
	}, 3, 0);


	struct Type charType;
	charType.typeName = "Char";
	charType.mangledName = "g_xyz_grime_core_Char";
	charType.llvmType = LLVMStructCreateNamed(LLVMGetGlobalContext(), "g_xyz_grime_core_Char");
	LLVMStructSetBody(charType.llvmType, (LLVMTypeRef []) {
		LLVMPointerType(LLVMIntType(8), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMVoidType(),
			(LLVMTypeRef[]) {
				charType.llvmType
			},
			1,
			0
		), 0),
		LLVMPointerType(LLVMFunctionType(
			LLVMIntType(8),
			(LLVMTypeRef[]) {
				charType.llvmType
			},
			1,
			0			
		), 0)
	}, 3, 0);
 
	pushType(voidType);
	pushType(boolType);
	pushType(byteType);
	pushType(shortType);
	pushType(intType);
	pushType(longType);

	pushType(charType);

	LLVMModuleRef module = codegenTLD(tree);
	printf("%s\n", LLVMPrintModuleToString(module));
}
