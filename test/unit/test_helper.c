#include "sput.h"

int
main(void)
{
    sput_start_testing();

    sput_enter_suite("");
    sput_run_test(gwavi_open_test);

    sput_finish_testing();

    return sput_get_return_value();
}
