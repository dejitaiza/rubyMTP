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


static VALUE cMTPTrack;


static void track_free(void *track)
{
  LIBMTP_destroy_track_t((LIBMTP_track_t *)track);


  return;
}


static VALUE track_alloc(VALUE klass)
{
  LIBMTP_track_t *track;

  VALUE obj = Qnil;


  track = LIBMTP_new_track_t();

  if(track != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, track_free, track);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create track");
  }


  return obj;
}


/*
 *  call-seq:
 *     track.to_hash() -> Hash containing track metadata
 *  
 *  Returns a hash containing the metadata for this track.
 *
 */

static VALUE track_to_hash(VALUE self)
{
  VALUE hash = rb_hash_new();

  LIBMTP_track_t *track;


  Data_Get_Struct(self, LIBMTP_track_t, track);


  rb_hash_aset(hash, rb_str_new2("track_id"),           INT2NUM(track->item_id));

  rb_hash_aset(hash, rb_str_new2("parent_id"),       INT2NUM(track->parent_id));

  if(track->title != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("title"),      rb_str_new2(track->title));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("title"),      Qnil);
  }

  if(track->artist != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("artist"),     rb_str_new2(track->artist));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("artist"),     Qnil);
  }

  if(track->genre != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("genre"),      rb_str_new2(track->genre));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("genre"),      Qnil);
  }

  if(track->album != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("album"),      rb_str_new2(track->album));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("album"),      Qnil);
  }

  if(track->date != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("date"),       rb_str_new2(track->date));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("date"),       Qnil);
  }

  rb_hash_aset(hash, rb_str_new2("number"),       INT2NUM(track->tracknumber));

  rb_hash_aset(hash, rb_str_new2("duration"),     INT2NUM(track->duration));

  rb_hash_aset(hash, rb_str_new2("rate"),         INT2NUM(track->samplerate));

  rb_hash_aset(hash, rb_str_new2("channels"),     INT2NUM(track->nochannels));

  rb_hash_aset(hash, rb_str_new2("codec"),        INT2NUM(track->wavecodec));

  rb_hash_aset(hash, rb_str_new2("bitrate"),      INT2NUM(track->bitrate));

  rb_hash_aset(hash, rb_str_new2("bitrate_type"), INT2NUM(track->bitratetype));

  rb_hash_aset(hash, rb_str_new2("rating"),       INT2NUM(track->rating));

  rb_hash_aset(hash, rb_str_new2("use_count"),    INT2NUM(track->usecount));

  if(track->filename != NULL)
  {
    rb_hash_aset(hash, rb_str_new2("file_name"),  rb_str_new2(track->filename));
  }
  else
  {
    rb_hash_aset(hash, rb_str_new2("file_name"),  Qnil);
  }

  rb_hash_aset(hash, rb_str_new2("file_size"),    INT2NUM(track->filesize));

  rb_hash_aset(hash, rb_str_new2("file_type"),    INT2NUM(track->filetype));


  return hash;
}


/*
 *  call-seq:
 *     track[key] -> Track value for key.
 *  
 *  Returns a value for the specified key.
 *
 */

static VALUE track_aref(VALUE self, VALUE name)
{
  return rb_hash_aref(track_to_hash(self), name);
}


/*
 *  call-seq:
 *     track[key] = value -> value
 *  
 *  Sets the value for the specified key.
 *
 */

