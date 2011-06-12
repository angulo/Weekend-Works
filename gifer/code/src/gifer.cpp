/* gifer.cpp -- Gifer implementation

Copyright (C) 2011 Javier Angulo

This file is part of Gifer

Gifer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Gifer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Gifer.  If not, see <http://www.gnu.org/licenses/>. */

#include <gifer.h>

namespace gifer {

	bool
	Image::_clicked (GdkEventButton *event)
	{
		set_selected (!_selected);
		return false;
	}
		
	Image::Image (string image_path, Gifer *parent) : Gtk::Table (2, 1, false), _image_path (image_path)
	{
		_selected = false;
		_copied = false;

		/* Gtk widgets */
		_event_box = new Gtk::EventBox ();
		_event_box->set_visible_window (false);
		attach (*_event_box, 0, 1, 0, 1, (Gtk::AttachOptions) Gtk::SHRINK, (Gtk::AttachOptions) (Gtk::SHRINK | Gtk::FILL), 15, 15);
		
		/* Added image */
		Glib::RefPtr<Gdk::Pixbuf> pb = Gdk::Pixbuf::create_from_file (image_path, DEFAULT_SIZE, DEFAULT_SIZE, true);
		_image_widget = new Gtk::Image ();
		_image_widget->set (pb);
		_event_box->add (*_image_widget);

		/* Background image */
		Glib::RefPtr<Gdk::Pixbuf> pb_background = Gdk::Pixbuf::create_from_file (IMAGE_BACKGROUND, DEFAULT_SIZE + 15, DEFAULT_SIZE + 15, true);
		_image_background = new Gtk::Image ();
		_image_background->set (pb_background);
		attach (*_image_background, 0, 1, 0, 1, (Gtk::AttachOptions) Gtk::SHRINK, (Gtk::AttachOptions) Gtk::SHRINK, 5, 5);

		/* Delay entry box */
		_entry_delay = new Gtk::SpinButton (0.01, 2);
		_entry_delay->set_alignment (Gtk::ALIGN_RIGHT);
		_entry_delay->set_adjustment (*Gtk::manage (new Gtk::Adjustment (0.5, 0.01, 100, 0.01, 0.01, 0)));
		_entry_delay->set_editable (true);
		_entry_delay->set_numeric (true);
		_entry_delay->set_width_chars (4);
		_entry_delay->set_tooltip_text (_("Set the image delay in seconds"));
		attach (*_entry_delay, 0, 1, 1, 2, (Gtk::AttachOptions) Gtk::SHRINK, (Gtk::AttachOptions) Gtk::SHRINK, 5, 5);

		_event_box->show ();
		_entry_delay->show ();
		_image_background->show ();
		_image_widget->show ();

		/* Connect signals */
		_event_box->signal_button_press_event ().connect (sigc::mem_fun (*this, &gifer::Image::_clicked), true);
		_event_box->signal_button_press_event ().connect (sigc::bind (sigc::mem_fun (*parent, &gifer::Gifer::child_clicked), this), true);

		show ();
	}

	Image::~Image ()
	{
	}

	void 
	Image::set_selected (bool selected)
	{
		/* Image deselected */
		if (_selected && !selected)
			{
				Glib::RefPtr<Gdk::Pixbuf> pb = Gdk::Pixbuf::create_from_file (IMAGE_BACKGROUND, DEFAULT_SIZE + 15, DEFAULT_SIZE + 15, true);
				_image_background->set (pb);
			}
		/* Image selected */
		else if (!_selected && selected)
			{
				Glib::RefPtr<Gdk::Pixbuf> pb = Gdk::Pixbuf::create_from_file (IMAGE_SELECTION, DEFAULT_SIZE + 15, DEFAULT_SIZE + 15, true);
				_image_background->set (pb);
			}
		_selected = selected;
	}

	bool 
	Image::get_selected ()
	{
		return _selected;
	}

	void 
	Image::set_copied (bool copied)
	{
		_copied = copied;
	}

	bool 
	Image::get_copied ()
	{
		return _copied;
	}

	int 
	Image::get_delay ()
	{
		return _entry_delay->get_value () * 100;
	}

	string 
	Image::get_path ()
	{
		return _image_path;
	}

