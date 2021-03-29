#define INITIAL_REMOVES_CAPACITY 5
#define PRODUCT_RESIZE_ADD_CAPACITY 5

typedef struct product
{
	char *name;
	char *description;

	float cost;
	unsigned short amount;

	unsigned short _removesCount;
	unsigned int _removesCapacity;
	char **removesDescription;
} Product;

Product createProduct();

void initProduct(Product *);
void destroyProduct(Product *);

bool addRemoveDescription(Product *, char *);

void copyProductTo(Product, Product *);
char *productToString(Product);
bool isProductsEquals(Product, Product);