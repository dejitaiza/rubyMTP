/**********************************************************************

 libMTP ruby extension

 Copyright (c) 2007 Todd Olivas (todd@topstorm.org)

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

 See LibMTP for detailed documentation.

 Download: http://rubyforge.org/projects/libmtp/

 History:
 v0.0.1        alpha code      Wed Jan 24 23:23:00 EST 2007

**********************************************************************/

#include <string.h>

#include <stdlib.h>

#include "mtp_proto.h"


VALUE mLibMTP;


/*
 *  call-seq:
 *     LibMTP::filetype_desc(type) -> Filetype description string
 *  
 *  Returns a filetype string associated with a filetype value.
 *
 *  Wraps: <i>LIBMTP_Get_Filetype_Description</i>
 *
 */

static VALUE mtp_filetype_desc(VALUE self, VALUE ftype)
{
  const char *desc_ptr;

  VALUE desc = Qnil;


  desc_ptr = LIBMTP_Get_Filetype_Description((LIBMTP_filetype_t)FIX2UINT(ftype));

  if(desc_ptr != NULL)
  {
    desc = rb_str_new2(desc_ptr);
  }


  return desc;
}


/*
 *  call-seq:
 *     LibMTP::entry_list() -> Array of LibMTP::Entry objects.
 *  
 *  Returns an array of LibMTP::Entry objects from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Supported_Devices_List</i>
 *
 */

