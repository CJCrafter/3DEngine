#pragma once

// Exception location macro
#define EXCEPTION(result) HRException(__LINE__, __FILE__, result)
#define LAST_EXCEPTION() HRException(__LINE__, __FILE__, GetLastError())