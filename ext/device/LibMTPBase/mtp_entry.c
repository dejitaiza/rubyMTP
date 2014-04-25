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


static VALUE cMTPEntry;


static void entry_free(void *ptr)
{
  LIBMTP_device_entry_t *entry = (LIBMTP_device_entry_t *)ptr;


  if(entry != NULL)
  {
    if(entry->vendor != NULL)
    {
      free(entry->vendor);
    }

    if(entry->product != NULL)
    {
      free(entry->product);
    }

    free(entry);
  }


  return;
}


static VALUE entry_alloc(VALUE klass)
{
  LIBMTP_device_entry_t *entry;

  VALUE obj = Qnil;


  entry = (LIBMTP_device_entry_t *)calloc(1, sizeof(LIBMTP_device_entry_t));

  if(entry != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, entry_free, entry);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create entry");
  }


  return obj;
}


/*
 *  call-seq:
 *     entry.to_hash() -> Hash containing entry metadata
 *  
 *  Returns a hash containing the metadata for this entry.
 *
 */

static VALUE entry_to_hash(VALUE self)
{
  VALUE hash = rb_hash_new();

  LIBMTP_device_entry_t *entry;


  Data_Get_Struct(self, LIBMTP_device_entry_t, entry);


  rb_hash_aset(hash, rb_str_new2("vendor_id"), INT2NUM(entry->vendor_id));

  rb_hash_aset(hash, rb_str_new2("product_id"), INT2NUM(entry->product_id));

  rb_hash_aset(hash, rb_str_new2("flags"), INT2NUM(entry->device_flags));

  if(entry->vendor != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("vendor"), rb_str_new2(entry->vendor));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("vendor"), Qnil);
  }

  if(entry->product != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("product"), rb_str_new2(entry->product));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("product"), Qnil);
  }


  return hash;
}


/*
 *  call-seq:
 *     entry[key] -> Entry value for key.
 *  
 *  Returns a value for the specified key.
 *
 */

static VALUE entry_aref(VALUE self, VALUE name)
{
  return rb_hash_aref(entry_to_hash(self), name);
}


/*
 *  call-seq:
 *     entry[key] = value -> value
 *  
 *  Sets the value for the specified key.
 *
 */

static VALUE entry_aset(VALUE self, VALUE name, VALUE value)
{
  LIBMTP_device_entry_t *entry;

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


  Data_Get_Struct(self, LIBMTP_device_entry_t, entry);

  if(strcmp("vendor_id", ptr) == 0)
  {
    entry->vendor_id = NUM2UINT(value);
  }
  else if(strcmp("product_id", ptr) == 0)
  {
    entry->product_id = NUM2UINT(value);
  }
  else if(strcmp("flags", ptr) == 0)
  {
    entry->device_flags = NUM2UINT(value);
  }
  else if(strcmp("vendor", ptr) == 0)
  {
    if(entry->vendor != NULL)
    {
      free(entry->vendor);

      entry->vendor = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	entry->vendor = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("product", ptr) == 0)
  {
    if(entry->product != NULL)
    {
      free(entry->product);

      entry->product = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	entry->product = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else
  {
    rb_raise(rb_eIndexError, "Unable to store data");
  }


  return self;
}


static VALUE entry_populate(VALUE array, VALUE self)
{
  VALUE key, value;


  key = rb_ary_shift(array);

  value = rb_ary_shift(array);


  return entry_aset(self, key, value);
}


/*
 *  call-seq:
 *     LibMTP::Entry.new(hash) -> New LibMTP::Entry object.
 *  
 *  Creates a new LibMTP::Entry object.  If new is called without any
 *  arguments, the object will be initialized with default values.
 *  Otherwise, a hash can be passed containing the initial values for
 *  the object.
 *
 */

static VALUE entry_init(int argc, VALUE *argv, VALUE self)
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

    rb_iterate(rb_each, hash, entry_populate, self);
  }


  return self;
}

static VALUE entry_init_copy(VALUE self, VALUE orig)
{

  LIBMTP_device_entry_t *entry_self;

  LIBMTP_device_entry_t *entry_orig;


  if(self == orig) return orig;


  if(!rb_obj_is_instance_of(orig, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  if(DATA_PTR(orig))
  {
    if(DATA_PTR(self))
    {
      entry_free(DATA_PTR(self));
    }


    DATA_PTR(self) = (LIBMTP_device_entry_t *)calloc(1, sizeof(LIBMTP_device_entry_t));


    Data_Get_Struct(self, LIBMTP_device_entry_t, entry_self);

    Data_Get_Struct(self, LIBMTP_device_entry_t, entry_orig);


    entry_self->vendor_id    = entry_orig->vendor_id;

    entry_self->product_id   = entry_orig->product_id;

    entry_self->device_flags = entry_orig->device_flags;

    if(entry_orig->vendor != NULL)
    {
      entry_self->vendor = strdup(entry_orig->vendor);
    }

    if(entry_orig->product != NULL)
    {
      entry_self->product = strdup(entry_orig->product);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     entry.<=> -> -1, 0, 1 
 *  
 *  Compares two LibMTP::Entry objects by their entry ID.
 *
 */

static VALUE entry_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_device_entry_t *entry_self;

  LIBMTP_device_entry_t *entry_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_device_entry_t, entry_self);

  Data_Get_Struct(other, LIBMTP_device_entry_t, entry_other);


  if(entry_self->vendor_id < entry_other->vendor_id)
  {
    status = -1;
  }
  else if(entry_self->vendor_id > entry_other->vendor_id)
  {
    status = 1;
  }
  else if(entry_self->product_id < entry_other->product_id)
  {
    status = -1;
  }
  else if(entry_self->product_id > entry_other->product_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Entry
 *
 *  A LibMTP::Entry object holds a set of entry metadata.  A LibMTP::Entry object can be converted to a
 *  hash by calling the <code>to_hash</code> method.  In addition, data fields can be accessed by calling
 *  the <code>[]</code> method or set by calling the <code>[]=</code> method.
 *
 *  The key names for a LibMTP::Entry object are given below.
 *
 *  vendor_id        =>    Integer vendor ID
 *
 *  product_id       =>    Integer product ID
 *
 *  flags            =>    Integer device flags
 *
 *  vendor           =>    Vendor name string
 *
 *  product          =>    Product name string
 *
 *  A list of device entries can be obtained by calling Device#entry_list on a connected device.
 *
 */

void Init_LibMTP_Entry(void)
{
  cMTPEntry = rb_define_class_under(mLibMTP, "Entry", rb_cObject);

  rb_define_alloc_func(cMTPEntry, entry_alloc);


  rb_define_method(cMTPEntry, "initialize",      entry_init, -1);

  rb_define_method(cMTPEntry, "initialize_copy", entry_init_copy, 1);


  rb_define_method(cMTPEntry, "to_hash",         entry_to_hash, 0);

  rb_define_method(cMTPEntry, "[]",              entry_aref, 1);

  rb_define_method(cMTPEntry, "[]=",             entry_aset, 2);

  rb_define_method(cMTPEntry, "<=>",             entry_cmp_by_id, 1);


  return;
}


VALUE Get_LibMTP_Entry(LIBMTP_device_entry_t *entry)
{
  return Data_Wrap_Struct(cMTPEntry, 0, entry_free, entry);
}
