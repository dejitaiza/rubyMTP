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


static VALUE cMTPAlbum;


static void album_free(void *album)
{
  LIBMTP_destroy_album_t((LIBMTP_album_t *)album);

  return;
}


static VALUE album_alloc(VALUE klass)
{
  LIBMTP_album_t *album;

  VALUE obj = Qnil;


  album = LIBMTP_new_album_t();

  if(album != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, album_free, album);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create album");
  }


  return obj;
}


/*
 *  call-seq:
 *     LibMTP::Album.new() -> New LibMTP::Album object.
 *
 *  Creates a new LibMTP::Album object.
 *
 */

static VALUE album_init(VALUE self)
{
  return self;
}


/*
 *  call-seq:
 *     album.id -> Album ID.
 *
 *  Returns the album ID as an integer.
 *
 */

static VALUE album_id(VALUE self)
{
  LIBMTP_album_t *album;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  return INT2NUM(album->album_id);
}


/*
 *  call-seq:
 *     album.id = value -> album.
 *
 *  Sets the integer album ID.
 *
 */

static VALUE album_set_id(VALUE self, VALUE value)
{
  LIBMTP_album_t *album;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  album->album_id = NUM2UINT(value);


  return self;
}


/*
 *  call-seq:
 *     album.name -> Album name string.
 *
 *  Returns the album name as a string.
 *
 */

static VALUE album_name(VALUE self)
{
  LIBMTP_album_t *album;

  VALUE value = Qnil;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->name != NULL)
  {
    value = rb_str_new2(album->name);
  }


  return value;
}


/*
 *  call-seq:
 *     album.name = value -> album.
 *
 *  Sets the album name string.
 *
 */