static VALUE track_aset(VALUE self, VALUE name, VALUE value)
{
  LIBMTP_track_t *track;

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


  Data_Get_Struct(self, LIBMTP_track_t, track);

  if(strcmp("track_id", ptr) == 0)
  {
    track->item_id = NUM2UINT(value);
  }
  else if(strcmp("parent_id", ptr) == 0)
  {
    track->parent_id = NUM2UINT(value);
  }
  else if(strcmp("title", ptr) == 0)
  {
    if(track->title != NULL)
    {
      free(track->title);

      track->title = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->title = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("artist", ptr) == 0)
  {
    if(track->artist != NULL)
    {
      free(track->artist);

      track->artist = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->artist = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("genre", ptr) == 0)
  {
    if(track->genre != NULL)
    {
      free(track->genre);

      track->genre = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->genre = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("album", ptr) == 0)
  {
    if(track->album != NULL)
    {
      free(track->album);

      track->album = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->album = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("date", ptr) == 0)
  {
    if(track->date != NULL)
    {
      free(track->date);

      track->date = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->date = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("number", ptr) == 0)
  {
    track->tracknumber = NUM2UINT(value);
  }
  else if(strcmp("duration", ptr) == 0)
  {
    track->duration = NUM2UINT(value);
  }
  else if(strcmp("rate", ptr) == 0)
  {
    track->samplerate = NUM2UINT(value);
  }
  else if(strcmp("channels", ptr) == 0)
  {
    track->nochannels = NUM2UINT(value);
  }
  else if(strcmp("codec", ptr) == 0)
  {
    track->wavecodec = NUM2UINT(value);
  }
  else if(strcmp("bitrate", ptr) == 0)
  {
    track->bitrate = NUM2UINT(value);
  }
  else if(strcmp("bitrate_type", ptr) == 0)
  {
    track->bitratetype = NUM2UINT(value);
  }
  else if(strcmp("rating", ptr) == 0)
  {
    track->rating = NUM2UINT(value);
  }
  else if(strcmp("use_count", ptr) == 0)
  {
    track->usecount = NUM2UINT(value);
  }
  else if(strcmp("file_name", ptr) == 0)
  {
    if(track->filename != NULL)
    {
      free(track->filename);

      track->filename = NULL;
    }


    if(!NIL_P(value))
    {
      v_string = StringValue(value);

      if(RSTRING(v_string)->ptr != NULL)
      {
	track->filename = strdup(RSTRING(v_string)->ptr);
      }
    }
  }
  else if(strcmp("file_size", ptr) == 0)
  {
    track->filesize = NUM2UINT(value);
  }
  else if(strcmp("file_type", ptr) == 0)
  {
    track->filetype = NUM2UINT(value);
  }
  else
  {
    rb_raise(rb_eIndexError, "Unable to store data");
  }


  return value;
}


static VALUE track_populate(VALUE array, VALUE self)
{
  VALUE key, value;


  key = rb_ary_shift(array);

  value = rb_ary_shift(array);


  return track_aset(self, key, value);
}


/*
 *  call-seq:
 *     LibMTP::Track.new(hash) -> New LibMTP::Track object.
 *  
 *  Creates a new LibMTP::Track object.  If new is called without any
 *  arguments, the object will be initialized with default values.
 *  Otherwise, a hash can be passed containing the initial values for
 *  the object.
 *
 */

static VALUE track_init(int argc, VALUE *argv, VALUE self)
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

    rb_iterate(rb_each, hash, track_populate, self);
  }


  return self;
}


static VALUE track_init_copy(VALUE self, VALUE orig)
{
  LIBMTP_track_t *track_self;

  LIBMTP_track_t *track_orig;


  if(self == orig) return orig;


  if(!rb_obj_is_instance_of(orig, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  if(DATA_PTR(orig))
  {
    if(DATA_PTR(self))
    {
      track_free(DATA_PTR(self));
    }


    DATA_PTR(self) = LIBMTP_new_track_t();


    Data_Get_Struct(self, LIBMTP_track_t, track_self);

    Data_Get_Struct(orig, LIBMTP_track_t, track_orig);


    if(track_orig->title != NULL)
    {
      track_self->title = strdup(track_orig->title);
    }

    if(track_orig->artist != NULL)
    {
      track_self->artist = strdup(track_orig->artist);
    }

    if(track_orig->album != NULL)
    {
      track_self->album = strdup(track_orig->album);
    }

    if(track_orig->genre != NULL)
    {
      track_self->genre = strdup(track_orig->genre);
    }

    if(track_orig->date != NULL)
    {
      track_self->date = strdup(track_orig->date);
    }

    if(track_orig->filename != NULL)
    {
      track_self->filename = strdup(track_orig->filename);
    }

    track_self->duration    = track_orig->duration;

    track_self->tracknumber = track_orig->tracknumber;

    track_self->filesize    = track_orig->filesize;

    track_self->filetype    = track_orig->filetype;

    track_self->samplerate  = track_orig->samplerate;

    track_self->nochannels  = track_orig->nochannels;

    track_self->wavecodec   = track_orig->wavecodec;

    track_self->bitrate     = track_orig->bitrate;

    track_self->bitratetype = track_orig->bitratetype;

    track_self->rating      = track_orig->rating;

    track_self->usecount    = track_orig->usecount;
  }


  return self;
}


/*
 *  call-seq:
 *     track.<=> -> -1, 0, 1 
 *  
 *  Compares two LibMTP::Track objects by their track ID.
 *
 */

static VALUE track_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_track_t *track_self;

  LIBMTP_track_t *track_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_track_t, track_self);

  Data_Get_Struct(other, LIBMTP_track_t, track_other);


  if(track_self->item_id < track_other->item_id)
  {
    status = -1;
  }
  else if(track_self->item_id > track_other->item_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Track
 *
 *  A LibMTP::Track object holds a set of track metadata.  A LibMTP::Track object can be converted to a
 *  hash by calling the <code>to_hash</code> method.  In addition, data fields can be accessed by calling
 *  the <code>[]</code> method or set by calling the <code>[]=</code> method.
 *
 *  The key names for a LibMTP::Track object are given below.
 *
 *  track_id        =>    Integer track ID
 *
 *  parent_id       =>    Integer parent ID
 *
 *  title           =>    Title string
 *
 *  artist          =>    Artist string
 *
 *  genre           =>    Genre string
 *
 *  album           =>    Album string
 *
 *  date            =>    Date string
 *
 *  number          =>    Integer track number
 *
 *  duration        =>    Duration in seconds
 *
 *  rate            =>    Integer sample rate
 *
 *  channels        =>   Integer number of channels
 *
 *  codec           =>    Integer wave codec
 *
 *  bitrate         =>    Integer bit rate
 *
 *  bitrate_type    =>     Integer bit rate type
 *
 *  rating          =>    Integer 
 *
 *  use_count       =>     Integer use count
 *
 *  file_name       =>    Filename string
 *
 *  file_size       =>    Integer file size
 *
 *  file_type       =>    Integer file type
 *
 */

void Init_LibMTP_Track(void)
{
  cMTPTrack = rb_define_class_under(mLibMTP, "Track", rb_cObject);


  rb_define_alloc_func(cMTPTrack, track_alloc);


  rb_define_method(cMTPTrack, "initialize",      track_init, -1);

  rb_define_method(cMTPTrack, "initialize_copy", track_init_copy, 1);


  rb_define_method(cMTPTrack, "to_hash",        track_to_hash, 0);

  rb_define_method(cMTPTrack, "[]",             track_aref, 1);

  rb_define_method(cMTPTrack, "[]=",            track_aset, 2);

  rb_define_method(cMTPTrack, "<=>",            track_cmp_by_id, 1);


  return;
}


VALUE Get_LibMTP_Track(VALUE value)
{
  VALUE track;

  VALUE hash;


  if(rb_obj_is_instance_of(cMTPTrack, rb_obj_class(value)))
  {
    track = cMTPTrack;
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


    track = rb_funcall(cMTPTrack, rb_intern("new"), 1, hash);
  }


  return track;
}


VALUE Wrap_LibMTP_Track(LIBMTP_track_t *track)
{
  return Data_Wrap_Struct(cMTPTrack, 0, track_free, track);
}
