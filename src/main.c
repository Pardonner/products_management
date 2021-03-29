#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <locale.h>

#include "./headers/product.h"
#include "./headers/memory.h"
#include "./headers/productsmanager.h"
#include "./headers/io.h"

void finishProgramOperation();
void seekProductInformationOperation();
void productEntryOperation();
void productRemoveOperation();
void newProductOperation();
void deleteProductOperation();

bool askForProduct(Product **);
bool askProductsName(char **);
bool readIntegerProperly(int *);
bool readAmountProperly(unsigned short *);

extern int errno;
extern Product **products;
extern unsigned int productCount;

bool running = false;

int main(int argc, char const *argv[])
{
	setlocale(LC_ALL, "Portuguese");

	initProductsManager();

	running = true;

	do
	{
		int option = -1;

		printf("Please insert:\n"
			"'0' to finish the program,\n"
			"'1' for product information,\n"
			"'2' for product entry,\n"
			"'3' for product remove,\n"
			"'4' for new product and\n"
			"'5' for product deletation.\n"
			"Option: ");

		if(!readIntegerProperly(&option))
			continue;

		switch(option)
		{
			case 0:
				finishProgramOperation();
				break;
			case 1:
				seekProductInformationOperation();
				break;
			case 2:
				productEntryOperation();
				break;
			case 3:
				productRemoveOperation();
				break;
			case 4:
				newProductOperation();
				break;
			case 5:
				deleteProductOperation();
				break;
			default:
				fprintf(stderr, "The operation '%d' is invalid!\n", option);
				break;
		}
	} while(running);

	destructProductsManager();

	puts("The program has ended!");
	return 0;
}

void finishProgramOperation()
{
	running = false;

	for(int i = 0; i < productCount; i++)
		printf("Product #%d: %s\n", i + 1, productToString(*products[i]));
}

void seekProductInformationOperation()
{
	Product *product;

	if(!askForProduct(&product))
		return;

	printf("Product: %s\n", productToString(*product));
}

void productEntryOperation()
{
	Product *product;
	unsigned short amount = 0;

	if(!askForProduct(&product) || !readAmountProperly(&amount))
		return;

	product->amount += amount;
	printf("Successfuly added an amount of %hu products!\n", amount);
}

void productRemoveOperation()
{
	Product *product;
	unsigned short amount = 0;
	char *removeDescription;

	if(!askForProduct(&product) || !readAmountProperly(&amount))
		return;

	if(product->amount - amount < 0)
	{
		fprintf(stderr, "Unsuficient inventory! Could not subtract %hu from %hu!\n", amount, product->amount);
		return;
	}

	puts("Please inform the reason for this remove:");
	removeDescription = readLine();

	if(removeDescription != NULL && errno == 0)
	{
		if(!addRemoveDescription(product, removeDescription))
		fprintf(stderr, "Could not register more removes to this product! "
			"Please consider giving this program more memory!\n");
		else
		{
			product->amount -= amount;
			printf("Successfuly removed an amount of %hu products!\n", amount);
			return;
		}
	}
	else
		fprintf(stderr, "Could not read the reason for this exit!"
			"Please consider giving this program more memory!\n");
	
	freePointer(&removeDescription);
}

void newProductOperation()
{
	Product result = createProduct();

	if(askProductsName(&result.name))
	{
		puts("Please insert the product's description:");
	
		result.description = readLine();
		
		if(result.description == NULL || errno != 0)
			fprintf(stderr, "Could not read the product's description!"
				" Please consider offering more memory to this program!\n");
		else
		{
			printf("Please insert the product's cost: ");

			result.cost = readDouble();

			if(errno != 0)
				switch(errno)
				{
					case 1:
					case 2:
						fprintf(stderr, "Invalid number!\n");
						break;
					default:
						fprintf(stderr, "Could not read the cost!"
							"Please consider offering more memory to this program!\n");
						break;
				}
			else if(readAmountProperly(&result.amount))
			{
				if(addProduct(result))
				{
					printf("Successfuly registered the product '%s'!\n", result.name);
					return;
				}
				else
					fprintf(stderr, "Could not register this product! "
						"Please consider giving this program more memory!\n");
			}
		}
	}

	destroyProduct(&result);
}

void deleteProductOperation()
{
	Product *product;

	if(!askForProduct(&product))
		return;

	if(removeProduct(*product))
		printf("Successfuly deleted this product!\n");
	else
		fprintf(stderr, "Could not delete this product!\n");
}

bool askForProduct(Product **product)
{
	char *name;
	bool successed = false;

	if(askProductsName(&name))
	{
		Product *result = searchProductByName(name);

		if(result == NULL)
			fprintf(stderr, "Could not find the product '%s'!\n", name);
		else
		{
			*product = result;
			successed = true;
		}
	}

	freePointer(&name);

	return successed;
}

bool askProductsName(char **container)
{
	char *name;

	puts("Please insert the product's name!");

	name = readLine();

	if(name == NULL || errno != 0)
	{
		fprintf(stderr, "Could not read the name passed! Please consider giving this program more memory!\n");

		free(name);
		return false;
	}

	*container = name;

	return true;
}

bool readIntegerProperly(int *container)
{
	*container = readInteger();

	switch(errno)
	{
		case 0:
			return true;
		case 1:
			fprintf(stderr, "Invalid integer number!\n");
			break;
		default:
			fprintf(stderr, "Could not read the number provided!\n");
			break;
	}

	return false;
}

bool readAmountProperly(unsigned short *container)
{
	int amount;

	printf("Please insert the amount: ");
	if(!readIntegerProperly(&amount))
		return false;

	if(amount < 1)
	{
		fprintf(stderr, "The amount should be greater than 0!\n");
		return false;
	}

	*container = (unsigned short) amount;
	return true;
}