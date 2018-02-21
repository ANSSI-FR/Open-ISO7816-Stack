#include "reader.h"
#include "reader_utils.h"


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di){
	return freq / (Fi/Di);
}