	void
	Gifer::_create_menubar ()
	{
		_action_group = Gtk::ActionGroup::create ();
		_actions.project_new = Gtk::Action::create ("ProjectNew", Gtk::Stock::NEW, _("_New project"), _("Create a new project"));
		_actions.quit = Gtk::Action::create ("Quit", Gtk::Stock::QUIT);
		_actions.select_all = Gtk::Action::create ("SelectAll", _("Select all"), _("Select all images"));
		_actions.unselect_all = Gtk::Action::create ("UnselectAll", _("Unselect all"), _("Unselect all images"));
		_actions.image_add = Gtk::Action::create ("ImageAdd", Gtk::Stock::ADD, _("_Add images"), _("Add new images to the list"));
		_actions.image_remove = Gtk::Action::create ("ImageRemove", Gtk::Stock::REMOVE, _("_Remove images"), _("Remove the selected images"));
		_actions.image_copy = Gtk::Action::create ("ImageCopy", Gtk::Stock::COPY, _("_Copy images"), _("Copy the selected images"));
		_actions.image_paste = Gtk::Action::create ("ImagePaste", Gtk::Stock::PASTE, _("_Paste images"), _("Paste the selected images"));
		_actions.image_move_right = Gtk::Action::create ("ImageMoveRight", Gtk::Stock::GO_FORWARD, _("Move right"), _("Move the selected image to the right"));
		_actions.image_move_left = Gtk::Action::create ("ImageMoveLeft", Gtk::Stock::GO_BACK, _("Move left"), _("Move the selected image to the left"));
		_actions.loop = Gtk::ToggleAction::create ("Loop", _("Loop"), _("Loop"), true);
		_actions.gif_generate = Gtk::Action::create ("GifGenerate", Gtk::Stock::SAVE_AS, _("_Generate GIF"), _("Generate an animated GIF using the added images"));
		_actions.gif_preview = Gtk::Action::create ("GifPreview", Gtk::Stock::EXECUTE, _("P_review GIF"), _("Preview current project"));
		_actions.about = Gtk::Action::create ("About", _("_About"));

		/* Initialize actions */
		_actions.image_copy->set_sensitive (false);
		_actions.image_paste->set_sensitive (false);
		_actions.image_move_left->set_sensitive (false);
		_actions.image_move_right->set_sensitive (false);
		_actions.gif_generate->set_sensitive (false);
		_actions.gif_preview->set_sensitive (false);

		_action_group->add (Gtk::Action::create ("MenuFile", _("_File")));
		_action_group->add (_actions.project_new, Gtk::AccelKey ("<control>N"), sigc::mem_fun (*this, &Gifer::_project_new));
		_action_group->add (_actions.quit, sigc::mem_fun (*this, &Gifer::_app_close));

		_action_group->add (Gtk::Action::create ("MenuEdit", _("_Edit")));

		_action_group->add (_actions.select_all, Gtk::AccelKey ("<control>A"), sigc::mem_fun (*this, &Gifer::_select_all));
		_action_group->add (_actions.unselect_all, Gtk::AccelKey ("<control>I"), sigc::mem_fun (*this, &Gifer::_unselect_all));
		_action_group->add (_actions.image_add, Gtk::AccelKey ("<control>M"), sigc::mem_fun (*this, &Gifer::_image_add));
		_action_group->add (_actions.image_remove, Gtk::AccelKey ("<control>D"), sigc::mem_fun (*this, &Gifer::_image_remove));
		_action_group->add (_actions.image_copy, Gtk::AccelKey ("<control>C"), sigc::mem_fun (*this, &Gifer::_image_copy));
		_action_group->add (_actions.image_paste, Gtk::AccelKey ("<control>V"), sigc::mem_fun (*this, &Gifer::_image_paste));
		_action_group->add (_actions.image_move_right, Gtk::AccelKey ("<alt>Right"), sigc::mem_fun (*this, &Gifer::_move_right));
		_action_group->add (_actions.image_move_left, Gtk::AccelKey ("<alt>Left"), sigc::mem_fun (*this, &Gifer::_move_left));

		_action_group->add (Gtk::Action::create ("MenuActions", _("_Actions")));
		_action_group->add (_actions.gif_preview, Gtk::AccelKey ("<control>P"), sigc::mem_fun (*this, &Gifer::_gif_preview));
		_action_group->add (_actions.gif_generate, Gtk::AccelKey ("<control>G"), sigc::mem_fun (*this, &Gifer::_gif_generate));

		_action_group->add (Gtk::Action::create ("MenuOptions", _("_Options")));
		_action_group->add (_actions.loop, Gtk::AccelKey ("<control>L"), sigc::mem_fun (*this, &Gifer::_loop_toggle));

		_action_group->add (Gtk::Action::create ("MenuHelp", _("_Help")));
		_action_group->add (_actions.about, sigc::mem_fun (_about_dialog, &Gtk::Window::show));

		_ui_manager = Gtk::UIManager::create ();
		_ui_manager->insert_action_group (_action_group);

		Glib::ustring ui_xml = 
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='MenuFile'>"
		"			<menuitem action='ProjectNew'/>"
		"			<separator/>"
		"			<menuitem action='Quit'/>"
		"		</menu>"
		"		<menu action='MenuEdit'>"
		"			<menuitem action='SelectAll'/>"
		"			<menuitem action='UnselectAll'/>"
		"			<separator/>"
		"			<menuitem action='ImageAdd'/>"
		"			<menuitem action='ImageRemove'/>"
		"			<separator/>"
		"			<menuitem action='ImageCopy'/>"
		"			<menuitem action='ImagePaste'/>"
		"			<separator/>"
		"			<menuitem action='ImageMoveLeft'/>"
		"			<menuitem action='ImageMoveRight'/>"
		"		</menu>"
		"		<menu action='MenuActions'>"
		"			<menuitem action='GifPreview'/>"
		"			<menuitem action='GifGenerate'/>"
		"		</menu>"
		"		<menu action='MenuOptions'>"
		"			<menuitem action='Loop'/>"
		"		</menu>"
		"		<menu action='MenuHelp'>"
		"			<menuitem action='About'/>"
		"		</menu>"
		"	</menubar>"
		"	<toolbar action='Toolbar'>"
		"		<toolitem action='ProjectNew'/>"
		"		<separator/>"
		"		<toolitem action='ImageAdd'/>"
		"		<toolitem action='ImageRemove'/>"
		"		<separator/>"
		"		<toolitem action='ImageCopy'/>"
		"		<toolitem action='ImagePaste'/>"
		"		<separator/>"
		"		<toolitem action='ImageMoveLeft'/>"
		"		<toolitem action='ImageMoveRight'/>"
		"		<separator/>"
		"		<toolitem action='GifPreview'/>"
		"		<toolitem action='GifGenerate'/>"
		"	</toolbar>"
		"</ui>";

		try
			{
			_ui_manager->add_ui_from_string (ui_xml);
			}
		catch (Glib::MarkupError& ex)
			{
				cerr << "[gifer::Gifer::_create_menubar]" << ex.what () << endl;
				return;
			}
		_window->add_accel_group (_ui_manager->get_accel_group ());
		_menubar = dynamic_cast<Gtk::MenuBar *> (_ui_manager->get_widget ("/MenuBar"));
		_toolbar = dynamic_cast<Gtk::Toolbar *> (_ui_manager->get_widget ("/Toolbar"));
	}

