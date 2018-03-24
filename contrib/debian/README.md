
Debian
====================
This directory contains files used to package prufusd/prufus-qt
for Debian-based Linux systems. If you compile prufusd/prufus-qt yourself, there are some useful files here.

## prufus: URI support ##


prufus-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install prufus-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your prufusqt binary to `/usr/bin`
and the `../../share/pixmaps/prufus128.png` to `/usr/share/pixmaps`

prufus-qt.protocol (KDE)

