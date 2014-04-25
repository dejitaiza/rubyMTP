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


static VALUE cMTPDevice;


static void device_free(void *device)
{
  LIBMTP_Release_Device((LIBMTP_mtpdevice_t *)device);


  return;
}


static VALUE device_alloc(VALUE klass)
{
  LIBMTP_mtpdevice_t *device;

  VALUE obj = Qnil;


  device = LIBMTP_Get_First_Device();

  if(device != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, device_free, device);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create device");
  }


  return obj;
}


static VALUE device_init(VALUE self)
{
  return self;
}


/*
 *  call-seq:
 *     device.dump() -> device
 *
 *  Displays a listing of device information on standard output.
 *
 *  Wraps: <i>LIBMTP_Dump_Device_Info</i>
 *
 */

static VALUE device_dump(VALUE self)
{
  LIBMTP_mtpdevice_t *device;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  LIBMTP_Dump_Device_Info(device);


  return self;
}


/*
 *  call-seq:
 *     device.dump_errors() -> device
 *
 *  Displays a listing of error information on standard output.
 *
 *  Wraps: <i>LIBMTP_Dump_Errorstack</i>
 *
 */

static VALUE device_dump_errors(VALUE self)
{
  LIBMTP_mtpdevice_t *device;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  LIBMTP_Dump_Errorstack(device);


  return self;
}


/*
 *  call-seq:
 *     device.reset_errors() -> device
 *
 *  Resets the error information for a device.
 *
 *  Wraps: <i>LIBMTP_Clear_Errorstack</i>
 *
 */

static VALUE device_reset_errors(VALUE self)
{
  LIBMTP_mtpdevice_t *device;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  LIBMTP_Clear_Errorstack(device);


  return self;
}


/*
 *  call-seq:
 *     device.reset() -> device
 *
 *  Resets the device if possible.  Raises I/O Exception on failure.
 *
 *  Wraps: <i>LIBMTP_Reset_Device</i>
 *
 */

static VALUE device_reset(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Reset_Device(device);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to reset device");
  }


  return self;
}


/*
 *  call-seq:
 *     device.model_name() -> model name string
 *
 *  Returns the model name of an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Modelname</i>
 *
 */

static VALUE device_model_name(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  VALUE name = Qnil;

  char *name_ptr;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  name_ptr = LIBMTP_Get_Modelname(device);

  if(name_ptr != NULL)
  {
    name = rb_str_new2(name_ptr);

    free(name_ptr);
  }


  return name;
}


/*
 *  call-seq:
 *     device.serial_number() -> Serial number string
 *
 *  Returns the serial number of an MTP device as a string.
 *
 *  Wraps: <i>LIBMTP_Get_Serialnumber</i>
 *
 */

static VALUE device_serial_number(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  VALUE serial_number = Qnil;

  char *serial_number_ptr;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  serial_number_ptr = LIBMTP_Get_Serialnumber(device);

  if(serial_number_ptr != NULL)
  {
    serial_number = rb_str_new2(serial_number_ptr);

    free(serial_number_ptr);
  }


  return serial_number;
}


/*
 *  call-seq:
 *     device.version() -> version string
 *
 *  Returns the hardware and firmware version of an MTP device as a string
 *
 *  Wraps: <i>LIBMTP_Get_Deviceversion</i>
 *
 */

static VALUE device_version(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  VALUE version = Qnil;

  char *version_ptr;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  version_ptr = LIBMTP_Get_Deviceversion(device);

  if(version_ptr != NULL)
  {
    version = rb_str_new2(version_ptr);

    free(version_ptr);
  }


  return version;
}


/*
 *  call-seq:
 *     device.friendly_name() -> Friendly name string
 *
 *  Returns the <i>friendly name</i> of an MTP device as a string.
 *  This is a name assigned by the user to a device.
 *
 *  Wraps: <i>LIBMTP_Get_Friendlyname</i>
 *
 */

