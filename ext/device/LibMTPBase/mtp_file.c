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


static VALUE cMTPFile;


static void file_free(void *file)
{
  LIBMTP_destroy_file_t((LIBMTP_file_t *)file);


  return;
}


static VALUE file_alloc(VALUE klass)
{
  LIBMTP_file_t *file;

  VALUE obj = Qnil;


  file = LIBMTP_new_file_t();

  if(file != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, file_free, file);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create file");
  }


  return obj;
}


/*
 *  call-seq:
 *     file.to_hash() -> Hash containing file metadata
 *
 *  Returns a hash containing the metadata for this file.
 *
 */

static VALUE file_to_hash(VALUE self)
{
  VALUE hash = rb_hash_new();

  LIBMTP_file_t *file;


  Data_Get_Struct(self, LIBMTP_file_t, file);

  rb_hash_aset(hash, rb_str_new2("file_id"),      INT2NUM(file->item_id));

  rb_hash_aset(hash, rb_str_new2("parent_id"),    INT2NUM(file->parent_id));

  if(file->filename != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("file_name"),  rb_str_new2(file->filename));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("file_name"),  Qnil);
  }

  rb_hash_aset(hash, rb_str_new2("file_size"),    INT2NUM(file->filesize));

  rb_hash_aset(hash, rb_str_new2("file_type"),    INT2NUM(file->filetype));


  return hash;
}


/*
 *  call-seq:
 *     file[key] -> File value for key.
 *
 *  Returns a value for the specified key.
 *
 */

static VALUE file_aref(VALUE self, VALUE name)
{
  return rb_hash_aref(file_to_hash(self), name);
}


/*
 *  call-seq:
 *     file[key] = value -> value
 *
 *  Sets the value for the specified key.
 *
 */

static VALUE file_aset(VALUE self, VALUE name, VALUE value)
{
  LIBMTP_file_t *file;

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


  Data_Get_Struct(self, LIBMTP_file_t, file);

  if(strcmp("file_id", ptr) == 0)
  {
    file->item_id = NUM2UINT(value);
  }
  else if(strcmp("parent_id", ptr) == 0)
  {
    file->parent_id = NUM2UINT(value);
  }
  else if(strcmp("file_name", ptr) == 0)
  {
    if(file->filename != NULL)
    {
      free(file->filename);

      file->filename = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->as.heap.ptr != NULL)
      {
	file->filename = strdup(RSTRING(v_string)->as.heap.ptr);
      }
    }
  }
  else if(strcmp("file_size", ptr) == 0)
  {
    file->filesize = NUM2UINT(value);
  }
  else if(strcmp("file_type", ptr) == 0)
  {
    file->filetype = NUM2UINT(value);
  }
  else
  {
    rb_raise(rb_eIndexError, "Unable to store data");
  }


  return value;
}


static VALUE file_populate(VALUE array, VALUE self)
{
  VALUE key, value;


  key = rb_ary_shift(array);

  value = rb_ary_shift(array);


  return file_aset(self, key, value);
}


/*
 *  call-seq:
 *     LibMTP::File.new(hash) -> New LibMTP::File object.
 *
 *  Creates a new LibMTP::File object.  If new is called without any
 *  arguments, the object will be initialized with default values.
 *  Otherwise, a hash can be passed containing the initial values for
 *  the object.
 *
 */

static VALUE file_init(int argc, VALUE *argv, VALUE self)
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

    rb_iterate(rb_each, hash, file_populate, self);
  }


  return self;
}


/*
 *  call-seq:
 *     file.<=> -> -1, 0, 1
 *
 *  Compares two LibMTP::File objects by their file ID.
 *
 */

static VALUE file_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_file_t *file_self;

  LIBMTP_file_t *file_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_file_t, file_self);

  Data_Get_Struct(other, LIBMTP_file_t, file_other);


  if(file_self->item_id < file_other->item_id)
  {
    status = -1;
  }
  else if(file_self->item_id > file_other->item_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::File
 *
 *  A LibMTP::File object holds a set of file metadata.  A LibMTP::File object can be converted to a
 *  hash by calling the <code>to_hash</code> method.  In addition, data fields can be accessed by calling
 *  the <code>[]</code> method or set by calling the <code>[]=</code> method.
 *
 *  The key names for a LibMTP::File object are given below.
 *
 *  file_id         =>    Integer file ID
 *
 *  parent_id       =>    Integer parent ID
 *
 *  file_name       =>    Filename string
 *
 *  file_size       =>    Integer file size
 *
 *  file_type       =>    Integer file type
 *
 */

void Init_LibMTP_File(void)
{
  cMTPFile = rb_define_class_under(mLibMTP, "File", rb_cObject);

  rb_define_alloc_func(cMTPFile, file_alloc);


  rb_define_method(cMTPFile, "initialize", file_init, -1);


  rb_define_method(cMTPFile, "to_hash", file_to_hash, 0);

  rb_define_method(cMTPFile, "[]",      file_aref, 1);

  rb_define_method(cMTPFile, "[]=",     file_aset, 2);

  rb_define_method(cMTPFile, "<=>",     file_cmp_by_id, 1);


  return;
}


VALUE Get_LibMTP_File(VALUE value)
{
  VALUE file;

  VALUE hash;


  if(rb_obj_is_instance_of(cMTPFile, rb_obj_class(value)))
  {
    file = cMTPFile;
  }
  else
  {
    if(TYPE(value) == T_HASH)
    {
      hash = value;
    }
    else if(rb_respond_to(value, rb_intern("to_hash")))
    {
      hash = rb_funcall(value, rb_intern("to_hash"), 0);
    }
    else
    {
      rb_raise(rb_eTypeError, "wrong argument class");
    }


    file = rb_funcall(cMTPFile, rb_intern("new"), 1, hash);
  }


  return file;
}


VALUE Wrap_LibMTP_File(LIBMTP_file_t *file)
{
  return Data_Wrap_Struct(cMTPFile, 0, file_free, file);
}
