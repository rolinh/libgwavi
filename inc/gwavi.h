/*
 * Copyright (c) 2008-2011, Michael Kohn
 * Copyright (c) 2013, Robin Hahling
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the author nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef H_GWAVI
#define H_GWAVI

#include <stdio.h>

/* structures */
struct gwavi_header_t
{
	int time_delay;
	int data_rate;
	int reserved;
	int flags;
	int number_of_frames;
	int initial_frames;
	int data_streams;
	int buffer_size;
	int width;
	int height;
	int time_scale;
	int playback_data_rate;
	int starting_time;
	int data_length;
};

struct gwavi_stream_header_t
{
	char data_type[5];
	char codec[5];
	int flags;
	int priority;
	int initial_frames;
	int time_scale;
	int data_rate;
	int start_time;
	int data_length;
	int buffer_size;
	int quality;
	int sample_size;
};

struct gwavi_stream_format_v_t
{
	int header_size;
	int width;
	int height;
	short int num_planes;
	short int bits_per_pixel;
	int compression_type;
	int image_size;
	int x_pels_per_meter;
	int y_pels_per_meter;
	int colors_used;
	int colors_important;
	int *palette;
	int palette_count;
};

struct gwavi_stream_format_a_t
{
	short format_type;
	int channels;
	int sample_rate;
	int bytes_per_second;
	int block_align;
	int bits_per_sample;
	short size;
};

struct gwavi_index_t
{
	int id;
	int flags;
	int offset;
	int length;
};

struct gwavi_t
{
	FILE *out;
	struct gwavi_header_t avi_header;
	struct gwavi_stream_header_t stream_header_v;
	struct gwavi_stream_format_v_t stream_format_v;
	struct gwavi_stream_header_t stream_header_a;
	struct gwavi_stream_format_a_t stream_format_a;
	long marker;
	int offsets_ptr;
	int offsets_len;
	long offsets_start;
	int *offsets;
	int offset_count;
};

struct gwavi_audio_t
{
	int channels;
	int bits;
	int samples_per_second;
};


/* Main ibrary functions */
struct gwavi_t *gwavi_open(char *filename, int width, int height, char *fourcc,
			   int fps, struct gwavi_audio_t *audio);
int gwavi_add_frame(struct gwavi_t *gwavi, unsigned char *buffer, size_t len);
int gwavi_add_audio(struct gwavi_t *gwavi, unsigned char *buffer, size_t len);
int gwavi_close(struct gwavi_t *gwavi);

/*
 * If needed, these functions can be called before closing the file to
 * change the framerate, codec, size.
 * Note: AVI can only have a single frame rate, codec, size for the whole file
 * so this affects anything recorded before these functions are called.
 */
void gwavi_set_framerate(struct gwavi_t *gwavi, int fps);
void gwavi_set_codec(struct gwavi_t *gwavi, const char *fourcc);
void gwavi_set_size(struct gwavi_t *gwavi, int width, int height);

#endif /* ndef H_GWAVI */