static VALUE device_friendly_name(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  VALUE name = Qnil;

  char *name_ptr;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  name_ptr = LIBMTP_Get_Friendlyname(device);

  if(name_ptr != NULL)
  {
    name = rb_str_new2(name_ptr);

    free(name_ptr);
  }


  return name;
}


/*
 *  call-seq:
 *     device.friendly_name = string  -> device
 *
 *  Sets the <i>friendly name</i> string for a device.
 *
 *  Wraps: <i>LIBMTP_Set_Friendlyname</i>
 *
 */

static VALUE device_set_friendly_name(VALUE self, VALUE orig_name)
{
  LIBMTP_mtpdevice_t *device;

  char *name_ptr;

  int status;

  VALUE name;


  name = StringValue(orig_name);

  name_ptr = RSTRING(name)->as.heap.ptr;

  if((name_ptr != NULL) && (RSTRING(name)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);


    status = LIBMTP_Set_Friendlyname(device, name_ptr);

    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to set friendly name");
    }
  }


  return self;
}


/*
 *  call-seq:
 *     device.sync_partner() -> Synchronization partner string
 *
 *  Returns the synchronization partner of an MTP device as a string.
 *
 *  Wraps: <i>LIBMTP_Get_Syncpartner</i>
 *
 */

static VALUE device_sync_partner(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  VALUE sync_partner = Qnil;

  char *sync_partner_ptr;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  sync_partner_ptr = LIBMTP_Get_Syncpartner(device);

  if(sync_partner_ptr != NULL)
  {
    sync_partner = rb_str_new2(sync_partner_ptr);

    free(sync_partner_ptr);
  }


  return sync_partner;
}


/*
 *  call-seq:
 *     device.sync_partner = string -> device
 *
 *  Sets the synchronization partner of an MTP device as a string.
 *
 *  Wraps: <i>LIBMTP_Set_Syncpartner</i>
 *
 */

static VALUE device_set_sync_partner(VALUE self, VALUE orig_sync_partner)
{
  LIBMTP_mtpdevice_t *device;

  char *sync_partner_ptr;

  VALUE sync_partner;

  int status;


  sync_partner = StringValue(orig_sync_partner);

  sync_partner_ptr = RSTRING(sync_partner)->as.heap.ptr;

  if((sync_partner_ptr != NULL) && (RSTRING(sync_partner)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);


    status = LIBMTP_Set_Syncpartner(device, sync_partner_ptr);

    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to set sync partner");
    }
  }


  return self;
}


/*
 *  call-seq:
 *     device.battery_level() -> Array [maximum level, current level]
 *
 *  Returns the maximum and current battery level as a two element array.
 *
 *  Wraps: <i>LIBMTP_Get_Batterylevel</i>
 *
 */

static VALUE device_battery_level(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  uint8_t max, current;

  VALUE hash = Qnil;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Get_Batterylevel(device, &max, &current);

  if(status == 0)
  {
    hash = rb_hash_new();

    rb_hash_aset(hash, rb_str_new2("Max Level"), INT2NUM(max));

    rb_hash_aset(hash, rb_str_new2("Current Level"), INT2NUM(current));
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get battery level");
  }


  return hash;
}


/*
 *  call-seq:
 *     device.secure_time() -> Secure time string.
 *
 *  Returns the secure time as a string that contains an XML document.
 *
 *  Wraps: <i>LIBMTP_Get_Secure_Time</i>
 *
 */

