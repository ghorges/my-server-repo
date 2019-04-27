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

void disable_write_protection(void)
{
    unsigned long cr0 = read_cr0();
    clear_bit(16,&cr0);
    write_cr0(cr0);
}

void enable_write_protection(void)
{
    unsigned long cr0 = read_cr0();
    set_bit(16,&cr0);
    write_cr0(cr0);
}

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


asmlinkage long
(*real_open)(const char __user *filename, int flags, umode_t mode);
asmlinkage long
(*real_unlink)(const char __user *pathname);
asmlinkage long
(*real_unlinkat)(int dfd, const char __user * pathname, int flag);

asmlinkage long
fake_open(const char __user *filename, int flags, umode_t mode)
{
    if ((flags & O_CREAT) && strcmp(filename, "/dev/null") != 0) {
            printk(KERN_ALERT "open: %s\n", filename);
              
    }

      return real_open(filename, flags, mode);

}

asmlinkage long
fake_unlink(const char __user *pathname)
{
      printk(KERN_ALERT "unlink: %s\n", pathname);

        return real_unlink(pathname);

}

asmlinkage long
fake_unlinkat(int dfd, const char __user * pathname, int flag)
{
      printk(KERN_ALERT "unlinkat: %s\n", pathname);

        return real_unlinkat(dfd, pathname, flag);

}

int
init_module(void)
{
    fm_alert("%s\n", "init ok");

    real_sys_call_table = get_sct();

    fm_alert("PAGE_OFFSET = %lx\n", PAGE_OFFSET);
    fm_alert("sys_call_table = %p\n", real_sys_call_table);
    fm_alert("sys_call_table - PAGE_OFFSET = %lu MiB\n",
             ((unsigned long)real_sys_call_table -
              (unsigned long)PAGE_OFFSET) / 1024 / 1024);
    
    disable_write_protection();

    real_open = (void *)real_sys_call_table[__NR_open];
    real_sys_call_table[__NR_open] = (unsigned long*)fake_open;
    real_unlink = (void *)real_sys_call_table[__NR_unlink];
    real_sys_call_table[__NR_unlink] = (unsigned long*)fake_unlink;
    real_unlinkat = (void *)real_sys_call_table[__NR_unlinkat];
    real_sys_call_table[__NR_unlinkat] = (unsigned long*)fake_unlinkat;
    enable_write_protection();

    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");
    disable_write_protection();
    real_sys_call_table[__NR_open] = (unsigned long*)real_open;
    real_sys_call_table[__NR_unlink] = (unsigned long*)real_unlink;
    real_sys_call_table[__NR_unlinkat] = (unsigned long*)real_unlinkat;
    enable_write_protection();
    return;
}
