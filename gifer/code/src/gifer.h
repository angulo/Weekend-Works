/* gifer.h -- Gifer header file

Copyright (C) 2011 Javier Angulo

This file is part of Gifer

Gifer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Gifer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Gifer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _GIFER_H
#define _GIFER_H

#define DEFAULT_SIZE 200
#define DEFAULT_DELAY "100"
#define IMAGE_SELECTION "/usr/share/gifer/selection.svg"
#define IMAGE_BACKGROUND "/usr/share/gifer/background.svg"
#define IMAGE_ARROW "/usr/share/gifer/arrow.svg"
#define IMAGE_LOGO "/usr/share/pixmaps/gifer.png"

#include <config.h>

#include <iostream>
#include <list>

#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <Magick++.h>

#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

namespace gifer {

	/** Actions. */
	struct Actions {
	
		/** New project. */
		Glib::RefPtr<Gtk::Action> project_new;

		/** Show menu. */
		Glib::RefPtr<Gtk::Action> about;

		/** Quit application. */
		Glib::RefPtr<Gtk::Action> quit;

		/** Select all. */
		Glib::RefPtr<Gtk::Action> select_all;

		/** Unselect all. */
		Glib::RefPtr<Gtk::Action> unselect_all;

		/** Add a new image to the list. */
		Glib::RefPtr<Gtk::Action> image_add;

		/** Remove an image from the list. */
		Glib::RefPtr<Gtk::Action> image_remove;

		/** Copy an image. */
		Glib::RefPtr<Gtk::Action> image_copy;
		
		/** Paste an image. */
		Glib::RefPtr<Gtk::Action> image_paste;

		/** Move selected image to the right. */
		Glib::RefPtr<Gtk::Action> image_move_right;

		/** Move selected image to the right. */
		Glib::RefPtr<Gtk::Action> image_move_left;

		/** Loop infinitely. */
		Glib::RefPtr<Gtk::Action> loop;

		/** Generate gif animation. */
		Glib::RefPtr<Gtk::Action> gif_generate;

		/** Preview gif animation. */
		Glib::RefPtr<Gtk::Action> gif_preview;
	};

	class Gifer;

	class Image : public Gtk::Table {
		
		private:
			
			/** Event box to support click events. */
			Gtk::EventBox *_event_box;

			/** Loaded image. */
			Gtk::Image *_image_widget;

			/** Selection mask. */
			Gtk::Image *_image_background;

			/** Change the delay of the image. */
			Gtk::SpinButton *_entry_delay;
			
			/** Path to the loaded image. */
			string _image_path;

			/** Whether the image is selected or not. */
			bool _selected;
			
			/** Whether the image is copied or not. */
			bool _copied;

			/** Clicked callback.
			 * @param event Button event.
			 * @return false. */
			bool _clicked (GdkEventButton *event);

		public:
			
			Image (string image_path, gifer::Gifer *parent);
			~Image ();
					
			/** Sets the image as selected or not.
			 * @param selected True if the image is selected. */
			void set_selected (bool selected);

			/** Get the selected property.
			 * @return True if the image is selected. */
			bool get_selected ();

			/** Sets the image as copied or not.
			 * @param selected True if the image is copied. */
			void set_copied (bool copied);

			/** Get the copied property.
			 * @return True if the image is copied. */
			bool get_copied ();

			/** Get the image delay. 
			 * @return Image delay in miliseconds. */
			int get_delay ();

			/** Get image path.
			 * @return Path to the image. */
			string get_path ();
	};

	class Gifer {

		private:
			
			/* Gtk widgets */

			/** Gtk main. */
			Gtk::Main *_main;

			/** Main window. */
			Gtk::Window *_window;

			Gtk::Dialog *_window_preview;

			/** Main layout box. */
			Gtk::VBox *_vbox;

			/** Box to pack images into. */
			Gtk::HButtonBox *_hbox_images;

			/** Scrolled window to pack images box into. */
			Gtk::ScrolledWindow *_scrolled_images;

			/** About window. */
			Gtk::AboutDialog *_about_dialog;

			/** Menu bar. */
			Gtk::MenuBar *_menubar;

			/** Tool bar. */
			Gtk::Toolbar *_toolbar;

			/** Actions used in the application. */
			gifer::Actions _actions;

			/** Action group for the menu. */
			Glib::RefPtr<Gtk::ActionGroup> _action_group;

			/** UI Manager. */
			Glib::RefPtr<Gtk::UIManager> _ui_manager;

			/** State bar. */
			Gtk::Statusbar *_statusbar;

			/** Arrow image. */
			Gtk::Image *_image_arrow;

			/** List of added images. */
			list<gifer::Image *> _images;

			/** Last directory used to add files. */
			string _last_dir_add;

			/** Last directory used to save gif. */
			string _last_dir_save;

			/** Loop infinitely. */
			bool _loop;

			/** Create the menu bar. */
			void _create_menubar ();

			/** Create the about dialog. */
			void _create_about ();

			/** Create a new project. */
			void _project_new ();

			/** Close application. */
			void _app_close ();

			/** Add image. */
			void _image_add ();
	
			/** Remove image. */
			void _image_remove ();

			/** Copy image. */
			void _image_copy ();

			/** Paste image. */
			void _image_paste ();

			/** Preview gif in thread. */
			void _gif_preview_priv ();

			/** Preview gif. */
			void _gif_preview ();

			/** Generate gif.
			 * @param Destination file. */
			void _gif_generate ();

			/** Toggle loop. */
			void _loop_toggle ();

			/** Generate the gif.
			 * @param dest_file Destination file. */
			void _gif_generate_priv (string dest_file = "");

			/** Select all images callback. */
			void _select_all ();

			/** Unselect all images callback. */
			void _unselect_all ();

			/** Move an image to the left. */
			void _move_right ();

			/** Move an image to the right. */
			void _move_left ();

		public:

			Gifer ();
			~Gifer ();

			/** Run gifer application.
			 * @param argc Argument count.
			 * @param argv Argument vector. */
			int run (int argc, char **argv);	

			/** Image clicked callback.
			 * @param event Click event.
			 * @param image Image reference.
			 * @return true. */
			bool child_clicked (GdkEventButton *event, gifer::Image *image);

	};

	/** Print program help. */
	void print_help ();
	
	/** Print program version. */
	void print_version ();

};

#endif