static VALUE album_set_name(VALUE self, VALUE value)
{
  LIBMTP_album_t *album;

  VALUE string;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->name != NULL)
  {
    free(album->name);

    album->name = NULL;
  }


  if(!NIL_P(value))
  {
    string = StringValue(value);

    if((RSTRING(string)->as.heap.ptr != NULL) && (RSTRING(string)->as.heap.len > 0))
    {
      album->name = strdup(RSTRING(string)->as.heap.ptr);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     album.artist -> Album artist name string.
 *
 *  Returns the album artist name as a string.
 *
 */

static VALUE album_artist(VALUE self)
{
  LIBMTP_album_t *album;

  VALUE value = Qnil;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->artist != NULL)
  {
    value = rb_str_new2(album->artist);
  }


  return value;
}


/*
 *  call-seq:
 *     album.artist = value -> album.
 *
 *  Sets the album artist name string.
 *
 */

static VALUE album_set_artist(VALUE self, VALUE value)
{
  LIBMTP_album_t *album;

  VALUE string;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->artist != NULL)
  {
    free(album->artist);

    album->artist = NULL;
  }


  if(!NIL_P(value))
  {
    string = StringValue(value);

    if((RSTRING(string)->as.heap.ptr != NULL) && (RSTRING(string)->as.heap.len > 0))
    {
      album->artist = strdup(RSTRING(string)->as.heap.ptr);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     album.genre -> Album genre string.
 *
 *  Returns the album genre as a string.
 *
 */

static VALUE album_genre(VALUE self)
{
  LIBMTP_album_t *album;

  VALUE value = Qnil;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->genre != NULL)
  {
    value = rb_str_new2(album->genre);
  }


  return value;
}


/*
 *  call-seq:
 *     album.genre = value -> album.
 *
 *  Sets the album genre string.
 *
 */

static VALUE album_set_genre(VALUE self, VALUE value)
{
  LIBMTP_album_t *album;

  VALUE string;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(album->genre != NULL)
  {
    free(album->genre);

    album->genre = NULL;
  }


  if(!NIL_P(value))
  {
    string = StringValue(value);

    if((RSTRING(string)->as.heap.ptr != NULL) && (RSTRING(string)->as.heap.len > 0))
    {
      album->genre = strdup(RSTRING(string)->as.heap.ptr);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     album.length -> Album length.
 *
 *  Returns the number of tracks in the album as an integer.
 *
 */

static VALUE album_length(VALUE self)
{
  LIBMTP_album_t *album;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  return INT2NUM(album->no_tracks);
}


/*
 *  call-seq:
 *     album.tracks -> Array of track IDs.
 *
 *  Returns an array containing a set of integer track IDs.
 *
 */

static VALUE album_tracks(VALUE self)
{
  LIBMTP_album_t *album;

  VALUE array = rb_ary_new();

  int i;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  for(i=0; i < album->no_tracks; i++)
  {
    rb_ary_push(array, INT2NUM(album->tracks[i]));
  }


  return array;
}


/*
 *  call-seq:
 *     album[index] -> Track ID for index.
 *
 *  Returns the integer track ID for the specified index.
 *
 */

static VALUE album_aref(VALUE self, VALUE index)
{
  LIBMTP_album_t *album;

  int i;


  Data_Get_Struct(self, LIBMTP_album_t, album);


  i = NUM2INT(index);

  if(i < 0)
  {
    i += album->no_tracks;

    if(i < 0)
    {
      rb_raise(rb_eRangeError, "index out of range");
    }
  }
  else if(i >= album->no_tracks)
  {
    rb_raise(rb_eRangeError, "index out of range");
  }


  return INT2NUM(album->tracks[i]);
}


/*
 *  call-seq:
 *     album[index] = id -> album
 *
 *  Sets the track ID specified by index to the given ID.
 *
 */

static VALUE album_aset(VALUE self, VALUE index, VALUE value)
{
  LIBMTP_album_t *album;

  int i;


  Data_Get_Struct(self, LIBMTP_album_t, album);

  if(NUM2INT(index) + 1 > album->no_tracks)
  {
    album->tracks = (uint32_t *)realloc(album->tracks, (NUM2UINT(index) + 1) * sizeof(uint32_t));

    for(i=album->no_tracks; i < NUM2INT(index); i++)
    {
      album->tracks[i] = 0;
    }

    album->no_tracks = NUM2UINT(index) + 1;
  }

  album->tracks[NUM2INT(index)] = NUM2UINT(value);


  return self;
}


/*
 *  call-seq:
 *     album.<=> -> -1, 0, 1
 *
 *  Compares two LibMTP::Album objects by their album ID.
 *
 */

static VALUE album_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_album_t *album_self;

  LIBMTP_album_t *album_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_album_t, album_self);

  Data_Get_Struct(other, LIBMTP_album_t, album_other);


  if(album_self->album_id < album_other->album_id)
  {
    status = -1;
  }
  else if(album_self->album_id > album_other->album_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Album
 *
 *  A LibMTP::Album object holds a set of album metadata.  This metadata includes an album ID,
 *  an album name, and a list of integer track IDs.
 *
 *  A list of albums can be obtained by calling Device#album_list on a connected device.
 *
 */

void Init_LibMTP_Album(void)
{
  cMTPAlbum = rb_define_class_under(mLibMTP, "Album", rb_cObject);

  rb_define_alloc_func(cMTPAlbum, album_alloc);


  rb_define_method(cMTPAlbum, "initialize", album_init, 0);


  rb_define_method(cMTPAlbum, "id",     album_id, 0);

  rb_define_method(cMTPAlbum, "id=",    album_set_id, 1);


  rb_define_method(cMTPAlbum, "name",   album_name, 0);

  rb_define_method(cMTPAlbum, "name=",  album_set_name, 1);


  rb_define_method(cMTPAlbum, "artist",   album_artist, 0);

  rb_define_method(cMTPAlbum, "artist=",  album_set_artist, 1);


  rb_define_method(cMTPAlbum, "genre",   album_genre, 0);

  rb_define_method(cMTPAlbum, "genre=",  album_set_genre, 1);


  rb_define_method(cMTPAlbum, "length", album_length, 0);

  rb_define_alias(cMTPAlbum,  "size",   "length");


  rb_define_method(cMTPAlbum, "tracks", album_tracks, 0);

  rb_define_alias(cMTPAlbum,  "to_a",   "tracks");


  rb_define_method(cMTPAlbum, "[]",     album_aref, 1);

  rb_define_method(cMTPAlbum, "[]=",    album_aset, 2);

  rb_define_method(cMTPAlbum, "<=>",    album_cmp_by_id, 1);


  return;
}


VALUE Wrap_LibMTP_Album(LIBMTP_album_t *album)
{
  return Data_Wrap_Struct(cMTPAlbum, 0, album_free, album);
}
