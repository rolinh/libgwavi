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

/*
 * Set of functions useful to create an AVI file. It is used to write things
 * such as AVI header and so on.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avi-utils.h"
#include "fileio.h"

int
write_avi_header(FILE *out, struct gwavi_header_t *avi_header)
{
	long marker, t;

	if (write_chars_bin(out, "avih", 4) == -1) {
		(void)fprintf(stderr, "write_avi_header: write_chars_bin() "
			      "failed\n");
		return -1;
	}
	if ((marker = ftell(out)) == -1) {
		perror("write_avi_header (ftell)");
		return -1;
	}
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	if (write_int(out, avi_header->time_delay) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->data_rate) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->reserved) == -1)
		goto write_int_failed;
	/* dwFlags */
	if (write_int(out, avi_header->flags) == -1)
		goto write_int_failed;
	/* dwTotalFrames */
	if (write_int(out, avi_header->number_of_frames) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->initial_frames) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->data_streams) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->buffer_size) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->width) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->height) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->time_scale) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->playback_data_rate) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->starting_time) == -1)
		goto write_int_failed;
	if (write_int(out, avi_header->data_length) == -1)
		goto write_int_failed;

	if ((t = ftell(out)) == -1) {
		perror("write_avi_header (ftell)");
		return -1;
	}
	if (fseek(out, marker, SEEK_SET) == -1) {
		perror("write_avi_header (fseek)");
		return -1;
	}
	if (write_int(out, (unsigned int)(t - marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1) {
		perror("write_avi_header (fseek)");
		return -1;
	}

	return 0;

write_int_failed:
	(void)fprintf(stderr, "write_avi_header: write_int() failed\n");
	return -1;
}

int
write_stream_header(FILE *out, struct gwavi_stream_header_t *stream_header)
{
	long marker, t;

	if (write_chars_bin(out, "strh", 4) == -1)
		goto write_chars_bin_failed;
	if ((marker = ftell(out)) == -1) {
		perror("write_stream_header (ftell)");
		return -1;
	}
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	if (write_chars_bin(out, stream_header->data_type, 4) == -1)
		goto write_chars_bin_failed;
	if (write_chars_bin(out, stream_header->codec, 4) == -1)
		goto write_chars_bin_failed;
	if (write_int(out, stream_header->flags) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->priority) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->initial_frames) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->time_scale) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->data_rate) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->start_time) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->data_length) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->buffer_size) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->video_quality) == -1)
		goto write_int_failed;
	if (write_int(out, stream_header->sample_size) == -1)
		goto write_int_failed;
	if (write_int(out, 0) == -1)
		goto write_int_failed;
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	if ((t = ftell(out)) == -1) {
		perror("write_stream_header (ftell)");
		return -1;
	}
	if (fseek(out, marker, SEEK_SET) == -1) {
		perror("write_stream_header (fseek)");
		return -1;
	}
	write_int(out, (unsigned int)(t - marker - 4));
	if (fseek(out, t, SEEK_SET) == -1){
		perror("write_stream_header (fseek)");
		return -1;
	}

	return 0;

write_int_failed:
	(void)fprintf(stderr, "write_stream_header: write_int() failed\n");
	return -1;

write_chars_bin_failed:
	(void)fprintf(stderr, "write_stream_header: write_chars_bin() failed\n");
	return -1;
}