	void
	Gifer::_create_about ()
	{
		_about_dialog = new Gtk::AboutDialog ();			

		/* Connect close button signal (gtk bug patch ) */
		Gtk::HBox *hbox = (Gtk::HBox *) *(_about_dialog->get_children ().begin ());
		Gtk::HButtonBox *buttons = (Gtk::HButtonBox *) std::vector<Gtk::Widget *> (hbox->get_children ())[1];
		((Gtk::Button *) buttons->children ()[0].get_widget ())->signal_clicked ().connect (sigc::mem_fun (_about_dialog, &Gtk::Window::hide));

		/* Fill the data */
		list<string> authors;
		authors.push_back ("Javier Angulo Lucerón <javier.angulo1@gmail.com>");
		_about_dialog->set_authors (authors);
		_about_dialog->set_copyright ("Copyright (C) 2011 Javier Angulo");
		_about_dialog->set_license ("GNU GPL v3");
		_about_dialog->set_logo (Gdk::Pixbuf::create_from_file (IMAGE_LOGO));
		_about_dialog->set_icon_from_file (IMAGE_LOGO);
		_about_dialog->set_program_name ("gifer");
		_about_dialog->set_version (PACKAGE_VERSION);	
		_about_dialog->set_website ("http://weekendworks.wordpress.com/gifer");
	}

