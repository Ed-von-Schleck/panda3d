// Filename: downloader.h
// Created by:  mike (09Jan97)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////
#ifndef DOWNLOADER_H
#define DOWNLOADER_H
//
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////
#include <pandabase.h>
#include <notify.h>
#include <filename.h>
#include <buffer.h>
#include <pointerTo.h>
#include <clockObject.h>

#if defined(WIN32_VC)
  #include <winsock.h>
#else
  #include <netinet/in.h>  // Irix seems to require this one for resolv.h.
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <resolv.h>
#endif

class URLSpec;

////////////////////////////////////////////////////////////////////
//       Class : Downloader
// Description : This object is used to manage downloading of data
//               from an HTTP server as a background task within a
//               single-threaded network application.  The class can
//               download a small piece at a time when run() is called
//               from time to time; it is designed to both limit time
//               spent in the run() call as well as limiting network
//               bandwidth utilized by the download, so that CPU
//               cycles and bandwidth are still available to the
//               application for other purposes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEXPRESS Downloader {
PUBLISHED:
  Downloader(void);
  virtual ~Downloader(void);

  int connect_to_server_by_proxy(const URLSpec &proxy, const string &server_name);
  int connect_to_server(const string &name, uint port=80);
  void disconnect_from_server(void);

  int initiate(const string &file_name, Filename file_dest);
  int initiate(const string &file_name, Filename file_dest,
                int first_byte, int last_byte, int total_bytes,
                bool partial_content = true);
  int initiate(const string &file_name);
  INLINE int get_file_size() const;

  int run(void);

  bool get_ramfile(Ramfile &rfile);

  INLINE void set_frequency(float frequency);
  INLINE float get_frequency(void) const;
  INLINE void set_byte_rate(float bytes);
  INLINE float get_byte_rate(void) const;
  INLINE void set_disk_write_frequency(int frequency);
  INLINE int get_disk_write_frequency(void) const;
  INLINE int get_bytes_written(void) const;
  INLINE int get_bytes_requested(void) const;
  INLINE float get_bytes_per_second(void) const;

  void cleanup(void);


private:
  class DownloadStatus {
  public:
    DownloadStatus(char *buffer, int first_byte, int last_byte,
                        int total_bytes, bool partial_content);
    void reset(void);

  public:
    bool _first_line_complete;
    bool _header_is_complete;
    bool _header_is_valid;
    char *_start;
    char *_next_in;
    int _bytes_in_buffer;
    int _total_bytes_written;
    int _total_bytes_requested;
    int _first_byte;
    int _last_byte;
    int _total_bytes;
    bool _partial_content;
    char *_buffer;
  };

  INLINE void recompute_buffer(void);

  int connect_to_server(void);
  int safe_send(int socket, const char *data, int length, long timeout);
  int fast_receive(int socket, DownloadStatus *status, int rec_size);
  int parse_http_response(const string &resp);
  int parse_header(DownloadStatus *status);
  int write_to_disk(DownloadStatus *status);
  int run_to_ram(void);
  int write_to_ram(DownloadStatus *status);

private:
  bool _connected;
  int _socket;
  string _server_name;
  bool _use_proxy;
  string _proxy_string;
  struct sockaddr_in _sin;
  bool _TCP_stack_initialized;

  bool _initiated;
  bool _ever_initiated;
  PT(Buffer) _buffer;
  int _disk_write_frequency;
  float _frequency;
  float _byte_rate;
  ulong _receive_size;
  int _disk_buffer_size;
  ofstream _dest_stream;
  ostringstream *_dest_string_stream;
  bool _recompute_buffer;

  DownloadStatus *_current_status;
  bool _got_any_data;
  int _total_bytes_written;
  int _total_bytes_requested;
  bool _download_to_ram;

  int _file_size;

  double _tlast;
  double _tfirst;
  ClockObject _clock;
};

#include "downloader.I"

#endif
