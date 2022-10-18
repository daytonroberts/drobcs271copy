// plist.c

// implements functions in plist.h

#include "plist.h"

struct plist_struct
{
	void *data;
	plist next;
	bool empt;
};

plist pl_new()
{
	plist ret = malloc(sizeof(struct plist_struct));
	ret->empt = TRUE;
	ret->next = NULL;
	return ret;
}

void pl_free(plist l)
{
	if (l->empt) {
		pl_free(l->next);
	}
	else {
		free(l);
	}
	return;
}

void p_wrapped(plist l)
{
	while (l->next) {
		printf("%lu, ", l->data);
		p_wrapped(l->next);
	}
	printf("%lu", l->data);
	return;
}	

void pl_print(plist l)
{
	printf("[");
	p_wrapped(l);
	printf("]");
	return;
}

void pl_append(plist l, void *x)
{
	if (l->next) {
		pl_append(l->next, x);
	}
	else { 
		plist ln = pl_new();
		ln -> data = x;
		l -> next = ln;
	}
	return;
}

void pl_extend(plist l1, plist l2)
{
	if (l1 -> next) {
		pl_extend(l1->next, l2);
	}
	else {
		l1->next = l2;
	}
	return;
}

void pl_insert(plist l, uint64_t i, void *x)
{
	uint64_t index = i;
	while (index > 1 && !l->empt){
		l=l->next;
		index--;
	}
	l->next=l;
	l->data=x;
	return;
}

bool pl_remove(plist l, void *x)
{
	if (l->next) {
		if (l->next->data == x) {
			plist hold = l->next;
			l->next = l->next->next;
			free(hold);
			return TRUE;
		}
		pl_remove(l->next, x);
	}
	return FALSE;
}

void *pl_pop(plist l, uint64_t i)
{
	uint64_t index = 0;
	while (index < i) {
		l = l->next;
		index++;
	}
	plist temp = l->next;
	void *ret = temp->data;
	l->next=l->next->next;
	free(temp);
	return ret;
}

void pl_clear(plist l)
{
	l->empt = TRUE;
	l->data = NULL;
	pl_free(l->next);
	return;
}

uint64_t pl_size(plist l){
	int i = 0;
	while (l->data){
		i++;
		l=l->next;
	}
	return i;
}


uint64_t pl_index(plist l, void *x)
{
	uint64_t index = 0;
	if (l->data){
		while (l->data != x){
			l = l->next;
			index += 1;
		}
	}
	if (index != pl_size(l)){
		return index;
	}
	exit(1);
}


uint64_t pl_count(plist l, void *x)
{	
	uint64_t count = 0;
	while (pl_remove(l,x)){
		count++;
	}
	return count;
}


void pl_reverse(plist l)
{
	plist ret = pl_new();
	plist temp = l;
	while(l->data) {
		l=l->next;
		pl_insert(l, 0, l->data);
	}
	l=ret;
	free(temp);
	return;
}


plist pl_copy(plist l)
{
	plist ret = pl_new();
	while(l->data) {
		l=l->next;
		pl_append(ret, l->data);
	}
	return ret;
}