static VALUE device_secure_time(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  char *time_ptr = NULL;

  VALUE time = Qnil;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Get_Secure_Time(device, &time_ptr);

  if((status == 0) && (time_ptr != NULL))
  {
    time = rb_str_new2(time_ptr);

    free(time_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get secure time");
  }


  return time;
}


/*
 *  call-seq:
 *     device.certificate() -> Public key certificate string
 *
 *  Returns a string that contains the public key certificate for an MTP device.
 *  This string contains an XML document.
 *
 *  Wraps: <i>LIBMTP_Get_Device_Certificate</i>
 *
 */

static VALUE device_certificate(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  char *certificate_ptr = NULL;

  VALUE certificate = Qnil;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Get_Device_Certificate(device, &certificate_ptr);

  if((status == 0) && (certificate_ptr != NULL))
  {
    certificate = rb_str_new2(certificate_ptr);

    free(certificate_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get secure certificate");
  }


  return certificate;
}


/*
 *  call-seq:
 *     device.supported_filetypes() -> Array of supported filetypes.
 *
 *  Returns an array of supported filetypes for an MTP device.
 *  The filetypes are defined as constants in the LibMTP module.
 *  A filetype can be converted to a string by calling LibMTP::filetype_desc().
 *
 *  Wraps: <i>LIBMTP_Get_Supported_Filetypes</i>
 *
 */

static VALUE device_supported_filetypes(VALUE self)
{
  LIBMTP_mtpdevice_t *device;

  uint16_t *filetypes_ptr = NULL;

  uint16_t filetypes_length = 0;

  VALUE filetypes = Qnil;

  int status;

  int i;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Get_Supported_Filetypes(device, &filetypes_ptr, &filetypes_length);

  if((status == 0) && (filetypes_ptr != NULL) && (filetypes_length > 0))
  {
    filetypes = rb_ary_new();

    for(i=0; i < filetypes_length; i++)
    {
      rb_ary_push(filetypes, INT2FIX(filetypes_ptr[i]));
    }

    free(filetypes_ptr);
  }
  else
  {
    if(filetypes_ptr != NULL)
    {
      free(filetypes_ptr);
    }


    rb_raise(rb_eIOError, "Unable to get secure certificate");
  }


  return filetypes;
}


/*
 *  call-seq:
 *     device.storage() -> Array of Storage objects.
 *
 *  Returns an array of LibMTP::Storage objects.
 *
 *  Wraps: <i>LIBMTP_Get_Storage</i>
 *
 */

static VALUE device_storage(VALUE self, VALUE sort_by)
{
  LIBMTP_devicestorage_t *current;

  LIBMTP_mtpdevice_t *device;

  VALUE array = rb_ary_new();

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Get_Storage(device, FIX2INT(sort_by));

  if(status == 0)
  {
    current = device->storage;

    while(current != NULL)
    {
      rb_ary_push(array, mtp_storage_create_with_copy(current));

      current = current->next;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get storage list");
  }


  return array;
}


/*
 *  call-seq:
 *     device.delete_object(id) -> device
 *
 *  Deletes the object with the specified ID from an MTP device.
 *  The object could be a track, a file, a playlist, etc.
 *
 *  Wraps: <i>LIBMTP_Delete_Object</i>
 *
 */

static VALUE device_delete_object(VALUE self, VALUE id)
{
  LIBMTP_mtpdevice_t *device;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device);

  status = LIBMTP_Delete_Object(device, NUM2UINT(id));

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to delete object");
  }


  return self;
}


/*
 *  call-seq:
 *     device.album_get(id) -> LibMTP::Album object.
 *
 *  Returns the LibMTP::Album object with the specified album ID from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Album</i>
 *
 */

static VALUE device_album_get(VALUE self, VALUE id)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_album_t *album_ptr;

  VALUE obj = Qnil;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  album_ptr = LIBMTP_Get_Album(device_ptr, NUM2UINT(id));

  if(album_ptr != NULL)
  {
    obj = Wrap_LibMTP_Album(album_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get album");
  }


  return obj;
}


/*
 *  call-seq:
 *     device.album_list() -> Array of LibMTP::Album objects.
 *
 *  Returns an array of LibMTP::Album objects from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Album_List</i>
 *
 */

static VALUE device_album_list(VALUE self)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_album_t *album_ptr;

  LIBMTP_album_t *current;

  VALUE array = rb_ary_new();


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  album_ptr = LIBMTP_Get_Album_List(device_ptr);

  if(album_ptr != NULL)
  {
    current = album_ptr;

    while(current != NULL)
    {
      rb_ary_push(array, Wrap_LibMTP_Album(current));

      current = current->next;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get album list");
  }


  return array;
}


/*
 *  call-seq:
 *     device.album_create(parent, album) -> device
 *
 *  Creates an album with the specified parent on an MTP device for the specified LibMTP::Album object.
 *
 *  Wraps: <i>LIBMTP_Create_New_Album</i>
 *
 */

static VALUE device_album_create(VALUE self, VALUE parent, VALUE album)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_album_t *album_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  Data_Get_Struct(album, LIBMTP_album_t, album_ptr);

  status = LIBMTP_Create_New_Album(device_ptr, album_ptr);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to create album");
  }


  return self;
}


