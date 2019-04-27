// Copyright 2016 Gu Zhengxiong <rectigu@gmail.com>
//
// This file is part of LibZeroEvil.
//
// LibZeroEvil is free software:
// you can redistribute it and/or modify it
// under the terms of the GNU General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// LibZeroEvil is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LibZeroEvil.
// If not, see <http://www.gnu.org/licenses/>.


# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/kernel.h>

# include <linux/slab.h>
// // __NR_close.
# include <linux/syscalls.h>
// // struct linux_dirent64.
# include <linux/dirent.h>
//# include "zeroevil/zeroevil.h"

# define fn_printk(level, fmt, ...)                     \
        printk(level "%s: " fmt, __func__, ##__VA_ARGS__)

// INFO: ``fm`` is short for ``__func__`` and ``module``.
# define fm_printk(level, fmt, ...)                     \
printk(level "%s.%s: " fmt,                        \
     THIS_MODULE->name, __func__, ##__VA_ARGS__)
              // INFO: I only use ``pr_alert`` at present.
// TODO: When wanting more, e.g. ``pr_info``, I will add them.
                # define fn_alert(fmt, ...) \
                    fn_printk(KERN_ALERT, fmt, ##__VA_ARGS__)
//
                    # define fm_alert(fmt, ...) \                                                      
                        fm_printk(KERN_ALERT, fmt, ##__VA_ARGS__)
MODULE_LICENSE("GPL");

unsigned long **real_sys_call_table;

unsigned long **
get_sct()
{
    unsigned long **entry = (unsigned long **)PAGE_OFFSET;

    for (;(unsigned long)entry<ULONG_MAX;entry+=1) {
        if (entry[__NR_close] == (unsigned long *)sys_close) {
            return entry;
        }
    }
    return NULL;
}

int
//init_module(void)
modulestart(void)
{
    fm_alert("%s\n", "init ok");

    real_sys_call_table = get_sct();

    fm_alert("PAGE_OFFSET = %lx\n", PAGE_OFFSET);
    fm_alert("sys_call_table = %p\n", real_sys_call_table);
    fm_alert("sys_call_table - PAGE_OFFSET = %lu MiB\n",
             ((unsigned long)real_sys_call_table -
              (unsigned long)PAGE_OFFSET) / 1024 / 1024);
    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");

    return;
}

module_init(modulestart);
