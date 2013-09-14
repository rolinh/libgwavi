#include "sput.h"

#include "avi-utils.h"
#include "gwavi.h"
#include "gwavi_test.h"

int
main(void)
{
    sput_start_testing();

    sput_enter_suite("test gwavi_open");
    sput_run_test(gwavi_open_test);

    sput_enter_suite("test gwavi_add_frame");
    sput_run_test(gwavi_add_frame_test);

    sput_enter_suite("test gwavi_add_audio");
    sput_run_test(gwavi_add_audio_test);

    sput_enter_suite("test gwavi_close");
    sput_run_test(gwavi_close_test);

    sput_enter_suite("test gwavi_set_framerate");
    sput_run_test(gwavi_set_framerate_test);

    sput_enter_suite("test gwavi_set_codec");
    sput_run_test(gwavi_set_codec_test);

    sput_enter_suite("test gwavi_set_size");
    sput_run_test(gwavi_set_size_test);

    sput_enter_suite("test check fourcc");
    sput_run_test(check_fourcc_test);

    sput_finish_testing();

    return sput_get_return_value();
}

/* API functions */
static void
gwavi_open_test(void)
{	struct gwavi_audio_t audio;

	sput_fail_unless(gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30,
				    NULL) != NULL, "valid call to gwavi_open");
	sput_fail_unless(gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30,
				    &audio) != NULL, "valid call to gwavi_open "
				    "with audio");
	sput_fail_unless(gwavi_open("/root/nopermission", 12, 20, "H264", 30,
		                    NULL) == NULL, "no R/W permissions");
	sput_fail_unless(gwavi_open("/tmp/sadfpoisadf/foo.avi", 12, 20, "H264",
				   30, NULL) == NULL, "no such directory");
}

static void
gwavi_add_frame_test(void)
{
	struct gwavi_t *gwavi;
	unsigned char *buffer;
	size_t buffer_len = 8192;

	buffer = (unsigned char *)malloc(buffer_len);

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, NULL);

	sput_fail_unless(gwavi_add_frame(gwavi, buffer, buffer_len) == 0,
			 "valid call to gwavi_add_frame");
	sput_fail_unless(gwavi_add_frame(NULL, buffer, buffer_len) == -1,
			 "NULL gwavi structure");
	sput_fail_unless(gwavi_add_frame(gwavi, NULL, buffer_len) == -1,
			"NULL buffer parameter");
	sput_fail_unless(gwavi_add_frame(gwavi, buffer, 0) == 0,
			"buffer of size 0");
}

static void
gwavi_add_audio_test(void)
{
	struct gwavi_t *gwavi;
	struct gwavi_audio_t audio;
	unsigned char *buffer;
	size_t buffer_len = 8192;
	size_t snd_buffer_len = 2048;
	unsigned char snd_buffer[snd_buffer_len];

	buffer = (unsigned char *)malloc(buffer_len);

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, &audio);

	sput_fail_unless(gwavi_add_audio(gwavi, snd_buffer, snd_buffer_len) == 0,
			 "valid call to gwavi_add_audio");
	sput_fail_unless(gwavi_add_audio(NULL, snd_buffer, snd_buffer_len) == -1,
			 "NULL gwavi parameter");
	sput_fail_unless(gwavi_add_audio(gwavi, NULL, snd_buffer_len) == -1,
			 "NULL gwavi parameter");
}

static void
gwavi_close_test(void)
{
	struct gwavi_t *gwavi;

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, NULL);
	sput_fail_unless(gwavi_close(gwavi) == 0, "valid call to gwavi_close");
	sput_fail_unless(gwavi_close(NULL) == -1, "NULL gwavi parameter");

}

static void
gwavi_set_framerate_test(void)
{
	struct gwavi_t *gwavi;

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, NULL);

	sput_fail_unless(gwavi_set_framerate(gwavi, 30) == 0, "valid call to "
			 "gwavi_set_framerate");
	sput_fail_unless(gwavi_set_framerate(NULL, 30) == -1, "NULL gwavi "
			 "parameter");
}

static void
gwavi_set_codec_test(void)
{
	struct gwavi_t *gwavi;

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, NULL);

	sput_fail_unless(gwavi_set_codec(gwavi, "H264") == 0, "valid call to "
			 "gwavi_set_codec");
	sput_fail_unless(gwavi_set_codec(gwavi, "foobar") == 0, "valid call to "
			 "gwavi_set_codec but weird fourcc");
	sput_fail_unless(gwavi_set_codec(NULL, "H264") == -1, "NULL gwavi "
			 "parameter");
}

static void
gwavi_set_size_test(void)
{
	struct gwavi_t *gwavi;

	gwavi = gwavi_open("/tmp/foo.avi", 1920, 1080, "H264", 30, NULL);

	sput_fail_unless(gwavi_set_size(gwavi, 1280, 720) == 0, "valid call to "
			 "gwavi_set_size");
	sput_fail_unless(gwavi_set_size(NULL, 1280, 720) == -1, "NULL gwavi "
			 "parameter");
}

/* helpers functions */
static void
check_fourcc_test(void)
{
	sput_fail_unless(check_fourcc(" FGF") == 1, "fourcc with space");
	sput_fail_unless(check_fourcc("V1 AV") == 1, "other fourcc with space");
	sput_fail_unless(check_fourcc(NULL) == -1, "NULL fourcc");
	sput_fail_unless(check_fourcc("H264") == 0, "valid fourcc");
}
