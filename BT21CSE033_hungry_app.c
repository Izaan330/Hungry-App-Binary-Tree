#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>

#define NUMBER_OF_FOOD_ITEMS_PER_RESTO 2
#define MAX_NO_OF_FOOD_ORDERS 7
#define MAX_NO_OF_FOOD_ITEMS 20
#define NUMBER_OF_FOOD_LOCATIONS 9
#define NUMBER_OF_USERS 10
#define NUMBER_OF_AGENTS 10
#define NUMBER_OF_ORDERS 5

int global_order_no = 1;
int global_user_order = 1;

struct order_input{
    char user_id[20];
    char food_loc[20];
    char dish[20];
    int price;
};

struct dish_input
{
    char dish[20];
    int price;
};

struct Food_loc_input{
    char name[20];
    char address[20];
    int seats;
    char outdoor_seating[20];
    char wifi[20];
    char category[20];
    char cuisine[20];
    char area[20];
    int cat_key;
    int cusi_key;
    int area_key;
};

struct user_input{
    char user_id[20];
    char name[20];
    char phone_no[20];
    char address[100];
};

struct agent_input{
    char agent_id[20];
    char name[20];
    char phone_no[20];
    char area[20];
    int accumulated_commision;
    int on_order;
};


typedef struct date{
    int day;
    int month;
    int year;
}Date;


// typedef struct dish{
//     char dish[NUMBER_OF_FOOD_ITEMS_PER_RESTO][20];
//     int price[NUMBER_OF_FOOD_ITEMS_PER_RESTO];
//     int fav_dish[NUMBER_OF_FOOD_ITEMS_PER_RESTO];
    
// }Dish;

// typedef struct dish_ll{
//     char dish[100];
//     int price;
//     int fav_dish;
//     Date first_order;
//     Date last_order;
//     struct dish_ll* right;
//     struct dish_ll* left;
//     int height;
// }Dish_LL;

// typedef struct food{
//     Dish dish;
//     Dish_LL dish_ll;
    
// }Food;

typedef struct Order{
    int order_no;
    char order[100];
    char agent_id[20];
    char user_id[20];
    char hotel[20];
    Date date;
    int price;
    int order_count;
    struct Order* right;
    struct Order* left;
    int height;
}Order;

typedef struct special_facilities{
    char outdoor_seating[10];//yes or no
    char wifi[10];
}Special_facilities;

typedef struct category_Node{
    char name[20];
    char address[100];
    char category[20];
    int cat_key;
    char cuisine[20];
    int cusi_key;
    char area[20];
    int area_key;
    int total_seats;
    int fav_food_location;
    int total_orders;
    Special_facilities special_facilities;
    //Food menu;
    struct category_Node* right;
    struct category_Node* left;
    int height;
}Category;



typedef struct delivery_agent{
    char agent_id[20];
    char name[20];
    char phone_no[20];
    char area[20];
    int accumulated_commision;
    int on_order;
    struct delivery_agent* right;
    struct delivery_agent* left;
    int height;
    //to keep account of all orders delivered by agent use ll
}Agent;

typedef struct resto_list{
    int order_count;
    Date date;
    char resto_name[20];
    struct resto_list* next;

}resto_list;

typedef struct order_list{
    char food_item[20];
    int food_item_count;
    Date date;
    struct order_list* next;
}order_list;

typedef struct food_list{
    char food_item[20];
    int food_item_count;
    Date date;
    struct food_list* next;
}food_list;

typedef struct user{
    char user_id[20];
    char name[20];
    char phone_no[20];
    char address[100];
    char order[50];
    //order_list* order_list;
    struct user* right;
    struct user* left;
    int height;
}User;

struct Food_loc_input table[NUMBER_OF_FOOD_LOCATIONS];


void food_location_input()
{
	FILE *fp1; // declare a file pointer
    char filename_1[] = "food_loc_input.txt"; // name of the input file


    fp1 = fopen(filename_1, "r");
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    } 

    // read the table data from the file
    for (int i = 0; i < NUMBER_OF_FOOD_LOCATIONS; i++) {
        fscanf(fp1, "%s %s %d %s %s %s %s %s %d %d %d", table[i].name, table[i].address, &table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].area,&table[i].cat_key,&table[i].cusi_key,&table[i].area_key);
    }

        
	 fclose(fp1);

	return;
}

int height_category(Category *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b)? a : b;
}