/*
 *  call-seq:
 *     device.album_update(album) -> device
 *
 *  Updates the album information on an MTP device for the specified LibMTP::Album object.
 *
 *  Wraps: <i>LIBMTP_Update_Album</i>
 *
 */

static VALUE device_album_update(VALUE self, VALUE album)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_album_t *album_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  Data_Get_Struct(album, LIBMTP_album_t, album_ptr);

  status = LIBMTP_Update_Album(device_ptr, album_ptr);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to send album");
  }


  return self;
}


/*
 *  call-seq:
 *     device.file_info_get(id) -> LibMTP::File object.
 *
 *  Returns the LibMTP::File object with the specified file ID.
 *
 *  Wraps: <i>LIBMTP_Get_Filemetadata</i>
 *
 */

static VALUE device_file_info_get(VALUE self, VALUE id)
{
  LIBMTP_file_t *file_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  VALUE obj = Qnil;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  file_ptr = LIBMTP_Get_Filemetadata(device_ptr, NUM2UINT(id));

  if(file_ptr != NULL)
  {
    obj = Wrap_LibMTP_File(file_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get file metadata");
  }


  return obj;
}


/*
 *  call-seq:
 *     device.file_info_list() -> Array of LibMTP::File objects.
 *
 *  Returns an array of LibMTP::File objects from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Filelisting_With_Callback</i>
 *
 */

static VALUE device_file_info_list(VALUE self)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_file_t *file_ptr;

  LIBMTP_file_t *current;

  VALUE array = rb_ary_new();


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  file_ptr = LIBMTP_Get_Filelisting_With_Callback(device_ptr, NULL, NULL);

  if(file_ptr != NULL)
  {
    current = file_ptr;

    while(current != NULL)
    {
      rb_ary_push(array, Wrap_LibMTP_File(current));

      current = current->next;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get file metadata listing");
  }


  return array;
}


/*
 *  call-seq:
 *     device.file_get(id, pathname) -> device
 *
 *  Retrieves the file with the specified file ID and writes it to the specified path.
 *
 *  Wraps: <i>LIBMTP_Get_File_To_File</i>
 *
 */

static VALUE device_file_get(VALUE self, VALUE id, VALUE pathname)
{
  LIBMTP_mtpdevice_t *device_ptr;

  VALUE path;

  int status;


  path = StringValue(pathname);

  if((RSTRING(path)->as.heap.ptr != NULL) && (RSTRING(path)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

    status = LIBMTP_Get_File_To_File(device_ptr, NUM2UINT(id), RSTRING(path)->as.heap.ptr, NULL, NULL);

    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to retrieve file");
    }
  }


  return self;
}


/*
 *  call-seq:
 *     device.file_send(parent, pathname, file) -> device
 *
 *  Sends the file specified by <i>pathname</i> to an MTP device with the metadata specified by the LibMTP::File
 *  object <i>file</i>.  The file will be a child of the object with the given ID specifed by <i>parent</i>.
 *
 *  If <i>file</i> contains a hash, a LibMTP::File object will be created from the hash data.
 *
 *  Wraps: <i>LIBMTP_Send_File_From_File</i>
 *
 */

static VALUE device_file_send(VALUE self, VALUE parent, VALUE pathname, VALUE file)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_file_t *file_ptr;

  VALUE path;

  int status;


  path = StringValue(pathname);

  if((RSTRING(path)->as.heap.ptr != NULL) && (RSTRING(path)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

    Data_Get_Struct(Get_LibMTP_File(file), LIBMTP_file_t, file_ptr);

    status = LIBMTP_Send_File_From_File(device_ptr, RSTRING(path)->as.heap.ptr, file_ptr, NULL, NULL);

    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to send file");
    }
  }


  return self;
}


