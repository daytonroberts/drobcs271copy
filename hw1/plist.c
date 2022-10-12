// plist.c

// implements functions in plist.h

#include "plist.h"

struct plist_struct
{
	void *data;
	plist next;
};

plist pl_new()
{
	plist ret = malloc(sizeof(struct(plist_struct));
	ret -> data = 0x4;
	ret -> next = malloc(ret -> data * sizeof(struct(plist_struct));
	return ret;
}

void pl_free(plist l)
{
	free(l);
	return;
}

void pl_print(plist l)
{
	printf("[");
	for (int i = 0; i < sizeof(l)/sizeof(int); i++)
	{
		if i == sizeof(l)/sizeof(int) - 1 {
			printf(("%d, ", l->data);
		}
		else {
			printf("%d", l->data);
		}
	}
	printf("]");
	return;
}

void pl_append(plist l, void *x)
{
	if l->next {
		pl_append(l->next, x)
	}
	else { 
		plist ln = pl_new();
		ln -> data = x
		l -> next = ln
	}
	return;
}

void pl_extend(plist l1, plist l2)
{
	if l1 -> next {
		pl_extend(l1->next, l2)
	}
	else {
		l1->next = l2
	}
	return;
}

void pl_insert(plist l, uint64_t i, void *x)
{
	return;
}

bool pl_remove(plist l, void *x)
{
	return FALSE;
}

void *pl_pop(plist l, uint64_t i)
{
	return NULL;
}

void pl_clear(plist l)
{
	return;
}

uint64_t pl_index(plist l, void *x)
{
	return -1;
}

uint64_t pl_count(plist l, void *x)
{
	return 0;
}

void pl_reverse(plist l)
{
	return;
}

plist pl_copy(plist l)
{
	return NULL;
}
