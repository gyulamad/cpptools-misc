#include "../TEST.hpp"
#include "../ConsoleLogger.hpp"

#include "test_str_ends_with.hpp"
#include "test_str_starts_with.hpp"
#include "test_parse.hpp"
#include "test_in_array.hpp"
#include "test_array_key_exists.hpp"
#include "test_str_cut_end.hpp"
#include "test_ms_to_datetime.hpp"
#include "test_get_time_ms.hpp"
#include "test_implode.hpp"
#include "test_capture_cout_cerr.hpp"
#include "test_fix_path.hpp"
#include "test_ShorthandGenerator.hpp"
#include "test_Arguments.hpp"

int main() {
    createLogger<ConsoleLogger>();
    tester.run();
}