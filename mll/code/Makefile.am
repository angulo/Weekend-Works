SUBDIRS = src po

DIST_SUBDIRS = src po

INTLTOOL_FILES = intltool-extract.in \
								 intltool-merge.in \
								 intltool-update.in

EXTRA_DIST = \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

DISTCLEANFILES = intltool-extract \
								 intltool-merge \
								 intltool-update \
								 po/.intltool-merge-cache


dist-hook:
	cp -r data doc debian $(distdir)
	rm -rf `find $(distdir)/data -type d -name .svn`
	rm -rf `find $(distdir)/doc -type d -name .svn`

install-data-local:
	install -d "$(DESTDIR)"/usr/share/pixmaps
	install -d "$(DESTDIR)"/usr/share/applications
	install -d "$(DESTDIR)"/usr/share/mll
	install data/mll.desktop "$(DESTDIR)"/usr/share/applications/mll.desktop
	install data/background.svg "$(DESTDIR)"/usr/share/mll/background.svg
	install data/selection.svg "$(DESTDIR)"/usr/share/mll/selection.svg
	install data/logo.png "$(DESTDIR)"/usr/share/pixmaps/mll.png
clean-local:
	cd doc; rm -rf html latex;
uninstall-local:
	rm -rf /usr/share/pixmaps/mll.png
	rm -rf /usr/share/applications/mll.desktop
	rm -rf /usr/share/mll
html-local:
	cd doc; doxygen doxygen.conf;
pdf-local:
	cd doc; doxygen doxygen.conf; cd latex; make;

