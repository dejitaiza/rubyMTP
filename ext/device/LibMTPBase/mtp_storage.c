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


static VALUE cMTPStorage;


static void storage_free(void *ptr)
{
  LIBMTP_devicestorage_t *storage = (LIBMTP_devicestorage_t *)ptr;


  if(storage != NULL)
  {
    if(storage->StorageDescription != NULL)
    {
      free(storage->StorageDescription);
    }

    if(storage->VolumeIdentifier != NULL)
    {
      free(storage->VolumeIdentifier);
    }

    free(storage);
  }


  return;
}


static VALUE storage_alloc(VALUE klass)
{
  LIBMTP_devicestorage_t *storage;

  VALUE obj = Qnil;


  storage = (LIBMTP_devicestorage_t *)calloc(1, sizeof(LIBMTP_devicestorage_t));

  if(storage != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, storage_free, storage);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create storage");
  }


  return obj;
}


static VALUE storage_init_copy(VALUE self, VALUE orig)
{

  LIBMTP_devicestorage_t *storage_self;

  LIBMTP_devicestorage_t *storage_orig;


  if(self == orig) return orig;


  if(!rb_obj_is_instance_of(orig, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  if(DATA_PTR(orig))
  {
    if(DATA_PTR(self))
    {
      storage_free(DATA_PTR(self));
    }


    DATA_PTR(self) = (LIBMTP_devicestorage_t *)calloc(1, sizeof(LIBMTP_devicestorage_t));


    Data_Get_Struct(self, LIBMTP_devicestorage_t, storage_self);

    Data_Get_Struct(self, LIBMTP_devicestorage_t, storage_orig);


    storage_self->id                 = storage_orig->id;

    storage_self->StorageType        = storage_orig->StorageType;

    storage_self->FilesystemType     = storage_orig->FilesystemType;

    storage_self->AccessCapability   = storage_orig->AccessCapability;

    storage_self->MaxCapacity        = storage_orig->MaxCapacity;

    storage_self->FreeSpaceInBytes   = storage_orig->FreeSpaceInBytes;

    storage_self->FreeSpaceInObjects = storage_orig->FreeSpaceInObjects;

    if(storage_orig->StorageDescription != NULL)
    {
      storage_self->StorageDescription = strdup(storage_orig->StorageDescription);
    }

    if(storage_orig->VolumeIdentifier != NULL)
    {
      storage_self->VolumeIdentifier = strdup(storage_orig->VolumeIdentifier);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     storage.to_hash() -> Hash containing storage metadata
 *  
 *  Returns a hash containing the metadata for this storage object.
 *
 */

static VALUE storage_to_hash(VALUE self)
{
  VALUE hash = rb_hash_new();

  LIBMTP_devicestorage_t *storage;


  Data_Get_Struct(self, LIBMTP_devicestorage_t, storage);

  /* Added fix for max_capacity, free_space_in_bytes, and free_space_in_objects to work with sizes > 4GB */
  /* Thanks to Greg White <gwhite@bustedflush.org> for providing this fix. */

  rb_hash_aset(hash, rb_str_new2("storage_id"), INT2NUM(storage->id));

  rb_hash_aset(hash, rb_str_new2("storage_type"), INT2NUM(storage->StorageType));

  rb_hash_aset(hash, rb_str_new2("filesystem_type"), INT2NUM(storage->FilesystemType));

  rb_hash_aset(hash, rb_str_new2("access_capability"), INT2NUM(storage->AccessCapability));

  rb_hash_aset(hash, rb_str_new2("max_capacity"), rb_ll2inum(storage->MaxCapacity));

  rb_hash_aset(hash, rb_str_new2("free_space_in_bytes"), rb_ll2inum(storage->FreeSpaceInBytes));

  rb_hash_aset(hash, rb_str_new2("free_space_in_objects"), rb_ll2inum(storage->FreeSpaceInObjects));

  if(storage->StorageDescription != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("description"), rb_str_new2(storage->StorageDescription));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("description"), Qnil);
  }

  if(storage->VolumeIdentifier != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("volume_id"), rb_str_new2(storage->VolumeIdentifier));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("volume_id"), Qnil);
  }


  return hash;
}


/*
 *  call-seq:
 *     storage[key] -> Storage value for key.
 *  
 *  Returns a value for the specified key.
 *
 */

static VALUE storage_aref(VALUE self, VALUE name)
{
  return rb_hash_aref(storage_to_hash(self), name);
}


/*
 *  call-seq:
 *     storage[key] = value -> value
 *  
 *  Sets the value for the specified key.
 *
 */

static VALUE storage_aset(VALUE self, VALUE name, VALUE value)
{
  LIBMTP_devicestorage_t *storage;

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


  /* Added fix for max_capacity, free_space_in_bytes, and free_space_in_objects to work with sizes > 4GB */
  /* Thanks to Greg White <gwhite@bustedflush.org> for providing this fix. */


  Data_Get_Struct(self, LIBMTP_devicestorage_t, storage);

  if(strcmp("storage_id", ptr) == 0)
  {
    storage->id = NUM2UINT(value);
  }
  else if(strcmp("storage_type", ptr) == 0)
  {
    storage->StorageType = NUM2UINT(value);
  }
  else if(strcmp("filesystem_type", ptr) == 0)
  {
    storage->FilesystemType = NUM2UINT(value);
  }
  else if(strcmp("access_capability", ptr) == 0)
  {
    storage->AccessCapability = NUM2UINT(value);
  }
  else if(strcmp("max_capacity", ptr) == 0)
  {
    storage->MaxCapacity = rb_num2ll(value);
  }
  else if(strcmp("free_space_in_bytes", ptr) == 0)
  {
    storage->FreeSpaceInBytes = rb_num2ll(value);
  }
  else if(strcmp("free_space_in_objects", ptr) == 0)
  {
    storage->FreeSpaceInObjects = rb_num2ll(value);
  }
  else if(strcmp("description", ptr) == 0)
  {
    if(storage->StorageDescription != NULL)
    {
      free(storage->StorageDescription);

      storage->StorageDescription = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	storage->StorageDescription = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("volume_id", ptr) == 0)
  {
    if(storage->VolumeIdentifier != NULL)
    {
      free(storage->VolumeIdentifier);

      storage->VolumeIdentifier = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	storage->VolumeIdentifier = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else
  {
    rb_raise(rb_eIndexError, "Unable to store data");
  }


  return self;
}


static VALUE storage_populate(VALUE array, VALUE self)
{
  VALUE key, value;


  key = rb_ary_shift(array);

  value = rb_ary_shift(array);


  return storage_aset(self, key, value);
}


/*
 *  call-seq:
 *     LibMTP::Storage.new(hash) -> New LibMTP::Storage object.
 *  
 *  Creates a new LibMTP::Storage object.  If new is called without any
 *  arguments, the object will be initialized with default values.
 *  Otherwise, a hash can be passed containing the initial values for
 *  the object.
 *
 */

static VALUE storage_init(int argc, VALUE *argv, VALUE self)
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

    rb_iterate(rb_each, hash, storage_populate, self);
  }


  return self;
}


/*
 *  call-seq:
 *     storage.<=> -> -1, 0, 1 
 *  
 *  Compares two LibMTP::Storage objects by their storage ID.
 *
 */

static VALUE storage_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_devicestorage_t *storage_self;

  LIBMTP_devicestorage_t *storage_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_devicestorage_t, storage_self);

  Data_Get_Struct(other, LIBMTP_devicestorage_t, storage_other);


  if(storage_self->id < storage_other->id)
  {
    status = -1;
  }
  else if(storage_self->id > storage_other->id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Storage
 *
 *  A LibMTP::Storage object holds a set of storage metadata.  A LibMTP::Storage object can be converted to a
 *  hash by calling the <code>to_hash</code> method.  In addition, data fields can be accessed by calling
 *  the <code>[]</code> method or set by calling the <code>[]=</code> method.
 *
 *  The key names for a LibMTP::Storage object are given below.
 *
 *  storage_id            => Integer storage ID
 *
 *  storage_type          => Integer storage type
 *
 *  filesystem_type       => Integer file system type.
 *
 *  access_capability     => Integer access capability
 *
 *  max_capacity          => Integer maximum capacity.
 *
 *  free_space_in_bytes   => Integer free space as number of bytes.
 *
 *  free_space_in_objects => Integer free space as number of objects.
 *
 *  description           => Description string.
 *
 *  volume_id             => Volume ID string.
 *
 */

void Init_LibMTP_Storage(void)
{
  cMTPStorage = rb_define_class_under(mLibMTP, "Storage", rb_cObject);

  rb_define_alloc_func(cMTPStorage, storage_alloc);


  rb_define_method(cMTPStorage, "initialize",      storage_init, -1);

  rb_define_method(cMTPStorage, "initialize_copy", storage_init_copy, 1);


  rb_define_method(cMTPStorage, "to_hash",         storage_to_hash, 0);

  rb_define_method(cMTPStorage, "[]",              storage_aref, 1);

  rb_define_method(cMTPStorage, "[]=",             storage_aset, 2);

  rb_define_method(cMTPStorage, "<=>",             storage_cmp_by_id, 1);



  return;
}


VALUE mtp_storage_create_with_copy(void *storage_ptr)
{
  LIBMTP_devicestorage_t *storage_orig = (LIBMTP_devicestorage_t *)storage_ptr;

  LIBMTP_devicestorage_t *storage_self = NULL;

  VALUE storage = Qnil;


  if(storage_orig == NULL)
  {
    rb_raise(rb_eIOError, "Unable to create storage");
  }
  else
  {
    storage_self = (LIBMTP_devicestorage_t *)calloc(1, sizeof(LIBMTP_devicestorage_t));

    storage_self->id                 = storage_orig->id;

    storage_self->StorageType        = storage_orig->StorageType;

    storage_self->FilesystemType     = storage_orig->FilesystemType;

    storage_self->AccessCapability   = storage_orig->AccessCapability;

    storage_self->MaxCapacity        = storage_orig->MaxCapacity;

    storage_self->FreeSpaceInBytes   = storage_orig->FreeSpaceInBytes;

    storage_self->FreeSpaceInObjects = storage_orig->FreeSpaceInObjects;


    if(storage_orig->StorageDescription != NULL)
    {
      storage_self->StorageDescription = strdup(storage_orig->StorageDescription);
    }


    if(storage_orig->VolumeIdentifier != NULL)
    {
      storage_self->VolumeIdentifier = strdup(storage_orig->VolumeIdentifier);
    }


    storage = Data_Wrap_Struct(cMTPStorage, 0, storage_free, storage_self);
  }


  return storage;
}