/*
 *  call-seq:
 *     device.folder_list() -> Array of LibMTP::Folder objects.
 *
 *  Returns an array of LibMTP::Folder from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Folder_List</i>
 *
 */

static VALUE device_folder_list(VALUE self)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_folder_t *folder_ptr;

  LIBMTP_folder_t *current;

  VALUE array = rb_ary_new();


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  folder_ptr = LIBMTP_Get_Folder_List(device_ptr);

  if(folder_ptr != NULL)
  {
    current = folder_ptr;

    while(current != NULL)
    {
      rb_ary_push(array, Wrap_LibMTP_Folder(current));

      current = current->sibling;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get folder listing");
  }


  return array;
}


/*
 *  call-seq:
 *     device.folder_create(parent, name) -> folder ID
 *
 *  Creates a folder with the specified name and parent ID on an MTP device.
 *
 *  Returns the folder ID of the created folder.
 *
 *  Wraps: <i>LIBMTP_Create_Folder</i>
 *
 */

static VALUE device_folder_create(VALUE self, VALUE parent, VALUE name)
{
  LIBMTP_mtpdevice_t *device_ptr;

  VALUE folder = Qnil;

  VALUE string;

  uint32_t id;


  string = StringValue(name);

  if((RSTRING(string)->as.heap.ptr != NULL) && (RSTRING(string)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

    id = LIBMTP_Create_Folder(device_ptr, RSTRING(string)->as.heap.ptr,0,0);


    if(id != 0)
    {
      folder = UINT2NUM(id);
    }
    else
    {
      rb_raise(rb_eIOError, "Unable to create folder");
    }
  }


  return folder;
}


/*
 *  call-seq:
 *     device.playlist_get(id) -> LibMTP::Playlist object.
 *
 *  Returns the LibMTP::Playlist object with the specified ID from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Playlist</i>
 *
 */

static VALUE device_playlist_get(VALUE self, VALUE id)
{
  LIBMTP_playlist_t *playlist_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  VALUE obj = Qnil;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  playlist_ptr = LIBMTP_Get_Playlist(device_ptr, NUM2UINT(id));

  if(playlist_ptr != NULL)
  {
    obj = Wrap_LibMTP_Playlist(playlist_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get playlist");
  }


  return obj;
}


/*
 *  call-seq:
 *     device.playlist_list() -> Array of LibMTP::Playlist objects.
 *
 *  Returns an array of LibMTP::Playlist objects from an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Playlist_List</i>
 *
 */

static VALUE device_playlist_list(VALUE self)
{
  LIBMTP_playlist_t *playlist_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_playlist_t *current;

  VALUE array = rb_ary_new();


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  playlist_ptr = LIBMTP_Get_Playlist_List(device_ptr);

  if(playlist_ptr != NULL)
  {
    current = playlist_ptr;

    while(current != NULL)
    {
      rb_ary_push(array, Wrap_LibMTP_Playlist(current));

      current = current->next;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get playlist list");
  }


  return array;
}


/*
 *  call-seq:
 *     device.playlist_create(parent, playlist) -> device
 *
 *  Creates the specified playlist on an MTP device with the specified parent ID.
 *
 *  Wraps: <i>LIBMTP_Create_New_Playlist</i>
 *
 */

static VALUE device_playlist_create(VALUE self, VALUE parent, VALUE playlist)
{
  LIBMTP_playlist_t *playlist_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  Data_Get_Struct(playlist, LIBMTP_playlist_t, playlist_ptr);

  status = LIBMTP_Create_New_Playlist(device_ptr, playlist_ptr);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to create playlist");
  }


  return self;
}


/*
 *  call-seq:
 *     device.playlist_update(playlist) -> device
 *
 *  Updates a playlist on an MTP device.
 *
 *  Wraps: <i>LIBMTP_Update_Playlist</i>
 *
 */

static VALUE device_playlist_update(VALUE self, VALUE playlist)
{
  LIBMTP_playlist_t *playlist_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  Data_Get_Struct(playlist, LIBMTP_playlist_t, playlist_ptr);

  status = LIBMTP_Update_Playlist(device_ptr, playlist_ptr);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to send playlist");
  }


  return self;
}


/*
 *  call-seq:
 *     device.track_get(id) -> LibMTP::Track object
 *
 *  Returns the Track object with the specified ID.
 *
 *  Wraps: <i>LIBMTP_Get_Trackmetadata</i>
 *
 */

static VALUE device_track_get(VALUE self, VALUE id)
{
  LIBMTP_track_t *track_ptr;

  LIBMTP_mtpdevice_t *device_ptr;

  VALUE obj = Qnil;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  track_ptr = LIBMTP_Get_Trackmetadata(device_ptr, NUM2UINT(id));

  if(track_ptr != NULL)
  {
    obj = Wrap_LibMTP_Track(track_ptr);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get track metadata");
  }


  return obj;
}


/*
 *  call-seq:
 *     device.track_list() -> Array of LibMTP::Track objects
 *
 *  Returns an array of all the tracks on an MTP device.
 *
 *  Wraps: <i>LIBMTP_Get_Tracklisting_With_Callback</i>
 *
 */

static VALUE device_track_list(VALUE self)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_track_t *track_ptr;

  LIBMTP_track_t *current;

  VALUE array = rb_ary_new();


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  track_ptr = LIBMTP_Get_Tracklisting_With_Callback(device_ptr, NULL, NULL);

  if(track_ptr != NULL)
  {
    current = track_ptr;

    while(current != NULL)
    {
      rb_ary_push(array, Wrap_LibMTP_Track(current));

      current = current->next;
    }
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to get track metadata listing");
  }


  return array;
}