int
write_stream_format_v(FILE *out, struct gwavi_stream_format_v_t *stream_format_v)
{
	long marker,t;
	unsigned int i;

	if (write_chars_bin(out, "strf", 4) == -1) {
		(void)fprintf(stderr, "write_stream_format_v: write_chars_bin()"
			      " failed\n");
		return -1;
	}
	if ((marker = ftell(out)) == -1) {
		perror("write_stream_format_v (ftell)");
		return -1;
	}
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	if (write_int(out, stream_format_v->header_size) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->width) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->height) == -1)
		goto write_int_failed;
	if (write_short(out, stream_format_v->num_planes) == -1) {
		(void)fprintf(stderr, "write_stream_format_v: write_short() "
			      "failed\n");
		return -1;
	}
	if (write_short(out, stream_format_v->bits_per_pixel) == -1) {
		(void)fprintf(stderr, "write_stream_format_v: write_short() "
			      "failed\n");
		return -1;
	}
	if (write_int(out, stream_format_v->compression_type) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->image_size) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->x_pels_per_meter) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->y_pels_per_meter) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->colors_used) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_v->colors_important) == -1)
		goto write_int_failed;

	if (stream_format_v->colors_used != 0)
		for (i = 0; i < stream_format_v->colors_used; i++) {
			if (fputc(stream_format_v->palette[i] & 255, out)
					== EOF)
				goto fputc_failed;
			if (fputc((stream_format_v->palette[i] >> 8) & 255, out)
					== EOF)
				goto fputc_failed;
			if (fputc((stream_format_v->palette[i] >> 16) & 255, out)
					== EOF)
				goto fputc_failed;
			if (fputc(0, out) == EOF)
				goto fputc_failed;
		}

	if ((t = ftell(out)) == -1) {
		perror("write_stream_format_v (ftell)");
		return -1;
	}
	if (fseek(out,marker,SEEK_SET) == -1) {
		perror("write_stream_format_v (fseek)");
		return -1;
	}
	if (write_int(out, (unsigned int)(t - marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1) {
		perror("write_stream_format_v (fseek)");
		return -1;
	}

	return 0;

write_int_failed:
	(void)fprintf(stderr, "write_stream_format_v: write_int() failed\n");
	return -1;

fputc_failed:
	(void)fprintf(stderr, "write_stream_format_v: fputc() failed\n");
	return -1;
}

int
write_stream_format_a(FILE *out, struct gwavi_stream_format_a_t *stream_format_a)
{
	long marker, t;

	if (write_chars_bin(out, "strf", 4) == -1) {
		(void)fprintf(stderr, "write_stream_format_a: write_chars_bin()"
			      " failed\n");
		return -1;
	}
	if ((marker = ftell(out)) == -1) {
		perror("write_stream_format_a (ftell)");
		return -1;
	}
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	if (write_short(out, stream_format_a->format_type) == -1)
		goto write_short_failed;
	if (write_short(out, stream_format_a->channels) == -1)
		goto write_short_failed;
	if (write_int(out, stream_format_a->sample_rate) == -1)
		goto write_int_failed;
	if (write_int(out, stream_format_a->bytes_per_second) == -1)
		goto write_int_failed;
	if (write_short(out, stream_format_a->block_align) == -1)
		goto write_short_failed;
	if (write_short(out, stream_format_a->bits_per_sample) == -1)
		goto write_short_failed;
	if (write_short(out, stream_format_a->size) == -1)
		goto write_short_failed;

	if ((t = ftell(out)) == -1) {
		perror("write_stream_format_a (ftell)");
		return -1;
	}
	if (fseek(out, marker, SEEK_SET) == -1) {
		perror("write_stream_format_a (fseek)");
		return -1;
	}
	if (write_int(out, (unsigned int)(t - marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1) {
		perror("write_stream_format_a (fseek)");
		return -1;
	}

	return 0;

write_int_failed:
	(void)fprintf(stderr, "write_stream_format_a: write_int() failed\n");
	return -1;

write_short_failed:
	(void)fprintf(stderr, "write_stream_format_a: write_short() failed\n");
	return -1;
}

int
write_avi_header_chunk(struct gwavi_t *gwavi)
{
	long marker, t;
	long sub_marker;
	FILE *out = gwavi->out;

	if (write_chars_bin(out, "LIST", 4) == -1)
		goto write_chars_bin_failed;
	if ((marker = ftell(out)) == -1)
		goto ftell_failed;
	if (write_int(out, 0) == -1)
		goto write_int_failed;
	if (write_chars_bin(out, "hdrl", 4) == -1)
		goto write_chars_bin_failed;
	if (write_avi_header(out, &gwavi->avi_header) == -1) {
		(void)fprintf(stderr, "write_avi_header_chunk: "
			      "write_avi_header() failed\n");
		return -1;
	}

	if (write_chars_bin(out, "LIST", 4) == -1)
		goto write_chars_bin_failed;
	if ((sub_marker = ftell(out)) == -1)
		goto ftell_failed;
	if (write_int(out, 0) == -1)
		goto write_int_failed;
	if (write_chars_bin(out, "strl", 4) == -1)
		goto write_chars_bin_failed;
	if (write_stream_header(out, &gwavi->stream_header_v) == -1) {
		(void)fprintf(stderr, "write_avi_header_chunk: "
			      "write_stream_header failed\n");
		return -1;
	}
	if (write_stream_format_v(out, &gwavi->stream_format_v) == -1) {
		(void)fprintf(stderr, "write_avi_header_chunk: "
			      "write_stream_format_v failed\n");
		return -1;
	}

	if ((t = ftell(out)) == -1)
		goto ftell_failed;

	if (fseek(out, sub_marker, SEEK_SET) == -1)
		goto fseek_failed;
	if (write_int(out, (unsigned int)(t - sub_marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1)
		goto fseek_failed;

	if (gwavi->avi_header.data_streams == 2) {
		if (write_chars_bin(out, "LIST", 4) == -1)
			goto write_chars_bin_failed;
		if ((sub_marker = ftell(out)) == -1)
			goto ftell_failed;
		if (write_int(out, 0) == -1)
			goto write_int_failed;
		if (write_chars_bin(out, "strl", 4) == -1)
			goto write_chars_bin_failed;
		if (write_stream_header(out, &gwavi->stream_header_a) == -1) {
			(void)fprintf(stderr, "write_avi_header_chunk: "
				      "write_stream_header failed\n");
			return -1;
		}
		if (write_stream_format_a(out, &gwavi->stream_format_a) == -1) {
			(void)fprintf(stderr, "write_avi_header_chunk: "
				      "write_stream_format_a failed\n");
			return -1;
		}

		if ((t = ftell(out)) == -1)
			goto ftell_failed;
		if (fseek(out, sub_marker, SEEK_SET) == -1)
			goto fseek_failed;
		if (write_int(out, (unsigned int)(t - sub_marker - 4)) == -1)
			goto write_int_failed;
		if (fseek(out, t, SEEK_SET) == -1)
			goto fseek_failed;
	}

	if ((t = ftell(out)) == -1)
		goto ftell_failed;
	if (fseek(out, marker, SEEK_SET) == -1)
		goto fseek_failed;
	if (write_int(out, (unsigned int)(t - marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1)
		goto fseek_failed;

	return 0;

ftell_failed:
	perror("write_avi_header_chunk (ftell)");
	return -1;

fseek_failed:
	perror("write_avi_header_chunk (fseek)");
	return -1;

write_int_failed:
	(void)fprintf(stderr, "write_avi_header_chunk: write_int() failed\n");
	return -1;

write_chars_bin_failed:
	(void)fprintf(stderr, "write_avi_header_chunk: write_chars_bin() failed\n");
	return -1;
}

int
write_index(FILE *out, int count, unsigned int *offsets)
{
	long marker, t;
	unsigned int offset = 4;

	if (offsets == 0)
		return -1;

	if (write_chars_bin(out, "idx1", 4) == -1) {
		(void)fprintf(stderr, "write_index: write_chars_bin) failed\n");
		return -1;
	}
	if ((marker = ftell(out)) == -1) {
		perror("write_index (ftell)");
		return -1;
	}
	if (write_int(out, 0) == -1)
		goto write_int_failed;

	for (t = 0; t < count; t++) {
		if ((offsets[t] & 0x80000000) == 0)
			write_chars(out, "00dc");
		else {
			write_chars(out, "01wb");
			offsets[t] &= 0x7fffffff;
		}
		if (write_int(out, 0x10) == -1)
			goto write_int_failed;
		if (write_int(out, offset) == -1)
			goto write_int_failed;
		if (write_int(out, offsets[t]) == -1)
			goto write_int_failed;

		offset = offset + offsets[t] + 8;
	}

	if ((t = ftell(out)) == -1) {
		perror("write_index (ftell)");
		return -1;
	}
	if (fseek(out, marker, SEEK_SET) == -1) {
		perror("write_index (fseek)");
		return -1;
	}
	if (write_int(out, (unsigned int)(t - marker - 4)) == -1)
		goto write_int_failed;
	if (fseek(out, t, SEEK_SET) == -1) {
		perror("write_index (fseek)");
		return -1;
	}

	return 0;

write_int_failed:
	(void)fprintf(stderr, "write_index: write_int() failed\n");
	return -1;
}

