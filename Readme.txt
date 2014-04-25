This is the README file for the LibMTP ruby wrapper version 0.0.6, November 24, 2007.

 Copyright (c) 2007 Todd Olivas (todd@topstorm.org)

 This file is part of LibMTP.

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


<< Installation >>

To build the LibMTP wrapper:

cd into the top level directory and type the following

ruby setup.rb

This creates the Makefile, runs it, and finally does the actual
installation.  The "make install" needs to copy some files to the ruby directory.
You will probably need to be root in order to do this.

This script also checks to see that the libmtp C library is installed on the system.
In addition, it also needs the libmtp.h and usb.h header files.


<< Documentation >>

The web page for this project and the reference documentation are available in the doc subdirectory.

Point your browser to the doc/index.html file to see it.


<< Testing >>

No unit tests have been developed yet.  That is the next thing to work on.




