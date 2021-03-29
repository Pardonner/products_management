#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "./headers/product.h"
#include "./headers/memory.h"

extern int errno;

Product createProduct()
{
	Product product;

	initProduct(&product);

	return product;
}

void initProduct(Product *product)
{
	product->name = NULL;
	product->description = NULL;

	product->cost = 0.0;
	product->amount = 0;

	product->_removesCount = 0;
	product->removesDescription = malloc(sizeof(char *) * (product->_removesCapacity = INITIAL_REMOVES_CAPACITY));
}

void destroyProduct(Product *product)
{
	freePointer(&product->name);

	freePointer(&product->description);

	product->cost = 0.0;
	product->amount = 0;

	for(int i = 0; i < product->_removesCount; i++)
		freePointer(&product->removesDescription[i]);

	free(product->removesDescription);
	product->removesDescription = NULL;

	product->_removesCapacity = 0;
}

bool addRemoveDescription(Product *product, char *removeDescription)
{
	errno = 0;

	if(product->_removesCount >= product->_removesCapacity)
	{
		product->removesDescription = realloc(product->removesDescription, sizeof(Product) * 
			(product->_removesCapacity += PRODUCT_RESIZE_ADD_CAPACITY));

		if(product->removesDescription == NULL || errno != 0)
			return false;
	}

	product->removesDescription[product->_removesCount++] = removeDescription;
	return true;
}

char *productToString(Product product)
{
	char *exits = createStringPointer(sizeof(char) * 300);

	for(int i = 0; i < product._removesCount; i++)
	{
		if(i != 0)
			strcat(exits, ", ");

		strcat(exits, product.removesDescription[i]);
	}

	resizeStringPointer(&exits);

	char *result = createStringPointer(sizeof(char) * 500);

	sprintf(result, "name(%s), description(%s), cost(%f), amount(%hu), exits(%s)", product.name, product.description, 
		product.cost, product.amount, exits);

	resizeStringPointer(&result);
	freePointer(&exits);

	return result;
}

void copyProductTo(Product product, Product *container)
{
	container->name = product.name;
	container->description = product.description;

	container->cost = product.cost;
	container->amount = product.amount;

	container->_removesCount = product._removesCount;
	container->removesDescription = product.removesDescription;
}

bool isProductsEquals(Product p1, Product p2)
{
	return strcmp(p1.name, p2.name) == 0;
}