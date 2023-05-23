#include "symbol_helper.h"
#include <linux/module.h> //__symbol_get(), __symbol_put()
#include <linux/kallsyms.h> //kallsyms_lookup_name()
#include <linux/version.h> //KERNEL_VERSION()

/*
 * On Linux kernels 5.7+, kallsyms_lookup_name() is no longer exported,
 * so we have to use kprobes to get the address.
 * Full credit to @f0lg0 for the idea.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#define KPROBE_LOOKUP 1
#include <linux/kprobes.h>
struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};
#endif

unsigned long kln(const char *name)
{
#ifdef KPROBE_LOOKUP
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
    unregister_kprobe(&kp);
#endif

    return kallsyms_lookup_name(name);
}

bool kernel_has_symbol(const char *name) {
    if (__symbol_get(name)) { //search for public symbols
        __symbol_put(name);

        return true;
    }

    return kln(name) != 0;
}