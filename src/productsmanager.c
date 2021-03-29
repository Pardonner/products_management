#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "./headers/product.h"
#include "./headers/productsmanager.h"

extern int errno;

Product **products = NULL;
unsigned int productCount = 0;

unsigned int productsLength = 0;

void initProductsManager()
{
	errno = 0;
	
	products = malloc(sizeof(Product) * (productsLength = INITIAL_PRODUCTS_CAPACITY));
	productCount = 0;

	if(products == NULL)
	{
		free(products);
		errno = -1;
	}
}

void destructProductsManager()
{
	for(int i = 0; i < productCount; i++)
	{
		destroyProduct(products[i]);

		free(products[i]);
		products[i] = NULL;
	}

	free(products);
	products = NULL;

	productCount = 0;
}

bool addProduct(Product product)
{
	if(productCount >= productsLength)
	{	
		products = realloc(products, sizeof(Product) * (productsLength += PRODUCTS_RESIZE_ADD_CAPACITY));

		if(products == NULL || errno != 0)
			return false;
	}

	Product *p = malloc(sizeof(Product));

	if(p == NULL || errno != 0)
	{
		free(p);
		return false;
	}

	copyProductTo(product, p);

	products[productCount++] = p;
	return true;
}

bool removeProduct(Product product)
{
	errno = 0;

	int productIndex = -1;

	for(int i = 0; i < productCount; i++)
	{
		if(productIndex >= 0)
		{
			products[i - 1] = products[i];
			continue;
		}
		
		if(!isProductsEquals(product, *products[i]))
			continue;

		productIndex = i;
		
		destroyProduct(products[i]);
		free(products[i]);
	}

	if(productIndex >= 0)
	{
		products[--productCount] = NULL;

		if(productsLength - productCount > PRODUCTS_RESIZE_ADD_CAPACITY)
		{
			products = realloc(products, sizeof(Product) * (productsLength -= PRODUCTS_RESIZE_ADD_CAPACITY));

			if(products == NULL || errno != 0)
				return false;
		}

		return true;
	}

	return false;
}

Product *searchProductByName(char *name)
{
	Product *result = NULL;

	for(int i = 0; i < productCount; i++)
	{
		if(strcmp(name, products[i]->name) != 0)
			continue;

		result = products[i];
		break;
	}

	return result;
}