/*
 *  call-seq:
 *     device.track_update(track) -> device
 *
 *  Updates the metadata for a Track.
 *
 *  If <i>track</i> contains a hash, a LibMTP::Track object will be created from the hash data.
 *
 *  Wraps: <i>LIBMTP_Update_Track_Metadata</i>
 *
 */

static VALUE device_track_update(VALUE self, VALUE track)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_track_t *track_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  Data_Get_Struct(Get_LibMTP_Track(track), LIBMTP_track_t, track_ptr);

  status = LIBMTP_Update_Track_Metadata(device_ptr, track_ptr);

  if(status != 0)
  {
    rb_raise(rb_eIOError, "Unable to send track");
  }


  return self;
}


/*
 *  call-seq:
 *     device.track_exists?(id) -> True or False
 *
 *  Returns True if a track with the specified ID exists.
 *
 *  Wraps: <i>LIBMTP_Track_Exists</i>
 *
 */

static VALUE device_track_exists(VALUE self, VALUE id)
{
  LIBMTP_mtpdevice_t *device_ptr;

  int status;


  Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

  status = LIBMTP_Track_Exists(device_ptr, NUM2UINT(id));


  return ((status) ? Qtrue : Qfalse);
}


/*
 *  call-seq:
 *     device.track_get_file(id, pathname) -> device
 *
 *  Retrieves the file with the specified track ID and writes it to the specified path.
 *
 *  Wraps: <i>LIBMTP_Get_Track_To_File</i>
 *
 */

