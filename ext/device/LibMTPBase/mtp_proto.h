#include "libmtp.h"

#include "ruby.h"


#ifndef __MTP_PROTO_INCLUDE_H__
#define __MTP_PROTO_INCLUDE_H__


RUBY_EXTERN VALUE mLibMTP;  /* in mtp_main.c */


void Init_LibMTP_Entry(void);

void Init_LibMTP_Storage(void);

void Init_LibMTP_Device(void);

void Init_LibMTP_Track(void);

void Init_LibMTP_File(void);

void Init_LibMTP_Folder(void);

void Init_LibMTP_Playlist(void);

void Init_LibMTP_Album(void);


VALUE mtp_storage_create_with_copy(void *);

VALUE device_create();


VALUE Wrap_LibMTP_Album(LIBMTP_album_t *);

VALUE Get_LibMTP_Entry(LIBMTP_device_entry_t *);

VALUE Wrap_LibMTP_Folder(LIBMTP_folder_t *);

VALUE Wrap_LibMTP_Playlist(LIBMTP_playlist_t *);


VALUE Get_LibMTP_File(VALUE);

VALUE Wrap_LibMTP_File(LIBMTP_file_t *);


VALUE Get_LibMTP_Track(VALUE);

VALUE Wrap_LibMTP_Track(LIBMTP_track_t *);


#endif
