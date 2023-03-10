#include "graindb/common/printer.hpp"

#include <stdio.h>

using namespace graindb;

void Printer::Print(string str) {
	fprintf(stderr, "%s\n", str.c_str());
}
