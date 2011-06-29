/* mll.h -- mll header file

Copyright (C) 2011 Javier Angulo

This file is part of mll

mll is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
mll is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with mll.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _MLL_H
#define _MLL_H

#include <gtk/gtk.h>

/** Interface definition */
struct mll_Gui {

	/** Main window. */
	GtkWindow *window;
	/** VBox to display the full app. */
	GtkVBox *vbox_main;
	/** Menu bar. */
	GtkMenuBar *menubar;
	/** Status bar. */
	GtkStatusBar *statusbar;
	/** Table to pack the application widgets. */
	GtkTable *table_all;

	/** Full source code scrolled windows. */
	GtkScrolledWindow **scrolled_source_full;
	/** Snippet source code scrolled windows. */
	GtkScrolledWindow **scrolled_source_snippet;
	/** Book text scrolled window. */
	GtkScrolledWindow *scrolled_text_chapter;
	/** Notebook to show full source code files. */
	GtkNotebook *notebook_source_full;
	/** Notebook to show snippets for the current chapter. */
	GtkNotebook *notebook_source_snippets;
	/** Full source code viewers. */
	GtkSourceView **source_full;
	/** Snippet source code viewers. */
	GtkSourceView **source_snippet;
	/** Book viewer. */
	GtkTextView *text_chapter;

	/** Next chapter button. */
	GtkButton *button_next;
	/** Previous chapter button. */
	GtkButton *button_prev;
	/** Run application button. */
	GtkButton *button_run;

	/** Chapter count label. */
	GtkLabel *label_chapter_count;
	/** Select a chapter by number. */
	GtkEntry *entry_goto_chapter;
	/** Go to a selected chapter button. */
	GtkButton *button_goto_chapter;
};

/** Snippet definition. */
struct mll_Snippet {

	/** Snippet number. */
	int number;
	/** Snippet source file. */
	char *file;
	/** Snippet top limit. */
	int top;
	/** Snippet bottom limit. */
	int bottom;

};

/** Book chapter definition. */
struct mll_Chapter {
	
	/** Chapter number. */
	int number;
	/** Number of code snippets. */
	int snippets_number;
	/** Snippets vector. */
	struct mll_Snippet **snippets;
	/** Chapter text. */
	char *text;

};

/** Book definition. */
struct mll_Book {
	
	/** Language name. */
	char *language;
	/** Introduction text. */
	char *intro;
	/** Number of chapters. */
	int n_chapters;
	/** Chapters. */
	struct mll_Chapter **chapters;
	/** Number of source files. */
	int n_files;
	/** Source file names. */
	char **files;

};

struct mll_Gui *
mll_create_gui ();

struct mll_Book *
mll_load_book_in_mem (char *book_file);

int
mll_load_book_in_gui (struct mll_gui *gui, struct mll_Book *book);

void
mll_show_gui (struct mll_gui *gui);

#endif 
