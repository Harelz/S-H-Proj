#include "SPArrayList.h"

SPArrayList* spArrayListCreate(int maxSize) {
	if (maxSize <= 0)
		return NULL;
	SPArrayList* instance = (SPArrayList *) malloc(sizeof(SPArrayList));
	if (instance == NULL) return NULL;
	instance -> maxSize = maxSize;
	instance -> actualSize = 0;
	instance -> elements = (int*) malloc(maxSize * sizeof(int));
	if (instance->elements == NULL) {
		free(instance);
		return NULL;
	}
	return instance;
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	if (src == NULL) return NULL;
	SPArrayList* cpy = spArrayListCreate(src->maxSize);
	if (cpy == NULL) return NULL;
	for(int i = 0; i < src->maxSize; i++)
		cpy->elements[i] = src->elements[i];
	cpy->maxSize = src->maxSize;
	cpy->actualSize = src->actualSize;
	return cpy;
}

void spArrayListDestroy(SPArrayList* src) {
	if(src != NULL){
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src ->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	if (src == NULL || src->actualSize < index || index < 0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize >= src->maxSize)
		return SP_ARRAY_LIST_FULL;
	for (int i = src ->actualSize; i > index; i--)
		src->elements[i] = src->elements[i - 1];
	src -> actualSize++;
	src->elements[index] = elem;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	if(src == NULL) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	if(src == NULL) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	if (src == NULL || src->actualSize < index || index < 0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src))
		return SP_ARRAY_LIST_EMPTY;
	src -> actualSize--;
	for (int i = index; i < src ->actualSize; ++i)
		src->elements[i] = src->elements[i + 1];
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	if(src == NULL) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src) {
	if(src == NULL) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListRemoveAt(src, src->actualSize);
}

int spArrayListGetAt(SPArrayList* src, int index) {
	if (src == NULL || src->actualSize <= index || src->actualSize == 0) {
		return UNDEFINED_VALUE;
	}
	return src->elements[index];
}

int spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src) {
	return spArrayListGetAt(src, src->actualSize - 1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if (src == NULL) {
		return UNDEFINED_VALUE;
	}

	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if (src == NULL) {
		return UNDEFINED_VALUE;
	}

	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src) {
	return !(src == NULL || src->maxSize > src->actualSize);
}

bool spArrayListIsEmpty(SPArrayList* src) {
	return !(src == NULL || src->actualSize != 0);
}