	void 
	Gifer::_project_new ()
	{
		if (_images.size () == 0)
			return;

		Gtk::MessageDialog dialog (_("Are you sure do you want to delete the current project?"), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
		dialog.set_icon_from_file (IMAGE_LOGO);
		if (dialog.run () == Gtk::RESPONSE_NO)
			return;

		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			delete *it;
		_images.clear ();
		_statusbar->push (_("New project created"));
	}

	void 
	Gifer::_app_close ()
	{
		if (_images.size () == 0)
			{
				Gtk::Main::quit ();
				return;
			}

		Gtk::MessageDialog dialog (_("Are you sure do you want to quit?"), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
		dialog.set_icon_from_file (IMAGE_LOGO);
		if (dialog.run () == Gtk::RESPONSE_NO)
			return;
		Gtk::Main::quit ();
	}

	void 
	Gifer::_image_add ()
	{
		Gtk::FileFilter format_filter;
		format_filter.add_pixbuf_formats ();

		Gtk::FileChooserDialog dialog (_("Add..."), Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.set_icon_from_file (IMAGE_LOGO);
		dialog.set_select_multiple (true);
		dialog.set_filter (format_filter);
		dialog.add_button (_("Cancel"), Gtk::RESPONSE_CANCEL);
		dialog.add_button (_("Add"), Gtk::RESPONSE_ACCEPT);
		int response = dialog.run ();

		list<string> files;
		if (response != Gtk::RESPONSE_ACCEPT)
			return;
		files = dialog.get_filenames ();
		list<string> problem_images;

		for (list<string>::iterator it = files.begin (); it != files.end (); it++)
			{
				try
					{
						gifer::Image *img = new gifer::Image (*it, this);
						_images.push_back (img);
						_hbox_images->pack_start (*img, Gtk::PACK_EXPAND_PADDING, 0);
					}
				catch (Glib::Exception &ex)
					{
						cerr << ex.what () << endl;
						problem_images.push_back (*it);
					}
			}
		if (problem_images.size () > 0)
			{
				string message = _("The following images have not been added. Check its format.");
				message += "\n";
				for (list<string>::iterator it = problem_images.begin (); it != problem_images.end (); it++)
					message += (*it + "\n");

				Gtk::MessageDialog msg_dialog (message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
				msg_dialog.set_icon_from_file (IMAGE_LOGO);
				msg_dialog.run ();
			}
		stringstream ss;
		ss << files.size () - problem_images.size ();
		_statusbar->push (ss.str () + _(" images added"));

		/* Check generation and preview actions */
		if (_images.size () == 0)
			{
				_actions.gif_generate->set_sensitive (false);
				_actions.gif_preview->set_sensitive (false);
			}
		else
			{
				_actions.gif_generate->set_sensitive (true);
				_actions.gif_preview->set_sensitive (true);
			}
	}

	void 
	Gifer::_image_remove ()
	{
		list<gifer::Image *> to_remove;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			if ((*it)->get_selected ())
				{
					to_remove.push_back (*it);
					delete *it;
				}
		for (list<gifer::Image *>::iterator it = to_remove.begin (); it != to_remove.end (); it++)
			_images.remove (*it);	
		stringstream ss;
		ss << to_remove.size ();
		_statusbar->push (ss.str () + _(" images removed"));

		/* Check generation and preview actions */
		if (_images.size () == 0)
			{
				_actions.gif_generate->set_sensitive (false);
				_actions.gif_preview->set_sensitive (false);
			}
		else
			{
				_actions.gif_generate->set_sensitive (true);
				_actions.gif_preview->set_sensitive (true);
			}
	}

	void 
	Gifer::_image_copy ()
	{
		int count = 0;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			{
				(*it)->set_copied((*it)->get_selected ());
				(*it)->set_selected (false);
				if ((*it)->get_copied ())
					count++;
			}
		stringstream ss;
		ss << count;
		_statusbar->push (ss.str () + _(" images copied"));

		_actions.image_paste->set_sensitive (count > 0);
	}

	void 
	Gifer::_image_paste ()
	{
		int count = 0;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			if ((*it)->get_copied ())
				{
					count++;
					gifer::Image *img = new gifer::Image ((*it)->get_path (), this);
					_images.push_back (img);
					_hbox_images->pack_start (*img, false, false, 0);
				}
		stringstream ss;
		ss << count;
		_statusbar->push (ss.str () + _(" images pasted"));
	}

	void
	Gifer::_gif_preview_priv ()
	{

		string dest_file = "/tmp/gifer_temp_gif.gif";
		_gif_generate_priv (dest_file);
		
		GDK_THREADS_ENTER ();
		_statusbar->push (_("Preview loaded"));
		Gtk::VBox *vbox = _window_preview->get_vbox ();
		Gtk::Box_Helpers::BoxList children = vbox->children ();
		list<Gtk::Widget *> to_remove;

		for (Gtk::Box_Helpers::BoxList::iterator it = children.begin (); it != children.end (); it++)
			to_remove.push_back (it->get_widget ());

		for (list<Gtk::Widget *>::iterator it = to_remove.begin (); it != to_remove.end (); it++)
			vbox->remove (**it);
		
		Gtk::Image *img = Gtk::manage (new Gtk::Image ());
		Glib::RefPtr<Gdk::PixbufAnimation> pb = Gdk::PixbufAnimation::create_from_file (dest_file);
		img->set (pb);
		img->show ();
		_window_preview->resize (pb->get_width (), pb->get_height ());
		vbox->pack_start (*img, true, true);

		GDK_THREADS_LEAVE ();

		remove (dest_file.c_str ());
	}

	void 
	Gifer::_gif_preview ()
	{
		if (_images.size () == 0)
			return;

		if (_window_preview)
			delete _window_preview;
		_window_preview = new Gtk::Dialog (_("Gifer - Preview"), true, false);
		_window_preview->set_size_request (DEFAULT_SIZE, DEFAULT_SIZE);
		_window_preview->set_position (Gtk::WIN_POS_CENTER_ALWAYS);
		_window_preview->set_icon_from_file (IMAGE_LOGO);
		
		Gtk::VBox *vbox = _window_preview->get_vbox ();

		/* Set the loading information */
		Gtk::Spinner *spinner = Gtk::manage (new Gtk::Spinner ());
		vbox->pack_start (*spinner, true, true);
		Gtk::Label *label = Gtk::manage (new Gtk::Label (_("Loading...")));
		vbox->pack_start (*label, false, false);
		_statusbar->push (_("Loading preview..."));

		spinner->show ();
		spinner->start ();
		label->show ();
		vbox->show ();
		_window_preview->show ();

		Glib::Thread::create (sigc::mem_fun (*this, &gifer::Gifer::_gif_preview_priv), true);
	}

	void 
	Gifer::_gif_generate ()
	{
		if (_images.size () == 0)
			return;

		Gtk::FileChooserDialog dialog (_("Generate as..."), Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_icon_from_file (IMAGE_LOGO);
		dialog.add_button (_("Cancel"), Gtk::RESPONSE_CANCEL);
		dialog.add_button (_("Save"), Gtk::RESPONSE_ACCEPT);
		int response = dialog.run ();

		if (response != Gtk::RESPONSE_ACCEPT)
			return;
		_gif_generate_priv (dialog.get_filename ());	
		_statusbar->push (_("GIF generated successfully"));
	}

	void
	Gifer::_loop_toggle ()
	{
		_loop = !_loop;
	}

	void 
	Gifer::_gif_generate_priv (string dest_file)
	{
		list<Magick::Image> image_list;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			{
				Magick::Image img ((*it)->get_path ());
				img.animationDelay ((*it)->get_delay ());
				img.animationIterations (_loop ? 0 : 1);
				image_list.push_back (img);
			}
		if (dest_file.size () < 4 || dest_file.substr (dest_file.size () - 4, 4) != ".gif")
			dest_file += ".gif";
		Magick::writeImages (image_list.begin (), image_list.end (), dest_file);
	}

	void 
	Gifer::_select_all ()
	{
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			(*it)->set_selected (true);	
	}

	void 
	Gifer::_unselect_all ()
	{
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			(*it)->set_selected (false);	
	}

	void 
	Gifer::_move_right ()
	{
		list<gifer::Image *>::iterator selected, next;
		int position = 0;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++, position++)
			if ((*it)->get_selected ())
				{
					selected = it;
					next = (++it);
					break;
				}

		_hbox_images->reorder_child (**selected,  position + 1);
		iter_swap (selected, next);	

		if ((position + 1) == (_images.size () - 1))
			_actions.image_move_right->set_sensitive (false);
		_actions.image_move_left->set_sensitive (true);
	}

	void 
	Gifer::_move_left ()
	{
		list<gifer::Image *>::iterator selected, last = _images.begin ();
		int position = 0;
		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++, position++)
			if ((*it)->get_selected ())
				{
					selected = it++;
					break;
				}
			else
				last = it;

		_hbox_images->reorder_child (**selected,  position - 1);
		iter_swap (selected, last);	

		if ((position - 1) == 0)
			_actions.image_move_left->set_sensitive (false);
		_actions.image_move_right->set_sensitive (true);
	}

	Gifer::Gifer ()
	{
		_loop = true;

		_main = new Gtk::Main (0, NULL);

		/* Main window */
		_window = new Gtk::Window ();
		_window->set_icon_from_file (IMAGE_LOGO);
		_window->grab_focus ();
		_window->set_can_default ();
		_window->grab_default ();
		_window->set_resizable (true);
		_window->set_position (Gtk::WIN_POS_CENTER);
		_window->set_title ("Gifer");
		_window->set_size_request (-1, DEFAULT_SIZE + 200);

		/* Main layout */
		_vbox = new Gtk::VBox ();
		_vbox->set_homogeneous (false);
		_window->add (*_vbox);

		/* Scrolled to insert images */
		_scrolled_images = new Gtk::ScrolledWindow ();
		_scrolled_images->set_policy (Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
		
		/* Images box */
		_hbox_images = new Gtk::HButtonBox ();
		_hbox_images->set_layout (Gtk::BUTTONBOX_START);
		_scrolled_images->add (*_hbox_images);

		/* About dialog */
		_create_about (); 	

		/* Menubar and toolbar*/
		_create_menubar ();

		/* Status bar */
		_statusbar = new Gtk::Statusbar ();
		_vbox->pack_start (*_menubar, Gtk::PACK_SHRINK);
		_vbox->pack_start (*_toolbar, Gtk::PACK_SHRINK);
		_vbox->pack_start (*_scrolled_images, Gtk::PACK_EXPAND_PADDING | Gtk::PACK_SHRINK, Gtk::PACK_EXPAND_PADDING | Gtk::PACK_SHRINK);
		_vbox->pack_end (*_statusbar, Gtk::PACK_SHRINK, Gtk::PACK_SHRINK);

		_statusbar->show ();
		_menubar->show ();
		_hbox_images->show ();
		_scrolled_images->show ();
		_vbox->show ();
		_window->show ();

	}

	Gifer::~Gifer ()
	{
	}

	int
	Gifer::run (int argc, char **argv)
	{
		/* Parse options */
		bool version = false, help = false;

		int c;
		while (1)
			{
				int option_index = 0;
				static struct option long_options [] = {
					{"version", 0, 0, 'v'},
					{"help", 0, 0, 'h'},
					{0, 0, 0, 0}
				};
				c = getopt_long (argc, argv, "vh", long_options, &option_index);
				if (c == -1)
					break;

				switch (c)
					{	
						case 'v':
							version = true;
							break;
						case 'h':
							help = true;
							break;
						case '?':
							cout << _("Invalid option: ") << c << endl << _("See options with gifer --help\n") << endl;
							return 1;
					}					
			}
		while (optind < argc)
			{
				cout << _("Invalid argument: ") << argv[optind] << endl << _("See options with gifer --help\n") << endl;
				return 1;	
			}
		if (help)
			{
				print_help ();
				return 0;
			}
		if (version)
			{
				print_version ();
				return 0;
			}

		/* Check threading support */
		Glib::thread_init ();
		if (!Glib::thread_supported ())
			{
				cout << _("Threading not supported: aborting.") << endl;
				return 1;
			}
		/* Run the application */
		_main->run (*_window);
	}

	bool 
	Gifer::child_clicked (GdkEventButton *event, gifer::Image *image)
	{
		gifer::Image *first_selected = NULL, *last_selected = NULL;
		int count = 0;

		for (list<gifer::Image *>::iterator it = _images.begin (); it != _images.end (); it++)
			if ((*it)->get_selected ())
				{
					last_selected = *it;
					if (count == 0)
						first_selected = *it;
					++count;
				}

		_actions.image_copy->set_sensitive (count > 0);
		_actions.image_move_left->set_sensitive (count == 1 && first_selected != _images.front ());
		_actions.image_move_right->set_sensitive (count == 1 && last_selected != _images.back ());
	}

	void
	print_help ()
	{
		cout << _("Usage: gifer [OPTION]... ") << endl;
		cout << endl;
		cout << _("Options:") << endl << endl;

		cout << _("\t-v, --version : Show the program version.") << endl;
		cout << _("\t-h, --help : Show this help.") << endl;
		cout << endl;
		cout << _("Send bugs to javier.angulo1@gmail.com") << endl;
	}

	void
	print_version ()
	{
		cout << PACKAGE_STRING << endl;
	}

};


int 
main (int argc, char **argv)
{
	bindtextdomain(GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	gifer::Gifer *app = new gifer::Gifer ();

	return app->run (argc, argv);
}
