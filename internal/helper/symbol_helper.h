#ifndef REDPILL_SYMBOL_HELPER_H
#define REDPILL_SYMBOL_HELPER_H

#include <linux/types.h> //bool

unsigned long kln(const char *name);

/**
 * Check if a given symbol exists
 *
 * This function will return true for both public and private kernel symbols
 *
 * @param name name of the symbol
 */
bool kernel_has_symbol(const char *name);

#endif //REDPILL_SYMBOL_HELPER_H
