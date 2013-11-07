/*
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
 * demo.c
 *
 * Demo application (that still needs to be written...)
 */
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "gwavi.h"

#define FILENAME_LEN 17

int
main(void)
{
	struct gwavi_t *gwavi;		  /* declare main gwavi structure */
	/*struct gwavi_audio_t audio;*/	  /* declare structure used for audio */
	unsigned int width = 320;	  /* set video width */
	unsigned int height = 240;	  /* set video height */
	unsigned int fps = 3;		  /* set number of frames per second */
	char *fourcc = "MJPG";		  /* set fourcc used */
	char *avi_out = "example.avi";    /* set out file name */

	struct stat frame_stat;
	char filename[FILENAME_LEN];
	unsigned char *buffer;
	ssize_t r;
	size_t count, len, buffer_len = 0;
	int i, fd, ret;

	/* TODO: add audio */
	/* gwavi = gwavi_open(avi_out, width, height, fourcc, fps, &audio); */
	gwavi = gwavi_open(avi_out, width, height, fourcc, fps, NULL);
	if (!gwavi) {
		(void)fprintf(stderr, "Call to gwavi_open() failed!\n");
		return EXIT_FAILURE;
	}

	/* read 15 jpg images that will act as frames */
	for (i = 1; i < 16; i++) {
		ret = sprintf(filename, "video-src/%02d.jpg", i);
		if (ret != FILENAME_LEN - 1) {
			if (ret < 0) {
				(void)fprintf(stderr, "An error occured during "
				    " snprintf call\n");
			} else {
				(void)fprintf(stderr, "Name of file to read was"
				    " truncated during sprintf call\n");
			}
			return EXIT_FAILURE;
		}

		fd = open(filename, O_RDONLY);
		if (fd == -1) {
			(void)fprintf(stderr, "Cannot open %s for reading\n",
			    filename);
			perror(" ");
			return EXIT_FAILURE;
		}

		if (fstat(fd, &frame_stat) == -1) {
			(void)fprintf(stderr, "Could not stat frame\n");
			perror(" ");
			return EXIT_FAILURE;
		}
		/* FIXME */
		len = frame_stat.st_size;
		if (len > buffer_len) {
			if (buffer_len > 0)
				free(buffer);
			buffer_len = len;
			buffer = malloc(buffer_len);
		}
		count = 0;
		while (count < len) {
			r = read(fd, buffer + count, len - count);
			if (r < 0) {
				(void)fprintf(stderr, "Failed to read from "
				    "buffer\n");
				perror(" ");
				return EXIT_FAILURE;
			}
			count += (size_t)r;
		}

		if (close(fd) == -1) {
			(void)fprintf(stderr, "Cannot close file descriptor\n");
			perror(" ");
			return EXIT_FAILURE;
		}

		if (gwavi_add_frame(gwavi, buffer, len) == -1) {
			(void)fprintf(stderr, "Cannot add frame to video\n");
			return EXIT_FAILURE;
		}
	}

	if (gwavi_close(gwavi) == -1) {
		(void)fprintf(stderr, "Call to gwavi_close() failed!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

