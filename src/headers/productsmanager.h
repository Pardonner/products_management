#define INITIAL_PRODUCTS_CAPACITY 5
#define PRODUCTS_RESIZE_ADD_CAPACITY 5

void initProductsManager();
void destructProductsManager();

bool addProduct(Product);
bool removeProduct(Product);

Product *searchProductByName(char *);