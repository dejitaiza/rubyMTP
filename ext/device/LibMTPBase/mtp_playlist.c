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


static VALUE cMTPPlaylist;


static void playlist_free(void *playlist)
{
  LIBMTP_destroy_playlist_t((LIBMTP_playlist_t *)playlist);

  return;
}


static VALUE playlist_alloc(VALUE klass)
{
  LIBMTP_playlist_t *playlist;

  VALUE obj = Qnil;


  playlist = LIBMTP_new_playlist_t();

  if(playlist != NULL)
  {
    obj = Data_Wrap_Struct(klass, 0, playlist_free, playlist);
  }
  else
  {
    rb_raise(rb_eIOError, "Unable to create playlist");
  }


  return obj;
}


/*
 *  call-seq:
 *     LibMTP::Playlist.new() -> New LibMTP::Playlist object.
 *  
 *  Creates a new LibMTP::Playlist object.
 *
 */

static VALUE playlist_init(VALUE self)
{
  return self;
}


/*
 *  call-seq:
 *     playlist.id -> Playlist ID.
 *  
 *  Returns the playlist ID as an integer.
 *
 */

static VALUE playlist_id(VALUE self)
{
  LIBMTP_playlist_t *playlist;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  return INT2NUM(playlist->playlist_id);
}


/*
 *  call-seq:
 *     playlist.id = value -> playlist.
 *  
 *  Sets the integer playlist ID.
 *
 */

static VALUE playlist_set_id(VALUE self, VALUE value)
{
  LIBMTP_playlist_t *playlist;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  playlist->playlist_id = NUM2UINT(value);


  return self;
}


/*
 *  call-seq:
 *     playlist.name -> Playlist name string.
 *  
 *  Returns the playlist name as a string.
 *
 */

static VALUE playlist_name(VALUE self)
{
  LIBMTP_playlist_t *playlist;

  VALUE value = Qnil;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  if(playlist->name != NULL)
  {
    value = rb_str_new2(playlist->name);
  }


  return value;
}


/*
 *  call-seq:
 *     playlist.name = value -> playlist.
 *  
 *  Sets the playlist name string.
 *
 */

static VALUE playlist_set_name(VALUE self, VALUE value)
{
  LIBMTP_playlist_t *playlist;

  VALUE string;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  if(playlist->name != NULL)
  {
    free(playlist->name);

    playlist->name = NULL;
  }


  if(!NIL_P(value))
  {
    string = StringValue(value);

    if((RSTRING(string)->ptr != NULL) && (RSTRING(string)->len > 0))
    {
      playlist->name = strdup(RSTRING(string)->ptr);
    }
  }


  return self;
}


/*
 *  call-seq:
 *     playlist.length -> Playlist length.
 *  
 *  Returns the number of tracks in the playlist as an integer.
 *
 */

static VALUE playlist_length(VALUE self)
{
  LIBMTP_playlist_t *playlist;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  return INT2NUM(playlist->no_tracks);
}


/*
 *  call-seq:
 *     playlist.tracks -> Array of track IDs.
 *  
 *  Returns an array containing a set of integer track IDs.
 *
 */

static VALUE playlist_tracks(VALUE self)
{
  LIBMTP_playlist_t *playlist;

  VALUE array = rb_ary_new();

  int i;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  for(i=0; i < playlist->no_tracks; i++)
  {
    rb_ary_push(array, INT2NUM(playlist->tracks[i]));
  }


  return array;
}


/*
 *  call-seq:
 *     playlist[index] -> Track ID for index.
 *  
 *  Returns the integer track ID for the specified index.
 *
 */

static VALUE playlist_aref(VALUE self, VALUE index)
{
  LIBMTP_playlist_t *playlist;

  int i;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);


  i = NUM2INT(index);

  if(i < 0)
  {
    i += playlist->no_tracks;

    if(i < 0)
    {
      rb_raise(rb_eRangeError, "index out of range");
    }
  }
  else if(i >= playlist->no_tracks)
  {
    rb_raise(rb_eRangeError, "index out of range");
  }


  return INT2NUM(playlist->tracks[i]);
}


