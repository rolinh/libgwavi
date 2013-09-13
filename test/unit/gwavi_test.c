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

    sput_enter_suite("test check fourcc");
    sput_run_test(check_fourcc_test);

    sput_finish_testing();

    return sput_get_return_value();
}

/* API functions */

static void
gwavi_open_test(void)
{
	sput_fail_unless(gwavi_open("/root/nopermission", 12, 20, "H264", 30,
		                    NULL) == NULL, "no R/W permissions");
	sput_fail_unless(gwavi_open("/tmp/sadfpoisadf/foo.avi", 12, 20, "H264",
				   30, NULL) == NULL, "no such directory");
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