static VALUE device_track_get_file(VALUE self, VALUE id, VALUE pathname)
{
  LIBMTP_mtpdevice_t *device_ptr;

  VALUE path;

  int status;


  path = StringValue(pathname);

  if((RSTRING(path)->as.heap.ptr != NULL) && (RSTRING(path)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

    status = LIBMTP_Get_Track_To_File(device_ptr, NUM2UINT(id), RSTRING(path)->as.heap.ptr, NULL, NULL);

    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to retrieve file");
    }
  }


  return self;
}


/*
 *  call-seq:
 *     device.track_send_file(parent, pathname, track) -> device
 *
 *  Sends the file specified by <i>path</i> with the track metadata specified by <i>track</i>.
 *
 *  If <i>track</i> contains a hash, a LibMTP::Track object will be created from the hash data.
 *
 *  Wraps: <i>LIBMTP_Send_Track_From_File</i>
 *
 */

static VALUE device_track_send_file(VALUE self, VALUE parent, VALUE pathname, VALUE track)
{
  LIBMTP_mtpdevice_t *device_ptr;

  LIBMTP_track_t *track_ptr;

  VALUE path;

  int status;


  path = StringValue(pathname);

  if((RSTRING(path)->as.heap.ptr != NULL) && (RSTRING(path)->as.heap.len > 0))
  {
    Data_Get_Struct(self, LIBMTP_mtpdevice_t, device_ptr);

    Data_Get_Struct(Get_LibMTP_Track(track), LIBMTP_track_t, track_ptr);

    status = LIBMTP_Send_Track_From_File(device_ptr, RSTRING(path)->as.heap.ptr, track_ptr, NULL, NULL);
    if(status != 0)
    {
      rb_raise(rb_eIOError, "Unable to send track");
    }
  }


  return self;
}



static VALUE device_list(VALUE klass)
{
  LIBMTP_mtpdevice_t *list = NULL;

  LIBMTP_error_number_t status;

  LIBMTP_mtpdevice_t *current;

  VALUE array = Qnil;


  status = LIBMTP_Get_Connected_Devices(&list);

  if(status == LIBMTP_ERROR_NONE)
  {
    array = rb_ary_new();

    while(list != NULL)
    {
      current = list;

      list = list->next;


      current->next = NULL;

      rb_ary_push(array, Data_Wrap_Struct(cMTPDevice, 0, device_free, current));
    }
  }
  else if(status == LIBMTP_ERROR_NO_DEVICE_ATTACHED)
  {
    array = rb_ary_new();
  }
  else if(status == LIBMTP_ERROR_CONNECTING)
  {
    rb_raise(rb_eIOError, "Unable to connect to device");
  }
  else if(status == LIBMTP_ERROR_MEMORY_ALLOCATION)
  {
    rb_raise(rb_eIOError, "Unable to allocate memory");
  }
  else if(status != LIBMTP_ERROR_NONE)
  {
    rb_raise(rb_eIOError, "Unknown error");
  }


  return array;
}


/*
 *  Document-class: LibMTP::Device
 *
 *  A <code>Device</code> object holds a connection to an MTP device.  Typically, a <code>Device</code> object
 *  is created by calling LibMTP::connect.  Currently, libmtp will simply connect to the first MTP device that
 *  it finds on a USB port, and there is no way to connect to multiple devices or to specify a device.
 *
 *  Once a <code>Device</code> object is created, it can be used to send and receive data on the connected
 *  MTP device.  For example, to display a list of supported filetypes for a device, you could do the following:
 *
 *  <code>LibMTP::connect do |device|</code>
 *
 *  <code>  array = device.supported_filetypes</code>
 *
 *  <code>  array.sort.each do |type|</code>
 *
 *  <code>    puts "Type: #{type}, Description: #{LibMTP::filetype_desc(type)}\n\n"</code>
 *
 *  <code>  end</code>
 *
 *  <code>end</code>
 *
 *  The MTP device will be released at the end of the connect block.
 *
 *  For more information, see the documentation that is provided with <i>libmtp</i>.
 *
 *  Or, see the libmtp homepage at http://libmtp.sourceforge.net/
 *
 */

