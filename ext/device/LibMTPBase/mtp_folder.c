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


static VALUE cMTPFolder;


static void folder_free(void *folder)
{
  LIBMTP_destroy_folder_t((LIBMTP_folder_t *)folder);


  return;
}


static VALUE folder_alloc(VALUE klass)
{
  LIBMTP_folder_t *folder;

  VALUE obj = Qnil;


  folder = LIBMTP_new_folder_t();

  if(folder != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, folder_free, folder);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create folder");
  }


  return obj;
}


/*
 *  call-seq:
 *     folder.to_hash() -> Hash containing folder data
 *
 *  Returns a hash containing the data for this folder.
 *
 */

static VALUE folder_to_hash(VALUE self)
{
  VALUE hash = rb_hash_new();

  LIBMTP_folder_t *folder;


  Data_Get_Struct(self, LIBMTP_folder_t, folder);


  rb_hash_aset(hash, rb_str_new2("folder_id"), INT2NUM(folder->folder_id));

  rb_hash_aset(hash, rb_str_new2("parent_id"), INT2NUM(folder->parent_id));

  if(folder->name != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("name"), rb_str_new2(folder->name));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("name"), Qnil);
  }


  return hash;
}


/*
 *  call-seq:
 *     folder[key] -> Folder value for key.
 *
 *  Returns a value for the specified key.
 *
 */

static VALUE folder_aref(VALUE self, VALUE name)
{
  return rb_hash_aref(folder_to_hash(self), name);
}


/*
 *  call-seq:
 *     folder[key] = value -> value
 *
 *  Sets the value for the specified key.
 *
 */

static VALUE folder_aset(VALUE self, VALUE name, VALUE value)
{
  LIBMTP_folder_t *folder;

  VALUE v_string;

  char *ptr;


  if(SYMBOL_P(name))
  {
    ptr = rb_id2name(SYM2ID(name));
  }
  else
  {
    ptr = StringValuePtr(name);
  }


  if(ptr == NULL) return self;


  Data_Get_Struct(self, LIBMTP_folder_t, folder);

  if(strcmp("folder_id", ptr) == 0)
  {
    folder->folder_id = NUM2UINT(value);
  }
  else if(strcmp("parent_id", ptr) == 0)
  {
    folder->parent_id = NUM2UINT(value);
  }
  else if(strcmp("name", ptr) == 0)
  {
    if(folder->name != NULL)
    {
      free(folder->name);

      folder->name = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->as.heap.ptr != NULL)
      {
	folder->name = strdup(RSTRING(v_string)->as.heap.ptr);
      }
    }
  }
  else
  {
    rb_raise(rb_eIndexError, "Unable to store data");
  }


  return self;
}


static VALUE folder_populate(VALUE array, VALUE self)
{
  VALUE key, value;


  key = rb_ary_shift(array);

  value = rb_ary_shift(array);


  return folder_aset(self, key, value);
}


/*
 *  call-seq:
 *     LibMTP::Folder.new(hash) -> New LibMTP::Folder object.
 *
 *  Creates a new LibMTP::Folder object.  If new is called without any
 *  arguments, the object will be initialized with default values.
 *  Otherwise, a hash can be passed containing the initial values for
 *  the object.
 *
 */

static VALUE folder_init(int argc, VALUE *argv, VALUE self)
{
  VALUE hash;


  if(argc == 1)
  {
    if(TYPE(argv[0]) == T_HASH)
    {
      hash = argv[0];
    }
    else
    {
      if(rb_respond_to(argv[0], rb_intern("to_hash")))
      {
	hash = rb_funcall(argv[0], rb_intern("to_hash"), 0);
      }
      else
      {
	rb_raise(rb_eTypeError, "wrong argument class");
      }
    }

    rb_iterate(rb_each, hash, folder_populate, self);
  }


  return self;
}


/*
 *  call-seq:
 *     folder.<=> -> -1, 0, 1
 *
 *  Compares two LibMTP::Folder objects by their folder ID.
 *
 */

static VALUE folder_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_folder_t *folder_self;

  LIBMTP_folder_t *folder_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_folder_t, folder_self);

  Data_Get_Struct(other, LIBMTP_folder_t, folder_other);


  if(folder_self->folder_id < folder_other->folder_id)
  {
    status = -1;
  }
  else if(folder_self->folder_id > folder_other->folder_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Folder
 *
 *  A LibMTP::Folder object holds a set of folder metadata.  A LibMTP::Folder object can be converted to a
 *  hash by calling the <code>to_hash</code> method.  In addition, data fields can be accessed by calling
 *  the <code>[]</code> method or set by calling the <code>[]=</code> method.
 *
 *  The key names for a LibMTP::Folder object are given below.
 *
 *  folder_id       =>    Integer folder ID
 *
 *  parent_id       =>    Integer parent ID
 *
 *  name            =>    Folder name string
 *
 */

void Init_LibMTP_Folder(void)
{
  cMTPFolder = rb_define_class_under(mLibMTP, "Folder", rb_cObject);

  rb_define_alloc_func(cMTPFolder, folder_alloc);


  rb_define_method(cMTPFolder, "initialize", folder_init, -1);


  rb_define_method(cMTPFolder, "to_hash",         folder_to_hash, 0);

  rb_define_method(cMTPFolder, "[]",              folder_aref, 1);

  rb_define_method(cMTPFolder, "[]=",             folder_aset, 2);

  rb_define_method(cMTPFolder, "<=>",             folder_cmp_by_id, 1);


  return;
}


VALUE Wrap_LibMTP_Folder(LIBMTP_folder_t *folder)
{
  return Data_Wrap_Struct(cMTPFolder, 0, folder_free, folder);
}
