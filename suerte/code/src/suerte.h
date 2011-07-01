/* suerte.h -- suerte header file

Copyright (C) 2011 Javier Angulo

This file is part of suerte

suerte is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
suerte is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with suerte. If not, see <http://www.gnu.org/licenses/>. */

#ifndef _SUERTE_H
#define _SUERTE_H

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <config.h>

/* DOMAIN DEFINITIONS. */

/** Item definition. */
struct _Item {

	/** Number used in deals. */
	unsigned int number;

	/** Representative name. */
	char *name;

	/** Value of the item, used to ponderate deals. Range 0-100. */
	unsigned int value;

	/** Number of people to be assigned to. */
	unsigned int n_assigns;

};

/** People definition. */
struct _Person {
	
	/** Number used in deals. */
	unsigned int number;

	/** Representative name. */
	char *name;

	/** Person value. Used to ponderate deals. Range 0-100. */
	unsigned int value;

	/** Number of assigned items. */
	unsigned int n_items;

	/** Vector of assigned items. */
	struct _Item **items;

};

/** Deal result. 
 * ITEMS_OVERASSIGNED if one or more items with value X
 * have been assigned to a person with value <= X.
 * ITEMS_UNDERASSIGNED if one or more items with n_assigns X
 * have been assigned to Y people, Y < X.
 * OK in the rest of cases. */
enum _Result {RESULT_OK, RESULT_ITEMS_OVERASSIGNED, RESULT_ITEMS_UNDERASSIGNED};

typedef struct _Item Item;
typedef struct _Person Person;
typedef enum _Result Result;

/* GRAPHICAL INTERFACE DEFINITIONS */

/** Interface definition */
struct _Interface {

	/** Main window. */
	GtkWindow *window;
	/** Menu bar. */
	GtkMenuBar *menubar;
	/** Toolbar. */
	GtkToolbar *toolbar;
	/** Status bar. */
	GtkStatusbar *statusbar;

	/** Main vertical box. */
	GtkVBox *vbox_main;
	/** Table to pack the rest of the widgets. */
	GtkTable *table_main;

	/** Tree view for items. */
	GtkTreeView *tree_items;
	/** Tree view for people. */
	GtkTreeView *tree_people;
	/** Model to store items. */
	GtkListStore *store_items;
	/** Model to store people. */
	GtkListStore *store_people;

	/** Button box to pack person buttons. */
	GtkHButtonBox *buttonbox_people;
	/** Button box to pack item buttons. */
	GtkHButtonBox *buttonbox_items;
	/** Add item button. */
	GtkButton *button_add_item;
	/** Add person button. */
	GtkButton *button_add_person;
	/** Remove item button. */ 
	GtkButton *button_remove_item;
	/** Remove person button. */
	GtkButton *button_remove_person;

	/** Distribute items button. */
	GtkButton *button_go;
};

typedef struct _Interface Interface;


/** Create an item.
 * @param number Item number.
 * @param name Item name.
 * @param value Importance in the deal, assign this item only to
 * a person which value is equals or greater than this value. Range 0-100. 
 * @param n_assign Assign this item to, as possible, n_assigns.
 * @return Item reference, NULL if parameters are incorrect. */
Item *
create_item (unsigned int number, const char* name, unsigned short int value, unsigned n_assigns);

/** Create a person.
 * @param number Person number.
 * @param name Person name.
 * @param value Importance in the deal, this person can obtain items with value
 * smaller or equals than value.
 * @return Person reference, NULL if parameters are incorrect. */
Person *
create_person (unsigned int number, const char* name, unsigned short int value);

/** Deal the items and assign them to the people.
 * @param items Vector of n_items size of items.
 * @param n_items Number of items to be assigned.
 * @param people Vector of n_people people.
 * @param n_people Number of people to assign items to.
 * @return Result of the deal. */
Result
deal (Item **items, unsigned int n_items, Person **people, unsigned int n_people);

/** Create an instance of the graphical interface.
 * @return Reference to a new graphical interface. */
Interface *
create_interface ();

#endif 
