/* suerte.c -- suerte implementation file

Copyright (C) 2011 Javier Angulo

This file is part of suerte

suerte is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
suerte is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with suerte. If not, see <http://www.gnu.org/licenses/>. */

#include <suerte.h>

Item *
create_item (unsigned int number, const char* name, unsigned short int value, unsigned int n_assigns)
{
	/* Check for parameters */
	if (name == NULL || value > 100 || n_assigns <= 0)
		{
			fprintf (stderr, _("[create_item] Invalid parameters while creating item, item discarded\n"));
			return NULL;
		}

	Item* item = (Item *) malloc (sizeof (Item));
	if (!item)
		{
			perror (_("[create_item] Error while allocating item"));
			return NULL;
		}

	item->number = number;
	item->name = strdup (name);
	item->value = value;
	item->n_assigns = n_assigns;

	return item;
}

Person *
create_person (unsigned int number, const char* name, unsigned short int value)
{
	/* Check for parameters */
	if (name == NULL || value > 100)
		{
			fprintf (stderr, _("[create_person] Invalid parameters while creating person, person discarded\n"));
			return NULL;
		}
	
	Person *person = (Person *) malloc (sizeof (Person));
	if (!person)
		{
			perror (_("[create_person] Error while allocating person"));
			return NULL;
		}
	person->number = number;
	person->name = strdup (name);
	person->value = value;
}

Result
deal (Item **items, unsigned int n_items, Person **people, unsigned int n_people)
{
	return RESULT_OK;
}

Interface *
create_interface ()
{
	Interface *iface = (Interface *) malloc (sizeof (Interface));	
	if (!iface)
		{
			perror (_("[create_interface] Error while allocating interface"));
			return NULL;
		}
	
	iface->window = GTK_WINDOW (gtk_window_new (GTK_WINDOW_TOPLEVEL));
	gtk_window_set_title (iface->window, "Suerte");
	gtk_window_set_icon_from_file (iface->window, "/usr/share/pixmaps/suerte.png", NULL);
	gtk_window_set_default_size (iface->window, 800, 600);
	gtk_window_set_position (iface->window, GTK_WIN_POS_CENTER_ALWAYS);

	iface->statusbar = GTK_STATUSBAR (gtk_statusbar_new ());
	iface->vbox_main = GTK_VBOX (gtk_vbox_new (FALSE, 0));
	iface->table_main = GTK_TABLE (gtk_table_new (3, 3, FALSE));
	gtk_table_set_col_spacings (iface->table_main, 3);

	iface->tree_items = GTK_TREE_VIEW (gtk_tree_view_new ());
	iface->tree_people = GTK_TREE_VIEW (gtk_tree_view_new ());
	iface->store_items = gtk_list_store_new (4, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);
	iface->store_people = gtk_list_store_new (4, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT, G_TYPE_STRING);

	
	gtk_tree_view_insert_column_with_attributes (iface->tree_items, -1, _("Number"), gtk_cell_renderer_text_new (), "text", 0, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_items, -1, _("Name"), gtk_cell_renderer_text_new (), "text", 1, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_items, -1, _("Value"), gtk_cell_renderer_text_new (), "text", 2, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_items, -1, _("Min. Assignments"), gtk_cell_renderer_text_new (), "text", 3, NULL);

	gtk_tree_view_insert_column_with_attributes (iface->tree_people, -1, _("Number"), gtk_cell_renderer_text_new (), "text", 0, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_people, -1, _("Name"), gtk_cell_renderer_text_new (), "text", 1, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_people, -1, _("Value"), gtk_cell_renderer_text_new (), "text", 2, NULL);
	gtk_tree_view_insert_column_with_attributes (iface->tree_people, -1, _("Items assigned"), gtk_cell_renderer_text_new (), "text", 3, NULL);

	gtk_tree_view_set_model (iface->tree_items, GTK_TREE_MODEL (iface->store_items));
	gtk_tree_view_set_model (iface->tree_people, GTK_TREE_MODEL (iface->store_people));

	g_object_unref (iface->store_items);
	g_object_unref (iface->store_people);

	iface->buttonbox_people = GTK_HBUTTON_BOX (gtk_hbutton_box_new ());
	iface->buttonbox_items = GTK_HBUTTON_BOX (gtk_hbutton_box_new ());
	gtk_button_box_set_layout (GTK_BUTTON_BOX (iface->buttonbox_people), GTK_BUTTONBOX_END);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (iface->buttonbox_items), GTK_BUTTONBOX_END);

	iface->button_add_item = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_ADD));
	iface->button_add_person = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_ADD));
	iface->button_remove_item = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_REMOVE));
	iface->button_remove_person = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_REMOVE));
	iface->button_go = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_EXECUTE));

	gtk_box_pack_start (GTK_BOX (iface->buttonbox_people), GTK_WIDGET (iface->button_add_person), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (iface->buttonbox_people), GTK_WIDGET (iface->button_remove_person), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (iface->buttonbox_items), GTK_WIDGET (iface->button_add_item), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (iface->buttonbox_items), GTK_WIDGET (iface->button_remove_item), FALSE, FALSE, 0);

	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (gtk_label_new (_("People"))), 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (gtk_label_new (_("Items"))), 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (iface->tree_people), 0, 1, 1, 2);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (iface->tree_items), 1, 2, 1, 2);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (iface->buttonbox_people), 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (iface->buttonbox_items), 1, 2, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (iface->table_main), GTK_WIDGET (iface->button_go), 2, 3, 1, 2);

	gtk_box_pack_start (GTK_BOX (iface->vbox_main), GTK_WIDGET (iface->table_main), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (iface->vbox_main), GTK_WIDGET (iface->statusbar), FALSE, FALSE, 0);

	gtk_container_add (GTK_CONTAINER (iface->window), GTK_WIDGET (iface->vbox_main));

	gtk_widget_show_all (GTK_WIDGET (iface->window));

	return iface;
}

int
main (int argc, char **argv)
{
	bindtextdomain (GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	gtk_init (&argc, &argv);
	Interface *iface = create_interface ();
	gtk_main ();
}
