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
	unsigned int time_delay;
	unsigned int data_rate;
	unsigned int reserved;
	unsigned int flags;
	unsigned int number_of_frames;
	unsigned int initial_frames;
	unsigned int data_streams;
	unsigned int buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int time_scale;
	unsigned int playback_data_rate;
	unsigned int starting_time;
	unsigned int data_length;
};

struct gwavi_stream_header_t
{
	char data_type[5];
	char codec[5];
	unsigned int flags;
	unsigned int priority;
	unsigned int initial_frames;
	unsigned int time_scale;
	unsigned int data_rate;
	unsigned int start_time;
	unsigned int data_length;
	unsigned int buffer_size;
	unsigned int video_quality;
	/**
	 * Value between 0-10000. If set to -1, drivers use default quality
	 * value.
	 */
	int audio_quality;
	unsigned int sample_size;
};

struct gwavi_stream_format_v_t
{
	unsigned int header_size;
	unsigned int width;
	unsigned int height;
	unsigned short int num_planes;
	unsigned short int bits_per_pixel;
	unsigned int compression_type;
	unsigned int image_size;
	unsigned int x_pels_per_meter;
	unsigned int y_pels_per_meter;
	unsigned int colors_used;
	unsigned int colors_important;
	unsigned int *palette;
	unsigned int palette_count;
};

struct gwavi_stream_format_a_t
{
	unsigned short format_type;
	unsigned int channels;
	unsigned int sample_rate;
	unsigned int bytes_per_second;
	unsigned int block_align;
	unsigned int bits_per_sample;
	unsigned short size;
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
	unsigned int *offsets;
	int offset_count;
};

struct gwavi_audio_t
{
	unsigned int channels;
	unsigned int bits;
	unsigned int samples_per_second;
};


/* Main ibrary functions */
struct gwavi_t *gwavi_open(char *filename, unsigned int width,
			   unsigned int height, char *fourcc, unsigned int fps,
			   struct gwavi_audio_t *audio);
int gwavi_add_frame(struct gwavi_t *gwavi, unsigned char *buffer, size_t len);
int gwavi_add_audio(struct gwavi_t *gwavi, unsigned char *buffer, size_t len);
int gwavi_close(struct gwavi_t *gwavi);

/*
 * If needed, these functions can be called before closing the file to
 * change the framerate, codec, size.
 * Note: AVI can only have a single frame rate, codec, size for the whole file
 * so this affects anything recorded before these functions are called.
 */
void gwavi_set_framerate(struct gwavi_t *gwavi, unsigned int fps);
void gwavi_set_codec(struct gwavi_t *gwavi, const char *fourcc);
void gwavi_set_size(struct gwavi_t *gwavi, unsigned int width, unsigned int height);

#endif /* ndef H_GWAVI */

