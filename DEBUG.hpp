#pragma once

#ifdef DEBUG
#undef DEBUG
#define DEBUG(msg) cout << F_FILE_LINE(__FILE__, __LINE__) << endl << msg << endl;
#endif