Category* newNode(char* name,char* address, int seats, char* outdoor_seating,char* wifi, char* category,char* cuisine,int cat_key,int cusi_key, int area_key)
{
	Category* node = (Category*)malloc(sizeof(Category));
	strcpy(node->name,name);
    strcpy(node->address,address);
    node->total_seats = seats;
    strcpy(node->special_facilities.outdoor_seating,outdoor_seating);
    strcpy(node->special_facilities.wifi,wifi);
    strcpy(node->category,category);
    strcpy(node->cuisine,cuisine);
    node->cat_key=cat_key;
    node->cusi_key=cusi_key;
    node->area_key=area_key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

Category *rightRotate(Category *y)
{
	Category *x = y->left;
	Category *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height_category(y->left),
					height_category(y->right)) + 1;
	x->height = max(height_category(x->left),
					height_category(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Category *leftRotate(Category *x)
{
	Category *y = x->right;
	Category *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height_category(x->left),
					height_category(x->right)) + 1;
	y->height = max(height_category(y->left),
					height_category(y->right)) + 1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(Category *N)
{
	if (N == NULL)
		return 0;
	return height_category(N->left) - height_category(N->right);
}

// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.

//FOR CAT_KEY
Category* insert_by_category(Category* node, char* name,char* address, int seats, char* outdoor_seating,char* wifi, char* category,char* cuisine,int cat_key,int cusi_key, int area_key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key));

	if (cat_key < node->cat_key)
		node->left = insert_by_category(node->left,name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else if (cat_key > node->cat_key)
		node->right = insert_by_category(node->right, name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_category(node->left),
						height_category(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && cat_key < node->left->cat_key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && cat_key > node->right->cat_key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && cat_key > node->left->cat_key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && cat_key < node->right->cat_key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

Category* make_cat_tree()
{
    Category* root=NULL;
    for(int i=0;i<NUMBER_OF_FOOD_LOCATIONS;i++)
    {
        root=insert_by_category(root,table[i].name,table[i].address,table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].cat_key,table[i].cusi_key,table[i].area_key);
        // printf("%s \n",table[i].name);//debugging
        // printf("%s \n",root->name);//debugging
    }
    return root;
}

Category* Search_by_cat_key(int cat_key, Category *root)
{
	Category *p = root;
	while(p != NULL && cat_key != p->cat_key)
	{
		if(cat_key < p->cat_key)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return p;
}

//FOR CUSI-KEY

Category* insert_by_cuisine(Category* node, char* name,char* address, int seats, char* outdoor_seating,char* wifi, char* category,char* cuisine,int cat_key,int cusi_key, int area_key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key));

	if (cusi_key < node->cusi_key)
		node->left = insert_by_cuisine(node->left,name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else if (cusi_key > node->cusi_key)
		node->right = insert_by_cuisine(node->right, name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_category(node->left),
						height_category(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && cusi_key < node->left->cusi_key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && cusi_key > node->right->cusi_key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && cusi_key > node->left->cusi_key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && cusi_key < node->right->cusi_key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

Category* make_cusi_tree()
{
    Category* root=NULL;
    for(int i=0;i<NUMBER_OF_FOOD_LOCATIONS;i++)
    {
        root=insert_by_cuisine(root,table[i].name,table[i].address,table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].cat_key,table[i].cusi_key,table[i].area_key);

    }
    return root;
}

Category* Search_by_cusi_key(int cusi_key, Category *root)
{
	Category *p = root;
	while(p != NULL && cusi_key != p->cusi_key)
	{
		if(cusi_key < p->cusi_key)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return p;
}

//FOR AREA_KEY

Category* insert_by_area(Category* node, char* name,char* address, int seats, char* outdoor_seating,char* wifi, char* category,char* cuisine,int cat_key,int cusi_key, int area_key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key));

	if (area_key < node->area_key)
		node->left = insert_by_area(node->left,name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else if (area_key > node->area_key)
		node->right = insert_by_area(node->right, name,address,seats,outdoor_seating,wifi,category,cuisine,cat_key,cusi_key,area_key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_category(node->left),
						height_category(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && area_key < node->left->area_key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && area_key > node->right->area_key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && area_key > node->left->area_key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && area_key < node->right->area_key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

Category* make_area_tree()
{
    Category* root=NULL;
    for(int i=0;i<NUMBER_OF_FOOD_LOCATIONS;i++)
    {
        root=insert_by_area(root,table[i].name,table[i].address,table[i].seats,table[i].outdoor_seating,table[i].wifi,table[i].category,table[i].cuisine,table[i].cat_key,table[i].cusi_key,table[i].area_key);

    }
    return root;
}  

Category* Search_by_area_key(int area_key, Category *root)
{
	Category *p = root;
	while(p != NULL && area_key != p->area_key)
	{
		if(area_key < p->area_key)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return p;
}

void print_restaurant_details(int area_key,Category* root)
{
    Category* p= Search_by_area_key(area_key,root);
    if(p==NULL)
    {
        printf("restaurant not found");
    }
    else
    {
        printf("restaurant name is %s \n",p->name);
        printf("restaurant address is %s \n",p->address);
    }
}

void Visit_by_category(Category * node)
{
    printf("Name of FOOD_LOCATION : %s \n",node->name);
     
        // printf("Address of FOOD_LOCATION : %s\n",node->address);
            
        printf("Total no of seats : %d \n",node->total_seats);
            
        printf("Outdoor seating : %s \n",node->special_facilities.outdoor_seating);
            
        printf("wifi : %s \n",node->special_facilities.wifi);

        printf("category : %s\n ",node->category);

        printf("cusine : %s\n",node->cuisine);

        printf("area : %s\n",node->area);

        printf("-----------------------------------\n");
}

void inorder_cat_traversal(Category* root)
{
    if(root!=NULL)
    {
        inorder_cat_traversal(root->left);
        Visit_by_category(root);
        inorder_cat_traversal(root->right);
    }
}

void print_food_location_list(Category* root)
{
    inorder_cat_traversal(root);
}

//-----------------------------
// See the diagram given above.


int height_user(User *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int getBalance_user(User *N)
{
	if (N == NULL)
		return 0;
	return height_user(N->left) - height_user(N->right);
}



User *rightRotate_user(User *y)
{
	User *x = y->left;
	User *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height_user(y->left),
					height_user(y->right)) + 1;
	x->height = max(height_user(x->left),
					height_user(x->right)) + 1;

	// Return new root
	return x;
}

User *leftRotate_user(User *x)
{
	User *y = x->right;
	User *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height_user(x->left),
					height_user(x->right)) + 1;
	y->height = max(height_user(y->left),
					height_user(y->right)) + 1;

	// Return new root
	return y;
}

// A utility function to left rotate subtree rooted with x

//food location input done , user input

struct user_input user_inp[NUMBER_OF_USERS];

void user_input(){


    FILE *fp1; // declare a file pointer
    char filename_1[] = "user_input.txt"; // name of the input file

    fp1 = fopen(filename_1, "r");
   
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    }



     // read the table data from the file
    for (int i = 0; i < NUMBER_OF_USERS; i++) 
    {
         fscanf(fp1, "%s %s %s %s", user_inp[i].user_id,user_inp[i].name,user_inp[i].phone_no,user_inp[i].address);
    }

    fclose(fp1);
}

User* new_user_node(char* user_id,char* name,char* phone_no, char* address)
{
	User* node = (User*)malloc(sizeof(User));
	strcpy(node->name,name);
    strcpy(node->address,address);
    strcpy(node->user_id,user_id);
    strcpy(node->phone_no,phone_no);
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

User* insert_user_node(User * node,char* user_id,char* name,char* phone_no, char* address)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(new_user_node(user_id,name,phone_no,address));

	if (strcmp(user_id,node->user_id)<0)
		node->left = insert_user_node(node->left,user_id,name,phone_no,address);
	else if (strcmp(user_id,node->user_id)>0)
		node->right = insert_user_node(node->right,user_id,name,phone_no,address);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_user(node->left),
						height_user(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance_user(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && (strcmp(user_id,node->left->user_id)<0))
		return rightRotate_user(node);

	// Right Right Case
	if (balance < -1 && (strcmp(user_id,node->right->user_id)>0))
		return leftRotate_user(node);

	// Left Right Case
	if (balance > 1 && (strcmp(user_id,node->left->user_id)>0))
	{
		node->left = leftRotate_user(node->left);
		return rightRotate_user(node);
	}

	// Right Left Case
	if (balance < -1 && (strcmp(user_id,node->right->user_id)<0))
	{
		node->right = rightRotate_user(node->right);
		return leftRotate_user(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

//should i make the below given function of return type user?

User* make_user_tree()
{
    User* root = NULL;
    for(int i=0;i< NUMBER_OF_USERS; i++ )
    {
        root=insert_user_node(root,user_inp[i].user_id,user_inp[i].name,user_inp[i].phone_no,user_inp[i].address);
    }
    return root;
}

void Visit_by_user(User* user)
{
        printf("User id : %s \n",user->user_id);
		printf("Name of user : %s \n",user->name);
     
        printf("Address of user : %s\n",user->address);
            
        printf("Phone no : %s \n",user->phone_no);
            
       
        printf("---------------------------------------\n");

}

void inorder_user_traversal(User* root)
{
    if(root!=NULL)
    {
        inorder_user_traversal(root->left);
        Visit_by_user(root);
        inorder_user_traversal(root->right);
    }
}

void print_user_list(User* root)
{
    inorder_user_traversal(root);
}
//merge sort for linked list for user


int diff_of_days(Date date_of_order,Date date_today)
{
    Date diff;
    if(date_today.day < date_of_order.day){
        diff.day = date_today.day+30 - date_of_order.day;
        date_today.month = date_today.month - 1;
    }
    else diff.day = date_today.day - date_of_order.day;

    if(date_today.month < date_of_order.month){
        diff.month = date_today.month+12 - date_of_order.month;
        date_today.year = date_today.year - 1;
    }
    else diff.month = date_today.month - date_of_order.month;

    diff.year = date_today.year - date_of_order.year;

    return (diff.day + diff.month*30 + diff.year*365);

}


order_list* SortedMerge(order_list* a,order_list* b)
{
	order_list* result = NULL;

	// Base cases
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	// Pick either a or b, and recur
	if (a->food_item_count <= b->food_item_count)
	{
		result = a;
		result->next =SortedMerge(a->next, b);
	}
	else
	{
		result = b;

		result->next = SortedMerge(a, b->next);
	}
	return (result);
}

// UTILITY FUNCTIONS
/* Split the nodes of the given list into
front and back halves, and return the
two lists using the reference parameters.
If the length is odd, the extra node should
go in the front list.Uses the fast/slow
pointer strategy. */
void FrontBackSplit(order_list* source,order_list** frontRef,order_list** backRef)
{
	order_list* fast;
	order_list* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and
	advance 'slow' one node */
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the
		list, so split it in two at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

void MergeSort(order_list** headRef)
{
	order_list* head = *headRef;
	order_list* a;
	order_list* b;

	// Base case -- length 0 or 1
	if ((head == NULL) ||
		(head->next == NULL))
	{
		return;
	}

	// Split head into 'a' and 'b' sublists
	FrontBackSplit(head, &a, &b);

	// Recursively sort the sublists
	MergeSort(&a);
	MergeSort(&b);

	/* answer = merge the two sorted
	lists together */
	*headRef = SortedMerge(a, b);
}



User* search_user(char* user_id,User* root)
{
    User *p = root;
	while(p != NULL && strcmp(user_id,p->user_id))
	{
        if(strcmp(user_id,p->user_id)<0)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return p;
}



void push_node_order(order_list** dummy_list,char* food_item,Date date)//correct code
{
	// Allocate node
	order_list* new_node= (order_list*)malloc(sizeof(order_list));

	// Put in the data
	strcpy(new_node->food_item,food_item);
    new_node->food_item_count=1;
    new_node->date=date;

    //printf("food item added to dummy_list is %s\n",new_node->food_item);//debugging

	// Link the old list of the new node
	new_node->next = (*dummy_list);

	// Move the head to point to the new node
	(*dummy_list) = new_node;
}

/* Given a reference (pointer to pointer) to the head
of a list and an int, appends a new node at the end */
void append(order_list** head_ref,char* food_item,Date date)
{
	/* 1. allocate node */
	order_list* new_node = (order_list*) malloc(sizeof(order_list));

	order_list *last = *head_ref; /* used in step 5*/

	/* 2. put in the data */
	//new_node->data = new_data;

    strcpy(new_node->food_item,food_item);
    new_node->food_item_count=1;
    new_node->date=date;


	/* 3. This new node is going to be the last node, so make next
		of it as NULL*/
	new_node->next = NULL;

	/* 4. If the Linked List is empty, then make the new node as head */
	if (*head_ref == NULL)
	{
	*head_ref = new_node;
	return;
	}
	
	/* 5. Else traverse till the last node */
	while (last->next != NULL)
		last = last->next;

	/* 6. Change the next of last node */
	last->next = new_node;
	return;	
}


void update_dummy_ll(order_list* dummy_list,char* food_item,Date date)
{
    order_list*dummy_list_copy=dummy_list;
   // printf("inside update_dummy_ll function");//debugging
    int found_copy_of_food_item=0;
    while(dummy_list_copy!=NULL)
    {
    //    printf("inside while loop \n");//debugging
        if(strcmp(dummy_list_copy->food_item,food_item)==0)
        {
            // printf("food item is %s\n",food_item);//debugging
            dummy_list_copy->food_item_count++;
            // printf("food item count  is %d\n",dummy_list_copy->food_item_count);//debugging
            found_copy_of_food_item=1;
        }
        dummy_list_copy=dummy_list_copy->next;
    }
    if(found_copy_of_food_item==0)
    {
        // printf("inside push node\n");//debugging
        // push_node_order(&dummy_list,food_item,date);
        append(&dummy_list,food_item,date);
    //printf("%s\n",dummy_list->food_item);//debugging
    }

}

void visit_order_node(Order* root,char* user_id,Date date,order_list* dummy_list)
{
    if(strcmp(user_id,root->user_id)==0 && diff_of_days(root->date,date)<=90)
    {
        update_dummy_ll(dummy_list,root->order,date);
    }

}


// void traverse_order_tree(Order* root,char* user_id,Date date,order_list* dummy_list)
// {
//     if(root!=NULL)
//     {
//         traverse_order_tree(root->left,user_id,date,dummy_list);
        
//         if(strcmp(user_id,root->user_id)==0 && diff_of_days(root->date,date)<=90)
//     {
//         int found_copy_of_food_item=0;
//     while(dummy_list!=NULL)
//     {
//         printf("inside while loop \n");//debugging
//         if(strcmp(dummy_list->food_item,root->order)==0)
//         {
//             printf("food item is %s\n",root->order);//debugging
//             dummy_list->food_item_count++;
//             printf("food item count  is %d\n",dummy_list->food_item_count);//debugging
//             found_copy_of_food_item=1;
//         }
//         dummy_list=dummy_list->next;
//     }
//     if(found_copy_of_food_item==0)
//     {
//         printf("inside push node\n");//debugging
//         push_node_order(&dummy_list,root->order,date);
//     printf("%s\n",dummy_list->food_item);//debugging
//     }
//     }


//         traverse_order_tree(root->right,user_id,date,dummy_list);
//     }
// }


// void traverse_order_tree(Order* root,char* user_id,Date date,order_list* dummy_list)
// {
//     if(root!=NULL)
//     {
//         printf("checkpoint 1 \n");//debugging
//         traverse_order_tree(root->left,user_id,date,dummy_list);
//         printf("checkpoint 2 \n");//debugging
//         visit_order_node(root,user_id,date,dummy_list);//for the sake of debugging i introduced the snippet 
//         printf("checkpoint 3 \n");//debugging
//         traverse_order_tree(root->right,user_id,date,dummy_list);
//         printf("checkpoint 4 \n");//debugging
        
//     }
// }


// void find_fav_food_of_user(char*  user_id, User* root_u,Order* root)
// {
//     Date date;
//     date.day=11;
//     date.month=4;
//     date.year=2023;
//     User* u_node=search_user(user_id,root_u);
//     order_list* dummy_list=NULL;

// //traverse_order_tree(root,u_node->user_id,date,dummy_list);
// // printf("%s \n",dummy_list->food_item);//debugging
//     MergeSort(&dummy_list);

//     order_list* dummy1=dummy_list;
//     order_list* dummy2=dummy_list;
//     order_list* dummy3=dummy_list;
//     // while(dummy1->next!=NULL)
//     // {
//     //     dummy1=dummy1->next;
//     // }
//     // printf("The best food is %s",dummy1->food_item);
//     // while(dummy2->next->next!=NULL)
//     // {
//     //     dummy2=dummy2->next;
//     // }
//     // printf("The second best food is %s",dummy2->food_item);
//     // while (dummy3->next->next->next!=NULL)
//     // {
//     //     dummy3=dummy3->next;
//     // }
//     // printf("The third best food is %s",dummy3->food_item);//temporary
   
// //--------------------------
//    // printf("the favourite foods of user are:\n");
//    // printf("%s \n",dummy_list->food_item);
//    // printf("%s \n",dummy_list->next->food_item);debugging
//     //printf("%s \n",dummy_list->next->next->food_item);debugging
    
// }

void flatten(Order* root)
{
	// base condition- return if root is NULL or if it is a
	// leaf node
	if (root == NULL || root->left == NULL && root->right == NULL)
		return;

	// if root->left exists then we have to make it
	// root->right
	if (root->left != NULL) {
		// move left recursively
		flatten(root->left);
		// store the node root->right
		Order* tmpRight = root->right;
		root->right = root->left;
		root->left = NULL;
		// find the position to insert the stored value
		Order* t = root->right;
		while (t->right != NULL)
			t = t->right;
		// insert the stored value
		t->right = tmpRight;
	}

	// now call the same function for root->right
	flatten(root->right);
}

void find_fav_food_of_user(Order* root,char* user_id,Date date,order_list* dummy_list)
{
    flatten(root);
    // printf("%s\n",root->order);//debugging

    // visit_order_node(root,user_id,date,dummy_list);
    // printf("%s \n", dummy_list->next->food_item);
    // visit_order_node(root->right,user_id,date,dummy_list);
    


    while(root!=NULL)
    {
        visit_order_node(root,user_id,date,dummy_list);
        root=root->right;
    }
    MergeSort(&dummy_list);

    order_list* dummy_list_cpy=dummy_list;
    order_list* dummy_list_cpy_1=dummy_list;


    while(dummy_list_cpy->next!=NULL)
    {
        dummy_list_cpy_1=dummy_list_cpy;
        dummy_list_cpy=dummy_list_cpy->next;
    }


    printf("The most favourite food of the user is %s \n",dummy_list_cpy->food_item);
    printf("The second most favourite food of the user is %s \n",dummy_list_cpy_1->food_item);
    
}


//--------------------------------------------------------------


int height_agent(Agent *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}



//user input done now we will do agent_input

Agent *rightRotate_agent(Agent *y)
{
	Agent *x = y->left;
	Agent *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height_agent(y->left),
					height_agent(y->right)) + 1;
	x->height = max(height_agent(x->left),
					height_agent(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Agent *leftRotate_agent(Agent *x)
{
	Agent *y = x->right;
	Agent *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height_agent(x->left),
					height_agent(x->right)) + 1;
	y->height = max(height_agent(y->left),
					height_agent(y->right)) + 1;

	// Return new root
	return y;
}

int getBalance_agent(Agent *N)
{
	if (N == NULL)
		return 0;
	return height_agent(N->left) - height_agent(N->right);
}
//user input done , agent input

struct agent_input agent_inp[NUMBER_OF_AGENTS];

void agent_input()
{
    FILE *fp1; // declare a file pointer
    char filename_1[] = "agent_input.txt"; // name of the input file



    fp1 = fopen(filename_1, "r");
   
    
    // check if the file was opened successfully
    if (fp1 == NULL) {
        printf("Failed to open file %s\n", filename_1);
        return ;
    }


    // read the table data from the file
        for (int i = 0; i < NUMBER_OF_AGENTS; i++) {
            fscanf(fp1, "%s %s %s %s %d %d", agent_inp[i].agent_id,agent_inp[i].name,agent_inp[i].phone_no,agent_inp[i].area,&agent_inp[i].accumulated_commision,&agent_inp[i].on_order);
            // printf("%s", agent_inp[i].name);//debugging
        }
        
    fclose(fp1);
}

Agent* new_agent_node(char* agent_id,char* name,char* phone_no, char* area,int cac,int on_order)
{
	Agent* node = (Agent*)malloc(sizeof(Agent));
	strcpy(node->name,name);
    strcpy(node->area,area);
    strcpy(node->agent_id,agent_id);
    strcpy(node->phone_no,phone_no);
    node->accumulated_commision=cac;
    node->on_order=on_order;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

Agent* insert_agent_node(Agent * node,char* agent_id,char* name,char* phone_no, char* area,int cac,int on_order)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(new_agent_node(agent_id,name,phone_no,area,cac,on_order));

	if (strcmp(agent_id,node->agent_id)<0)
		node->left = insert_agent_node(node->left,agent_id,name,phone_no,area,cac,on_order);
	else if (strcmp(agent_id,node->agent_id)>0)
		node->right = insert_agent_node(node->right,agent_id,name,phone_no,area,cac,on_order);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_agent(node->left),
						height_agent(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance_agent(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && (strcmp(agent_id,node->left->agent_id)<0))
		return rightRotate_agent(node);

	// Right Right Case
	if (balance < -1 && (strcmp(agent_id,node->right->agent_id)>0))
		return leftRotate_agent(node);

	// Left Right Case
	if (balance > 1 && (strcmp(agent_id,node->left->agent_id)>0))
	{
		node->left = leftRotate_agent(node->left);
		return rightRotate_agent(node);
	}

	// Right Left Case
	if (balance < -1 && (strcmp(agent_id,node->right->agent_id)<0))
	{
		node->right = rightRotate_agent(node->right);
		return leftRotate_agent(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

//should i make the below given function of return type user?

Agent* make_agent_tree()
{
    Agent* root = NULL;
    for(int i=0;i< NUMBER_OF_AGENTS; i++ )
    {
        root=insert_agent_node(root,agent_inp[i].agent_id,agent_inp[i].name,agent_inp[i].phone_no,agent_inp[i].area,agent_inp[i].accumulated_commision,agent_inp[i].on_order);
       // printf("%s \n",root->name);//debugging
    }
    return root;
}

void Visit_by_agent(Agent* agent)
{
     printf("Agent id : %s \n",agent->agent_id);
		printf("Name of Agent : %s \n",agent->name);
     
        printf("Phone no : %s\n",agent->phone_no);
            
        printf("accumulated commision : %d \n",agent->accumulated_commision);
            
        printf("Phone no : %s \n",agent->phone_no);
            
       
        printf("---------------------------------------\n");

}

void inorder_agent_traversal(Agent* root)
{
    if(root!=NULL)
    {
        inorder_agent_traversal(root->left);
        Visit_by_agent(root);
        inorder_agent_traversal(root->right);
    }
}

void print_agent_list(Agent* root)
{
    inorder_agent_traversal(root);
}

//traversing through the tree and strcmp with all of them

void check_agent_area(Agent* node,char* area)
{
    if(!strcmp(node->area,area))
    {
        printf("Agent id is %s . \n Agent name is %s .\n -------------------------\n",node->agent_id,node->name);
    }
}

void inorder_agent_traversal_for_area(Agent* root,char* area)
{
    if(root!=NULL)
    {
        inorder_agent_traversal_for_area(root->left,area);
        check_agent_area(root,area);
        inorder_agent_traversal_for_area(root->right,area);
    }
}

void print_area_wise_agent(Agent* root,char * area)
{
    inorder_agent_traversal_for_area(root,area);
}

Agent* search_agent(char* agent_id,Agent* root)
{
    Agent *p = root;
	while(p != NULL && strcmp(agent_id,p->agent_id))
	{
        if(strcmp(agent_id,p->agent_id)<0)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return p;
}


//--------------------------------------------------------------

//-------------------------------------------------------------

int height_order(Order *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

Order *rightRotate_order(Order *y)
{
	Order *x = y->left;
	Order *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height_order(y->left),
					height_order(y->right)) + 1;
	x->height = max(height_order(x->left),
					height_order(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Order *leftRotate_order(Order *x)
{
	Order *y = x->right;
	Order *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height_order(x->left),
					height_order(x->right)) + 1;
	y->height = max(height_order(y->left),
					height_order(y->right)) + 1;

	// Return new root
	return y;
}


int getBalance_order(Order *N)
{
	if (N == NULL)
		return 0;
	return height_order(N->left) - height_order(N->right);
}



Order* new_order_node( char order[100],int price,Date date,char agent_id[20],char user_id[20],char hotel[20])
{
    Order* new_order
		= (Order*)malloc(sizeof(Order));

	// Order* last = *head_ref; /* used in step 5*/

	/* 2. put in the data */
	strcpy(new_order->order,order);
    strcpy(new_order->agent_id,agent_id);
    strcpy(new_order->user_id,user_id);
    strcpy(new_order->hotel,hotel);

    new_order->order_no = global_order_no;
    new_order->price = price;
    new_order->order_count=1;
    new_order->left=new_order->right=NULL;
    new_order->height=1;

    new_order->date.day = date.day;
    new_order->date.month = date.month;
    new_order->date.year = date.year;

//    printf("%s \n",new_order->order);//debugging

    return new_order;

    //global_order_no++;
}


Order* insert_order_node(Order * node,int order_no, char order[100],int price,Date date,char agent_id[20],char user_id[20],char hotel[20])
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(new_order_node(order,price,date,agent_id,user_id,hotel));

	if (order_no< node->order_no)
		node->left = insert_order_node(node->left,order_no,order,price,date,agent_id,user_id,hotel);
	else if (order_no> node->order_no)
		node->right = insert_order_node(node->right,order_no,order,price,date,agent_id,user_id,hotel);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height_order(node->left),
						height_order(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance_order(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && ((order_no<node->left->order_no)))
		return rightRotate_order(node);

	// Right Right Case
	if (balance < -1 && ((order_no>node->right->order_no)))
		return leftRotate_order(node);

	// Left Right Case
	if (balance > 1 && ((order_no>node->left->order_no)))
	{
		node->left = leftRotate_order(node->left);
		return rightRotate_order(node);
	}

	// Right Left Case
	if (balance < -1 && ((order_no<node->right->order_no)))
	{
		node->right = rightRotate_order(node->right);
		return leftRotate_order(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

Order* place_order(Order*  root,char* agent_id_for_searching,Agent* root_a,char* user_id_for_searching,User* root_u,char* resto_name,int order_no, char order[100],int price,Date date,char agent_id[20],char user_id[20],char hotel[20])

{
    //Order* root;
    Agent* node_a;
    node_a=search_agent(agent_id_for_searching,root_a);

    if(node_a->on_order==0)
    {
        node_a->on_order=1;
        User* user_node = search_user(user_id_for_searching,root_u);
        root=insert_order_node(root,order_no,order,price,date,agent_id,user_id,hotel);
       // strcpy(user_node->order,order);
        printf("Order number  : %d\n",global_order_no);
        printf("Restaurant : %s\n",resto_name);
        printf("User id  : %s\n",user_node->user_id );
        printf("Agent id : %s\n",node_a->agent_id);
        printf("------------------------------\n");

    global_order_no++;
    }
    else
    {
        printf("Order cant be placed as agent was not assigned. \n");
    }

    return root;
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
resto_list* SortedMerge_resto(resto_list* a,resto_list* b)
{
	resto_list* result = NULL;

	// Base cases
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	// Pick either a or b, and recur
	if (a->order_count <= b->order_count)
	{
		result = a;
		result->next =SortedMerge_resto(a->next, b);
	}
	else
	{
		result = b;

		result->next = SortedMerge_resto(a, b->next);
	}
	return (result);
}

// UTILITY FUNCTIONS
/* Split the nodes of the given list into
front and back halves, and return the
two lists using the reference parameters.
If the length is odd, the extra node should
go in the front list.Uses the fast/slow
pointer strategy. */
void FrontBackSplit_resto(resto_list* source,resto_list** frontRef,resto_list** backRef)
{
	resto_list* fast;
	resto_list* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and
	advance 'slow' one node */
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the
		list, so split it in two at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

void MergeSort_resto(resto_list** headRef)
{
	resto_list* head = *headRef;
	resto_list* a;
	resto_list* b;

	// Base case -- length 0 or 1
	if ((head == NULL) ||
		(head->next == NULL))
	{
		return;
	}

	// Split head into 'a' and 'b' sublists
	FrontBackSplit_resto(head, &a, &b);

	// Recursively sort the sublists
	MergeSort_resto(&a);
	MergeSort_resto(&b);

	/* answer = merge the two sorted
	lists together */
	*headRef = SortedMerge_resto(a, b);
}


void push_node_resto(resto_list** dummy_list,char* resto_name,Date date)
{
	// Allocate node
	resto_list* new_node= (resto_list*)malloc(sizeof(resto_list));

	// Put in the data
	strcpy(new_node->resto_name,resto_name);
    new_node->order_count=1;
    new_node->date=date;

	// Link the old list of the new node
	new_node->next = (*dummy_list);

	// Move the head to point to the new node
	(*dummy_list) = new_node;
}

void append_resto(resto_list** head_ref, char* resto_name,Date date)
{
    printf("inside append\n");
	/* 1. allocate node */
	resto_list* new_node = (resto_list*) malloc(sizeof(resto_list));

	resto_list *last = *head_ref; /* used in step 5*/

	/* 2. put in the data */
	//new_node->data = new_data;
    strcpy(new_node->resto_name,resto_name);
    new_node->order_count=1;
    new_node->date=date;


	/* 3. This new node is going to be the last node, so make next
		of it as NULL*/
	new_node->next = NULL;

	/* 4. If the Linked List is empty, then make the new node as head */
	if (*head_ref == NULL)
	{
	*head_ref = new_node;
	return;
	}
	
	/* 5. Else traverse till the last node */
	while (last->next != NULL)
		last = last->next;

	/* 6. Change the next of last node */
	last->next = new_node;
	return;	
}



//void update_dummy_ll(order_list* dummy_list,char* food_item,Date date)
void update_dummy_resto_list(resto_list* dummy_list,Order* root)
{
    printf("inside update dummy resto list\n");
    int found_copy_of_resto=0;
    while(dummy_list!=NULL)
    {
        printf("inside update dummy resto list and in while\n");

        if(strcmp(dummy_list->resto_name,root->hotel)==0)
        {
            printf("the resto name is %s\n",dummy_list->resto_name);
            dummy_list->order_count++;
            printf("order count is %d\n",dummy_list->order_count);
            found_copy_of_resto=1;
        }
        dummy_list=dummy_list->next;
    }
    if(found_copy_of_resto==0)
    {
        printf("inside condition of if then do append\n");
        //push_node_resto(&dummy_list,root->hotel,root->date);
        append_resto(&dummy_list,root->hotel,root->date);
    }

}

// void visit_order_node(Order* node,char* user_id,Date date,order_list* dummy_list)
//{
    // if(strcmp(user_id,node->user_id)==0 && diff_of_days(node->date,date)<=90)
    // {
    //     update_dummy_ll(dummy_list,node->order,date);
    // }

//}
void visit_for_restaurant(Order* root,Date current_date,int N,resto_list* dummy_list)
{
    printf("inside visit for restaurant function\n");
    if(diff_of_days(root->date,current_date)<=N)
    {
        printf("inside visit for restaurant function and diff of days condition satisfied\n");
        update_dummy_resto_list(dummy_list,root);
    }
}


// void traverse_order_tree(Order* root,char* user_id,Date date,order_list* dummy_list)
// {
//     if(root!=NULL)
//     {
//         traverse_order_tree(root->left,user_id,date,dummy_list);
//         visit_order_node(root,user_id,date,dummy_list);
//         traverse_order_tree(root->right,user_id,date,dummy_list);
//     }
// }
void traverse_order_for_restaurant(Order* root,Date current_date,int N,resto_list*dummy_list)
{   
    printf("inside traverse\n");//debugging
    if(root!=NULL)
    {
        printf("inside traverse and root!=NULL");//debugging

        traverse_order_for_restaurant(root->left,current_date,N,dummy_list);
        visit_for_restaurant(root,current_date,N,dummy_list);
        traverse_order_for_restaurant(root->right,current_date,N,dummy_list);
    }
}


void find_fav_restaurant(User* root_u,Order* root,int N)
{
    Date date;
    date.day=11;
    date.month=4;
    date.year=2023;
    // User* u_node=search_user(user_id,root_u);
      resto_list* dummy_list=NULL;
    traverse_order_for_restaurant(root,date,N,dummy_list);

    // MergeSort_resto(&dummy_list);
    // resto_list* dummy1=dummy_list;
    // resto_list* dummy2=dummy_list;
    // resto_list* dummy3=dummy_list;

    
    // while(dummy1->next!=NULL)
    // {
    //     dummy1=dummy1->next;
    // }
    // printf("The best restaurant is %s",dummy1->resto_name);
    // while(dummy2->next->next!=NULL)
    // {
    //     dummy2=dummy2->next;
    // }
    // printf("The second best restaurant is %s",dummy2->resto_name);
    // while (dummy3->next->next->next!=NULL)
    // {
    //     dummy3=dummy3->next;
    // }
    // printf("The third best restaurant is %s",dummy3->resto_name);
    
    
    // // printf("the favourite foods of user are:\n");
    // // printf("%s \n",dummy_list->food_item);
    // // printf("%s \n",dummy_list->next->food_item);
    // // printf("%s \n",dummy_list->next->next->food_item);
    
}


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
 Order * minValueNode( Order* node)
{
	 Order* current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
 Order* delete_order_Node( Order* root, int order_no)
{
	// STEP 1: PERFORM STANDARD BST DELETE

	if (root == NULL)
		return root;

	// If the order_no to be deleted is smaller than the
	// root's order_no, then it lies in left subtree
	if ( order_no < root->order_no )
		root->left = delete_order_Node(root->left, order_no);

	// If the order_no to be deleted is greater than the
	// root's order_no, then it lies in right subtree
	else if( order_no > root->order_no )
		root->right = delete_order_Node(root->right, order_no);

	// if order_no is same as root's order_no, then This is
	// the node to be deleted
	else
	{
		// node with only one child or no child
		if( (root->left == NULL) || (root->right == NULL) )
		{
			 Order *temp = root->left ? root->left :
											root->right;

			// No child case
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
			*root = *temp; // Copy the contents of
							// the non-empty child
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			 Order* temp = minValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->order_no = temp->order_no;

			// Delete the inorder successor
			root->right = delete_order_Node(root->right, temp->order_no);
		}
	}

	// If the tree had only one node then return
	if (root == NULL)
	return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + max(height_order(root->left),
						height_order(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
	// check whether this node became unbalanced)
	int balance = getBalance_order(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && getBalance_order(root->left) >= 0)
		return rightRotate_order(root);

	// Left Right Case
	if (balance > 1 && getBalance_order(root->left) < 0)
	{
		root->left = leftRotate_order(root->left);
		return rightRotate_order(root);
	}

	// Right Right Case
	if (balance < -1 && getBalance_order(root->right) <= 0)
		return leftRotate_order(root);

	// Right Left Case
	if (balance < -1 && getBalance_order(root->right) > 0)
	{
		root->right = rightRotate_order(root->right);
		return leftRotate_order(root);
	}

	return root;
}

Order* delivery(Order* root,int order_no,char* agent_id_for_searching,Agent* root_a,int price)
{
    Order* new_root;   
    Agent* node_a;
    node_a=search_agent(agent_id_for_searching,root_a);

    node_a->on_order=0;
    printf("Agent has been freed.\n");
    printf("Accumulated commmision %d \n",node_a->accumulated_commision);
    node_a->accumulated_commision += price/10;
    printf("Final Accumulated commmision %d \n",node_a->accumulated_commision);
    printf("Free Agent : %s \n",node_a->agent_id);

    printf("-------------------------------");
    new_root=delete_order_Node(root,order_no);


    return new_root;//so that i can traverse easily the next time
}

Order* cancel_order(Order* root,int order_no,char* agent_id_for_searching,Agent* root_a)
{
    Order* new_root;   
    Agent* node_a;
    node_a=search_agent(agent_id_for_searching,root_a);

    node_a->on_order=0;
    printf("Agent has been freed.");

    printf("Free Agent : %s \n",node_a->agent_id);

    new_root=delete_order_Node(root,order_no);


    return new_root;//so that i can traverse easily the next time
}

//-----------------------------------------------------------------------


food_list* SortedMerge_for_food(food_list* a,food_list* b)
{
	food_list* result = NULL;

	// Base cases
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	// Pick either a or b, and recur
	if (a->food_item_count <= b->food_item_count)
	{
		result = a;
		result->next =SortedMerge_for_food(a->next, b);
	}
	else
	{
		result = b;

		result->next = SortedMerge_for_food(a, b->next);
	}
	return (result);
}

// UTILITY FUNCTIONS
/* Split the nodes of the given list into
front and back halves, and return the
two lists using the reference parameters.
If the length is odd, the extra node should
go in the front list.Uses the fast/slow
pointer strategy. */
void FrontBackSplit_for_food(food_list* source,food_list** frontRef,food_list** backRef)
{
	food_list* fast;
	food_list* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and
	advance 'slow' one node */
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the
		list, so split it in two at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

void MergeSort_for_food(food_list** headRef)
{
	food_list* head = *headRef;
	food_list* a;
	food_list* b;

	// Base case -- length 0 or 1
	if ((head == NULL) ||
		(head->next == NULL))
	{
		return;
	}

	// Split head into 'a' and 'b' sublists
	FrontBackSplit_for_food(head, &a, &b);

	// Recursively sort the sublists
	MergeSort_for_food(&a);
	MergeSort_for_food(&b);

	/* answer = merge the two sorted
	lists together */
	*headRef = SortedMerge_for_food(a, b);
}



void push_node_food(food_list** dummy_list,char* food_item,Date date)
{
	// Allocate node
	food_list* new_node= (food_list*)malloc(sizeof(food_list));

	// Put in the data
	strcpy(new_node->food_item,food_item);
    new_node->food_item_count=1;
    new_node->date=date;

	// Link the old list of the new node
	new_node->next = (*dummy_list);

	// Move the head to point to the new node
	(*dummy_list) = new_node;
}


void update_dummy_food_ll(food_list* dummy_list,char* food_item,Date date)
{
    int found_copy_of_food_item=0;
    while(dummy_list!=NULL)
    {
        if(strcmp(dummy_list->food_item,food_item)==0)
        {
            dummy_list->food_item_count++;
            found_copy_of_food_item=1;
        }
        dummy_list=dummy_list->next;
    }
    if(found_copy_of_food_item==0)
    {
        push_node_food(&dummy_list,food_item,date);
    }

}

void visit_order_node_for_food(Order* node,Date date,food_list* dummy_list,int N)
{
    if(diff_of_days(node->date,date)<=N)
    {
        update_dummy_food_ll(dummy_list,node->order,date);
    }

}

void traverse_order_for_fav_food(Order* root,Date date,food_list* dummy_list,int N)
{
    if(root!=NULL)
    {
        traverse_order_for_fav_food(root->left,date,dummy_list,N);
        visit_order_node_for_food(root,date, dummy_list ,N);
        traverse_order_for_fav_food(root->right,date,dummy_list,N);
    }
}


void find_fav_food_across_restaurant(int N,Order* root)
{
    Date date;
    date.day=11;
    date.month=4;
    date.year=2023;
    //User* u_node=search_user(user_id,root_u);
    food_list* dummy_list=NULL;

    traverse_order_for_fav_food(root,date,dummy_list,N);

    MergeSort_for_food(&dummy_list);

    food_list* dummy1=dummy_list;
    food_list* dummy2=dummy_list;
    food_list* dummy3=dummy_list;
    while(dummy1->next!=NULL)
    {
        dummy1=dummy1->next;
    }
    printf("The best food is %s",dummy1->food_item);
    while(dummy2->next->next!=NULL)
    {
        dummy2=dummy2->next;
    }
    printf("The second best food is %s",dummy2->food_item);
    while (dummy3->next->next->next!=NULL)
    {
        dummy3=dummy3->next;
    }
    printf("The third best food is %s",dummy3->food_item);
}

//-----------------------------------------------------------------------

void Visit_by_order(Order* order)
{
    printf("Order no : %d\n",order->order_no);
    printf("Order : %s\n",order->order);
    printf("Order price : %d\n",order->price);
    printf("Order date : %d:%d:%d\n",order->date.day,order->date.month,order->date.year);
    printf("Order count : %d \n",order->order_count);
    printf(" %s\n",order->hotel);

    printf("---------------------------------------\n");

}

void inorder_order_traversal(Order* root)
{
    if(root!=NULL)
    {
        inorder_order_traversal(root->left);
        Visit_by_order(root);
        inorder_order_traversal(root->right);
    }
}


void print_global_order_list(Order* root)
{
    inorder_order_traversal(root);
}

void check_in_range(User* node,char* user_id_1,char* user_id_2)
{
    if(strcmp(node->user_id,user_id_1)>=0 && strcmp(node->user_id,user_id_2)<=0)
    {
        printf("User_id: %s have %s as an order.",node->user_id,node->order);
    }
}

void range_traversal(User* root,char* user_id_1,char* user_id_2)
{
    if(root!=NULL)
    {
        range_traversal(root->left,user_id_1,user_id_2);
        check_in_range(root,user_id_1,user_id_2);
        range_traversal(root->right,user_id_1,user_id_2);
    }
}

void range_search(char* user_id_1,char* user_id_2,User* root)
{
    range_traversal(root,user_id_1,user_id_2);
}


int main()
{
    //Q1 search functions
        food_location_input();
        //by category
        {
        // Category* cat_node=make_cat_tree();
        // cat_node=Search_by_cat_key(10,cat_node);
        // //note: cat_key 10-19 are family category restaurant
        //     //  cat_key 20-29 are fast-food category resto
        //     //  cat_key 30-30 are fine-dining category resto 
        // printf("Hotel name is %s",cat_node->name);
        }

        //by cusine
        {
        // Category* cusi_node=make_cusi_tree();
        // cusi_node=Search_by_cusi_key(2,cusi_node);
        
        // printf("Hotel cuisine is %s",cusi_node->cuisine);
        }
        
        //by area
        {
        // Category* area_node=make_area_tree();
        // area_node=Search_by_area_key(2,area_node);

        // printf("Hotel is in area named as %s",area_node->address);
        }
    
    //Q2 order function(needed in Q3 and Q4)
        agent_input();

        Agent* root_a=NULL;
        root_a=make_agent_tree();

        user_input();
        User* root_u=make_user_tree();

        
        Date date1={9,4,2023};
        Order* root=NULL;
        // root=place_order(root,"A003",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A003","U001","Hotel_A");
        // root=place_order(root,"A004",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A004","U001","Hotel_A");
        // root=place_order(root,"A001",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A001","U001","Hotel_A");
        // root=place_order(root,"A002",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A002","U001","Hotel_A");
         


    //Q3 delivery function (dependent on Q2)
      {  //root=delivery(root,2,"A002",root_a,200);
        //root=delivery(root,1,"A004",root_a,200);
      }

    //Q4 cancel function
        {
        // root=cancel_order(root,2,"A002",root_a);
        // root=cancel_order(root,1,"A004",root_a);
        }

    //Q5 fav food of user

        // root=place_order(root,"A004",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A004","U001","Hotel_A");
        // root=place_order(root,"A002",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A002","U001","Hotel_A");
        // root=place_order(root,"A010",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A010","U001","Hotel_A");
        // root=place_order(root,"A008",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pasta",200,date1,"A008","U001","Hotel_A");
        // root=place_order(root,"A006",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pasta",200,date1,"A006","U001","Hotel_A");


        // order_list* dummy_list=(order_list*)malloc(sizeof(order_list));
        // strcpy(dummy_list->food_item,"zero");
        // dummy_list->food_item_count=-1;
        // dummy_list->date=date1;



        // find_fav_food_of_user(root,"U001",date1,dummy_list);

    //Q6 find fav resto

    
        root=place_order(root,"A004",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A004","U001","Hotel_A");
        root=place_order(root,"A002",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A002","U001","Hotel_A");
        root=place_order(root,"A010",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pizza",200,date1,"A010","U001","Hotel_A");
        root=place_order(root,"A008",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pasta",200,date1,"A008","U001","Hotel_A");
        root=place_order(root,"A006",root_a,"U001",root_u,"Hotel_A",global_order_no,"Pasta",200,date1,"A006","U001","Hotel_A");



    find_fav_restaurant(root_u,root,90);












      

        
       



    
    return 0;
}