/*
 *  call-seq:
 *     playlist[index] = id -> playlist
 *  
 *  Sets the track ID specified by index to the given ID.
 *
 */

static VALUE playlist_aset(VALUE self, VALUE index, VALUE value)
{
  LIBMTP_playlist_t *playlist;

  int i;


  Data_Get_Struct(self, LIBMTP_playlist_t, playlist);

  if(NUM2INT(index) + 1 > playlist->no_tracks)
  {
    playlist->tracks = (uint32_t *)realloc(playlist->tracks, (NUM2UINT(index) + 1) * sizeof(uint32_t));

    for(i=playlist->no_tracks; i < NUM2INT(index); i++)
    {
      playlist->tracks[i] = 0;
    }

    playlist->no_tracks = NUM2UINT(index) + 1;
  }

  playlist->tracks[NUM2INT(index)] = NUM2UINT(value);


  return self;
}


/*
 *  call-seq:
 *     playlist.<=> -> -1, 0, 1 
 *  
 *  Compares two LibMTP::Playlist objects by their playlist ID.
 *
 */

static VALUE playlist_cmp_by_id(VALUE self, VALUE other)
{
  LIBMTP_playlist_t *playlist_self;

  LIBMTP_playlist_t *playlist_other;

  int status = 0;


  if(!rb_obj_is_instance_of(other, rb_obj_class(self)))
  {
    rb_raise(rb_eTypeError, "wrong argument class");
  }


  Data_Get_Struct(self,  LIBMTP_playlist_t, playlist_self);

  Data_Get_Struct(other, LIBMTP_playlist_t, playlist_other);


  if(playlist_self->playlist_id < playlist_other->playlist_id)
  {
    status = -1;
  }
  else if(playlist_self->playlist_id > playlist_other->playlist_id)
  {
    status = 1;
  }


  return INT2FIX(status);
}


/*
 *  Document-class: LibMTP::Playlist
 *
 *  A LibMTP::Playlist object holds a set of playlist metadata.  This metadata includes a playlist ID,
 *  a playlist name, and a list of integer track IDs.
 *
 *  A list of playlists can be obtained by calling Device#playlist_list on a connected device.
 *
 */

void Init_LibMTP_Playlist(void)
{
  cMTPPlaylist = rb_define_class_under(mLibMTP, "Playlist", rb_cObject);

  rb_define_alloc_func(cMTPPlaylist, playlist_alloc);


  rb_define_method(cMTPPlaylist, "initialize", playlist_init, 0);


  rb_define_method(cMTPPlaylist, "id",     playlist_id, 0);

  rb_define_method(cMTPPlaylist, "id=",    playlist_set_id, 1);


  rb_define_method(cMTPPlaylist, "name",   playlist_name, 0);

  rb_define_method(cMTPPlaylist, "name=",  playlist_set_name, 1);


  rb_define_method(cMTPPlaylist, "length", playlist_length, 0);

  rb_define_alias(cMTPPlaylist,  "size",   "length");


  rb_define_method(cMTPPlaylist, "tracks", playlist_tracks, 0);

  rb_define_alias(cMTPPlaylist,  "to_a",   "tracks");


  rb_define_method(cMTPPlaylist, "[]",     playlist_aref, 1);

  rb_define_method(cMTPPlaylist, "[]=",    playlist_aset, 2);

  rb_define_method(cMTPPlaylist, "<=>",    playlist_cmp_by_id, 1);


  return;
}


VALUE Wrap_LibMTP_Playlist(LIBMTP_playlist_t *playlist)
{
  return Data_Wrap_Struct(cMTPPlaylist, 0, playlist_free, playlist);
}
