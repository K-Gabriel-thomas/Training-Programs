// Inventory management using linked list.

#include <stdio.h>
#include <stdlib.h>
#include "mytools.h"

#define EMPTY_INVENTORY printf("The inventory is currently empty. Try adding an Item first!\n\n")
#define LENGTH_OF_ID 15
#define LENGTH_OF_NAME 50
#define FILE_NAME "inventory_data.dat"

struct item_node
{
	char id[LENGTH_OF_ID];
	char name[LENGTH_OF_NAME];
	float price;
	struct item_node *next_item_node;
};
typedef struct item_node item_node;
item_node *list_head = NULL;
item_node *list_last = NULL;

enum
{
	CHANGE_PRICE = 1,
	CHANGE_ITEM_NAME,
	REMOVE_ITEM,
};

int record_size = sizeof(item_node) - sizeof(item_node *);
int is_data_changed = 0;

void add_item();
void show_all_items();
void modify_item_details(int);
void append_item(item_node *);
void save_all_data_to_file();
void load_data_from_file();
void clear_all_data_in_list();
int get_nodes_count();
void print_options();

void main()
{
	load_data_from_file();
	char user_option;
	while (1)
	{
		print_options();
		scanf(" %c", &user_option);
		clear_buffer();
		printf("\n");
		switch (user_option)
		{
		case '1':
			add_item();
			break;
		case '2':
			show_all_items();
			break;
		case '3':
			modify_item_details(CHANGE_ITEM_NAME);
			break;
		case '4':
			modify_item_details(CHANGE_PRICE);
			break;
		case '5':
			modify_item_details(REMOVE_ITEM);
			break;
		case '6':
			save_all_data_to_file();
			break;
		case '7':
			save_all_data_to_file();
			clear_all_data_in_list();
			printf("Exited!\n");
			exit(0);
		default:
			printf("Enter a valid option!\n");
			break;
		}
	}
}

void add_item()
{
	item_node *new_item_node = malloc(sizeof(item_node));
	printf("Enter item id: ");
	fgets(new_item_node->id, LENGTH_OF_ID, stdin);
	remove_newline(new_item_node->id);
	printf("Enter name of the item: ");
	fgets(new_item_node->name, LENGTH_OF_NAME, stdin);
	remove_newline(new_item_node->name);
	printf("Enter the price: ");
	scanf("%f", &(new_item_node->price));
	clear_buffer();
	append_item(new_item_node);
	is_data_changed = 1;
	printf("\nItem added Successfully\n\n");
}

void show_all_items()
{
	if (list_head == NULL)
	{
		EMPTY_INVENTORY;
	}
	else
	{
		for (item_node *current_node = list_head; current_node != NULL; current_node = current_node->next_item_node)
		{
			printf("Id: %s\n", current_node->id);
			printf("Name: %s\n", current_node->name);
			printf("Price: %f\n", current_node->price);
			printf("\n");
		}
	}
}

void modify_item_details(int mode)
{
	if (list_head == NULL)
	{
		EMPTY_INVENTORY;
	}
	else
	{
		char target_id[LENGTH_OF_ID];
		item_node *previous_node = NULL;
		item_node *current_node = NULL;
		int is_id_found = 0;
		char *prompt_text = (mode == CHANGE_ITEM_NAME) ? "Rename" : (mode == CHANGE_PRICE) ? "Update the price"
																						   : "Remove";
		printf("Enter the Id of the item to %s: ", prompt_text);
		fgets(target_id, LENGTH_OF_ID, stdin);
		remove_newline(target_id);
		current_node = list_head;
		while (current_node != NULL)
		{
			if (strcmp(current_node->id, target_id) != 0)
			{
				previous_node = current_node;
				current_node = current_node->next_item_node;
			}
			else
			{
				is_id_found = 1;
				break;
			}
		}
		if (is_id_found)
		{
			if (mode == CHANGE_ITEM_NAME)
			{
				printf("Enter a new name for item-'%s': ", current_node->name);
				fgets(current_node->name, LENGTH_OF_NAME, stdin);
				remove_newline(current_node->name);
			}
			if (mode == CHANGE_PRICE)
			{
				printf("Enter a new price for item-'%s': ", current_node->name);
				scanf("%f", &current_node->price);
			}
			if (mode == REMOVE_ITEM)
			{
				if (current_node == list_head)
				{
					list_head = list_head->next_item_node;
					if (list_head == NULL)
					{
						list_last = NULL;
					}
				}
				else
				{
					previous_node->next_item_node = current_node->next_item_node;
					if (previous_node->next_item_node == NULL)
					{
						list_last = previous_node;
					}
					current_node->next_item_node = NULL;
				}
				free(current_node);
			}
			is_data_changed = 1;
			char *success_text = (mode == CHANGE_ITEM_NAME) ? "renamed" : (mode == CHANGE_PRICE) ? "updated with a new price"
																								 : "removed from inventory";
			printf("\nItem ID '%s' has been %s successfully.\n\n", target_id, success_text);
		}
		else
		{
			printf("\nItem ID '%s' not found in the list.\n\n", target_id);
		}
	}
}

void save_all_data_to_file()
{
	if (!is_data_changed)
	{
		printf("No changes to save!\n\n");
		return;
	}
	FILE *fp_data = fopen(FILE_NAME, "wb");
	for (item_node *current_node = list_head; current_node != NULL; current_node = current_node->next_item_node)
	{
		fwrite(current_node, record_size, 1, fp_data);
	}
	fclose(fp_data);
	is_data_changed = 0;
	printf("All the data has been successfully saved to the file!\n\n");
}

void append_item(item_node *new_item_node)
{
	if (list_head == NULL)
	{
		list_head = new_item_node;
	}
	else
	{
		list_last->next_item_node = new_item_node;
	}
	list_last = new_item_node;
	list_last->next_item_node = NULL;
}

void load_data_from_file()
{
	clear_all_data_in_list();
	item_node record;
	FILE *fp_data = fopen(FILE_NAME, "rb");
	if (fp_data == NULL)
		return;
	while (fread(&record, record_size, 1, fp_data) == 1)
	{
		item_node *new_item_node = malloc(sizeof(item_node));
		*new_item_node = record;
		append_item(new_item_node);
	}
	fclose(fp_data);
	is_data_changed = 0;
}

void clear_all_data_in_list()
{
	item_node *current_node = list_head, *next_node;
	while (current_node != NULL)
	{
		next_node = current_node->next_item_node;
		free(current_node);
		current_node = next_node;
	}
	list_head = NULL;
	list_last = NULL;
}

int get_nodes_count()
{
	int nodes_counter = 0;
	item_node *current_node = list_head;
	while (current_node != NULL)
	{
		nodes_counter++;
		current_node = current_node->next_item_node;
	}
	return nodes_counter;
}

void print_options()
{
	printf("1. Add New Item.\n");
	printf("2. Show all Items.\n");
	printf("3. Change an Item Name.\n");
	printf("4. Change Price of an Item.\n");
	printf("5. Remove an Item.\n");
	printf("6. Save all Data.\n");
	printf("7. Exit.\n");
	printf("Choose an option from the above: ");
}
