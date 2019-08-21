#include <stdlib.h>
#include <stdio.h>

#define DEBUG

struct g_xyz_grime_core_List {
	void *data;
	void (*g_destruct)(struct g_xyz_grime_core_List);
	void **(*g_cget)(struct g_xyz_grime_core_List);
	void *(*g_get)(struct g_xyz_grime_core_List, int);
};

struct g_xyz_grime_core_Int {
	void *data;
	void (*g_destruct)(struct g_xyz_grime_core_Int);
	int (*g_cget)(struct g_xyz_grime_core_Int);
	struct g_xyz_grime_core_Int (*g_add)(struct g_xyz_grime_core_Int, struct g_xyz_grime_core_Int);
};

void gc_literal_construct_g_xyz_grime_core_Int(struct g_xyz_grime_core_Int *this, int x);

void gc_literal_g_destruct_g_xyz_grime_core_Int(struct g_xyz_grime_core_Int this) {
#ifdef DEBUG
	printf("destruct Int called\n");
#endif 
	free(this.data);
}

int gc_literal_g_cget_g_xyz_grime_core_Int(struct g_xyz_grime_core_Int this) {
#ifdef DEBUG
	printf("cget Int called\n");
#endif
	return ((int *) this.data)[0];
}

struct g_xyz_grime_core_Int gc_literal_g_add_g_xyz_grime_core_Int(struct g_xyz_grime_core_Int x, struct g_xyz_grime_core_Int y) {
#ifdef DEBUG
	printf("add Int called\n");
#endif
	struct g_xyz_grime_core_Int out;
	gc_literal_construct_g_xyz_grime_core_Int(&out, x.g_cget(x) + y.g_cget(y));
	return out;
}

void gc_literal_construct_g_xyz_grime_core_Int(struct g_xyz_grime_core_Int *this, int x) {
#ifdef DEBUG
	printf("construct Int called\n");
#endif
	this->data = malloc(sizeof(int));
	((int*) this->data)[0] = x;
	this->g_destruct = &gc_literal_g_destruct_g_xyz_grime_core_Int;
	this->g_cget = &gc_literal_g_cget_g_xyz_grime_core_Int;
	this->g_add = &gc_literal_g_add_g_xyz_grime_core_Int;
}

struct g_xyz_grime_core_Long {
	void *data;
	void (*g_destruct)(struct g_xyz_grime_core_Long);
	long (*g_cget)(struct g_xyz_grime_core_Long);
};

void gc_literal_g_destruct_g_xyz_grime_core_Long(struct g_xyz_grime_core_Long this) {
#ifdef DEBUG
	printf("destruct Long called\n");
#endif 
	free(this.data);
}

long gc_literal_g_cget_g_xyz_grime_core_Long(struct g_xyz_grime_core_Long this) {
#ifdef DEBUG
	printf("cget Long called\n");
#endif
	return ((long *) this.data)[0];
}

void gc_literal_construct_g_xyz_grime_core_Long(struct g_xyz_grime_core_Long *this, long x) {
#ifdef DEBUG
	printf("construct Long called\n");
#endif
	this->data = malloc(sizeof(long));
	((long *) this->data)[0] = x;
	this->g_destruct = &gc_literal_g_destruct_g_xyz_grime_core_Long;
	this->g_cget = &gc_literal_g_cget_g_xyz_grime_core_Long;
}

struct g_xyz_grime_core_Char {
	void *data;
	void (*g_destruct)(struct g_xyz_grime_core_Char);
	char (*g_cget)(struct g_xyz_grime_core_Char);
};

void gc_literal_g_destruct_g_xyz_grime_core_Char(struct g_xyz_grime_core_Char this) {
#ifdef DEBUG
	printf("destruct Char called\n");
#endif 
	free(this.data);
}

char gc_literal_g_cget_g_xyz_grime_core_Char(struct g_xyz_grime_core_Char this) {
#ifdef DEBUG
	printf("cget Char called\n");
#endif
	return ((char *) this.data)[0];
}

void gc_literal_construct_g_xyz_grime_core_Char(struct g_xyz_grime_core_Char *this, char x) {
#ifdef DEBUG
	printf("construct Char called\n");
#endif
	this->data = malloc(sizeof(char));
	((char *) this->data)[0] = x;
	this->g_destruct = &gc_literal_g_destruct_g_xyz_grime_core_Char;
	this->g_cget = &gc_literal_g_cget_g_xyz_grime_core_Char;
}