static VALUE mtp_entry_list(VALUE self)
{
  LIBMTP_device_entry_t *entry_ptr = NULL;

  LIBMTP_device_entry_t *current;

  VALUE array = rb_ary_new();

  int count = 0;

  int status;

  int i;


  status = LIBMTP_Get_Supported_Devices_List(&entry_ptr, &count);

  if((status == 0) && (entry_ptr != NULL))
  {
    for(i=0; i < count; i++)
    {
      current = (LIBMTP_device_entry_t *)malloc(sizeof(LIBMTP_device_entry_t));

      current->vendor_id    = entry_ptr[i].vendor_id;

      current->product_id   = entry_ptr[i].product_id;

      current->device_flags = entry_ptr[i].device_flags;

      if(entry_ptr[i].vendor != NULL)
      {
	current->vendor = strdup(entry_ptr[i].vendor);
      }

      if(entry_ptr[i].product != NULL)
      {
	current->product = strdup(entry_ptr[i].product);
      }

      rb_ary_push(array, Get_LibMTP_Entry(current));
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get device entry listing");
  }


  return array;
}


/*
 *  Document-module: LibMTP
 *
 *  The LibMTP module contains a set of classes that wrap the functionality of the libmtp C library.
 *  This library implements the Media Transfer Protocol (MTP) which can be used to communicate with
 *  various MP3 players and other devices that implement this protocol over USB.
 *
 *  The classes contained in this module are the Album, Device, Entry, File, Folder, Playlist, Storage, and Track.
 *  Each of these classes wraps a C data structure from the libmtp C library.
 *
 *  A list of devices currently supported by libmtp can be obtained by calling
 *
 *  array = LibMTP::entry_list()
 *
 *  array.each do |entry|
 *
 *    puts "Vendor ID: #{entry['vendor_id']}, Product ID: #{entry['product_id']}, Name: #{entry['name']}\n\n"
 *
 *  end
 *
 *  In addition, a list of filetypes supported by a device can be obtained by calling
 *
 *  LibMTP::connect do |device|
 *
 *    array = device.supported_filetypes()
 *
 *    array.sort.each do |type|
 *
 *      puts "Type: #{type}, Description: #{LibMTP::filetype_desc(type)}\n\n"
 *
 *    end
 *
 *  end
 *
 *  The LibMTP::connect() function is used to connect to an MTP device on a USB port.  Currently, the libmtp C library
 *  only supports connecting to the first MTP device that it finds.  See the documentation for the LibMTP::Device
 *  object for a list of operations that can be performed once a device is connected.
 *
 *  For more information, see the documentation that is provided with <i>libmtp</i>.
 *
 *  Or, see the libmtp homepage at http://libmtp.sourceforge.net/
 *
 */

void Init_LibMTPBase()
{
  LIBMTP_Init();


  mLibMTP = rb_define_module("LibMTP");


  rb_define_module_function(mLibMTP, "filetype_desc", mtp_filetype_desc, 1);

  rb_define_module_function(mLibMTP, "entry_list", mtp_entry_list, 0);


  rb_define_const(mLibMTP, "FILETYPE_WAV",                INT2FIX(LIBMTP_FILETYPE_WAV));

  rb_define_const(mLibMTP, "FILETYPE_MP3",                INT2FIX(LIBMTP_FILETYPE_MP3));

  rb_define_const(mLibMTP, "FILETYPE_WMA",                INT2FIX(LIBMTP_FILETYPE_WMA));

  rb_define_const(mLibMTP, "FILETYPE_OGG",                INT2FIX(LIBMTP_FILETYPE_OGG));

  rb_define_const(mLibMTP, "FILETYPE_AUDIBLE",            INT2FIX(LIBMTP_FILETYPE_AUDIBLE));

  rb_define_const(mLibMTP, "FILETYPE_MP4",                INT2FIX(LIBMTP_FILETYPE_MP4));

  rb_define_const(mLibMTP, "FILETYPE_UNDEF_AUDIO",        INT2FIX(LIBMTP_FILETYPE_UNDEF_AUDIO));

  rb_define_const(mLibMTP, "FILETYPE_WMV",                INT2FIX(LIBMTP_FILETYPE_WMV));

  rb_define_const(mLibMTP, "FILETYPE_AVI",                INT2FIX(LIBMTP_FILETYPE_AVI));

  rb_define_const(mLibMTP, "FILETYPE_MPEG",               INT2FIX(LIBMTP_FILETYPE_MPEG));

  rb_define_const(mLibMTP, "FILETYPE_ASF",                INT2FIX(LIBMTP_FILETYPE_ASF));

  rb_define_const(mLibMTP, "FILETYPE_QT",                 INT2FIX(LIBMTP_FILETYPE_QT));

  rb_define_const(mLibMTP, "FILETYPE_UNDEF_VIDEO",        INT2FIX(LIBMTP_FILETYPE_UNDEF_VIDEO));

  rb_define_const(mLibMTP, "FILETYPE_JPEG",               INT2FIX(LIBMTP_FILETYPE_JPEG));

  rb_define_const(mLibMTP, "FILETYPE_JFIF",               INT2FIX(LIBMTP_FILETYPE_JFIF));

  rb_define_const(mLibMTP, "FILETYPE_TIFF",               INT2FIX(LIBMTP_FILETYPE_TIFF));

  rb_define_const(mLibMTP, "FILETYPE_BMP",                INT2FIX(LIBMTP_FILETYPE_BMP));

  rb_define_const(mLibMTP, "FILETYPE_GIF",                INT2FIX(LIBMTP_FILETYPE_GIF));

  rb_define_const(mLibMTP, "FILETYPE_PICT",               INT2FIX(LIBMTP_FILETYPE_PICT));

  rb_define_const(mLibMTP, "FILETYPE_PNG",                INT2FIX(LIBMTP_FILETYPE_PNG));

  rb_define_const(mLibMTP, "FILETYPE_VCALENDAR1",         INT2FIX(LIBMTP_FILETYPE_VCALENDAR1));

  rb_define_const(mLibMTP, "FILETYPE_VCALENDAR2",         INT2FIX(LIBMTP_FILETYPE_VCALENDAR2));

  rb_define_const(mLibMTP, "FILETYPE_VCARD2",             INT2FIX(LIBMTP_FILETYPE_VCARD2));

  rb_define_const(mLibMTP, "FILETYPE_VCARD3",             INT2FIX(LIBMTP_FILETYPE_VCARD3));

  rb_define_const(mLibMTP, "FILETYPE_WINDOWSIMAGEFORMAT", INT2FIX(LIBMTP_FILETYPE_WINDOWSIMAGEFORMAT));

  rb_define_const(mLibMTP, "FILETYPE_WINEXEC",            INT2FIX(LIBMTP_FILETYPE_WINEXEC));

  rb_define_const(mLibMTP, "FILETYPE_TEXT",               INT2FIX(LIBMTP_FILETYPE_TEXT));

  rb_define_const(mLibMTP, "FILETYPE_HTML",               INT2FIX(LIBMTP_FILETYPE_HTML));

  rb_define_const(mLibMTP, "FILETYPE_FIRMWARE",           INT2FIX(LIBMTP_FILETYPE_FIRMWARE));

  rb_define_const(mLibMTP, "FILETYPE_AAC",                INT2FIX(LIBMTP_FILETYPE_AAC));

  rb_define_const(mLibMTP, "FILETYPE_MEDIACARD",          INT2FIX(LIBMTP_FILETYPE_MEDIACARD));

  rb_define_const(mLibMTP, "FILETYPE_FLAC",               INT2FIX(LIBMTP_FILETYPE_FLAC));

  rb_define_const(mLibMTP, "FILETYPE_MP2",                INT2FIX(LIBMTP_FILETYPE_MP2));

  rb_define_const(mLibMTP, "FILETYPE_M4A",                INT2FIX(LIBMTP_FILETYPE_M4A));

  rb_define_const(mLibMTP, "FILETYPE_DOC",                INT2FIX(LIBMTP_FILETYPE_DOC));

  rb_define_const(mLibMTP, "FILETYPE_XML",                INT2FIX(LIBMTP_FILETYPE_XML));

  rb_define_const(mLibMTP, "FILETYPE_XLS",                INT2FIX(LIBMTP_FILETYPE_XLS));

  rb_define_const(mLibMTP, "FILETYPE_PPT",                INT2FIX(LIBMTP_FILETYPE_PPT));

  rb_define_const(mLibMTP, "FILETYPE_MHT",                INT2FIX(LIBMTP_FILETYPE_MHT));

  rb_define_const(mLibMTP, "FILETYPE_JP2",                INT2FIX(LIBMTP_FILETYPE_JP2));

  rb_define_const(mLibMTP, "FILETYPE_JPX",                INT2FIX(LIBMTP_FILETYPE_JPX));

  rb_define_const(mLibMTP, "FILETYPE_UNKNOWN",            INT2FIX(LIBMTP_FILETYPE_UNKNOWN));


  rb_define_const(mLibMTP, "STORAGE_NOTSORTED",           INT2FIX(LIBMTP_STORAGE_SORTBY_NOTSORTED));

  rb_define_const(mLibMTP, "STORAGE_FREESPACE",           INT2FIX(LIBMTP_STORAGE_SORTBY_FREESPACE));

  rb_define_const(mLibMTP, "STORAGE_MAXSPACE",            INT2FIX(LIBMTP_STORAGE_SORTBY_MAXSPACE));


  Init_LibMTP_Track();

  Init_LibMTP_File();

  Init_LibMTP_Folder();

  Init_LibMTP_Playlist();

  Init_LibMTP_Album();


  Init_LibMTP_Entry();

  Init_LibMTP_Storage();

  Init_LibMTP_Device();


  return;
}
