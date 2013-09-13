#include "sput.h"
#include "gwavi.h"
#include "gwavi_test.h"

int
main(void)
{
    sput_start_testing();

    sput_enter_suite("test gwavi_open");
    sput_run_test(gwavi_open_test);

    sput_finish_testing();

    return sput_get_return_value();
}

static void
gwavi_open_test(void)
{
	sput_fail_unless(gwavi_open("/root/nopermission", 12, 20, "H264", 30,
		                    NULL) == NULL, "no R/W permissions");
	sput_fail_unless(gwavi_open("/tmp/sadfpoisadf/foo.avi", 12, 20, "H264",
				   30, NULL) == NULL, "no such directory");
}