void Init_LibMTP_Device(void)
{
  cMTPDevice = rb_define_class_under(mLibMTP, "Device", rb_cObject); /* in mtp_main.c */

  rb_define_alloc_func(cMTPDevice, device_alloc);


  rb_define_method(cMTPDevice, "initialize", device_init, 0);

  rb_define_method(cMTPDevice, "dump", device_dump, 0);

  rb_define_method(cMTPDevice, "dump_errors", device_dump_errors, 0);

  rb_define_method(cMTPDevice, "reset_errors", device_reset_errors, 0);

  rb_define_method(cMTPDevice, "reset", device_reset, 0);

  rb_define_method(cMTPDevice, "model_name", device_model_name, 0);

  rb_define_method(cMTPDevice, "serial_number", device_serial_number, 0);

  rb_define_method(cMTPDevice, "version", device_version, 0);

  rb_define_method(cMTPDevice, "friendly_name", device_friendly_name, 0);

  rb_define_method(cMTPDevice, "friendly_name=", device_set_friendly_name, 1);

  rb_define_method(cMTPDevice, "sync_partner", device_sync_partner, 0);

  rb_define_method(cMTPDevice, "sync_partner=", device_set_sync_partner, 1);

  rb_define_method(cMTPDevice, "battery_level", device_battery_level, 0);

  rb_define_method(cMTPDevice, "secure_time", device_secure_time, 0);

  rb_define_method(cMTPDevice, "certificate", device_certificate, 0);

  rb_define_method(cMTPDevice, "supported_filetypes", device_supported_filetypes, 0);

  rb_define_method(cMTPDevice, "storage", device_storage, 1);


  rb_define_method(cMTPDevice, "delete_object", device_delete_object, 1);


  rb_define_method(cMTPDevice, "album_get",       device_album_get,       1);

  rb_define_method(cMTPDevice, "album_list",      device_album_list,      0);

  rb_define_method(cMTPDevice, "album_create",    device_album_create,    2);

  rb_define_method(cMTPDevice, "album_update",    device_album_update,    1);


  rb_define_method(cMTPDevice, "file_info_get", device_file_info_get, 1);

  rb_define_method(cMTPDevice, "file_info_list", device_file_info_list,  0);

  rb_define_method(cMTPDevice, "file_get", device_file_get,  2);

  rb_define_method(cMTPDevice, "file_send", device_file_send,  3);


  rb_define_method(cMTPDevice, "folder_list", device_folder_list, 0);

  rb_define_method(cMTPDevice, "folder_create", device_folder_create, 1);


  rb_define_method(cMTPDevice, "playlist_get", device_playlist_get, 1);

  rb_define_method(cMTPDevice, "playlist_list", device_playlist_list, 0);

  rb_define_method(cMTPDevice, "playlist_create", device_playlist_create, 1);

  rb_define_method(cMTPDevice, "playlist_update", device_playlist_update, 1);


  rb_define_method(cMTPDevice, "track_get", device_track_get, 1);

  rb_define_method(cMTPDevice, "track_list", device_track_list, 0);

  rb_define_method(cMTPDevice, "track_update", device_track_update, 1);

  rb_define_method(cMTPDevice, "track_exists?", device_track_exists, 1);

  rb_define_method(cMTPDevice, "track_get_file", device_track_get_file, 2);

  rb_define_method(cMTPDevice, "track_send_file", device_track_send_file, 3);


  rb_define_module_function(cMTPDevice, "list", device_list, 0);


  return;
}


VALUE device_create()
{
  return rb_funcall(cMTPDevice, rb_intern("new"), 0);
}

