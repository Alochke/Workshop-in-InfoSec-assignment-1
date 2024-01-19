#ifndef HW2SECWS
#define HW2SECWS
#include <linux/kernel.h>   /* We're doing kernel work. */
#include <linux/module.h>   /* We're adding a kernel module. */
#include <linux/slab.h>     /* For kmaloc and kfree. */
#include <linux/errno.h>    /* For standard error numbering. */
#include <linux/netfilter.h>/* Next two includes are for the Netfilter API */
#include <linux/netfilter_ipv4.h>
#include "sysfs.h"

#define ALLOW_MESSAGE "*** Packet Accepted ***\n" /*The \n is apparently necessary for immediate printing of printk.*/
#define BLOCK_MESSAGE "*** Packet Dropped ***\n"
#define HOOKS_NUM 3 /* The number of nf_hook_ops structs We'll use */
#define IN 0 /* The index of the NF_IP_LOCAL_IN hook */
#define OUT 1 /* The index of the NF_IP_LOCAL_OUT hook */
#define FORWARD 2 /* The index of the NF_IP_LOCAL_FORWARD hook */
/* 
    The next part is kind of disgusting, sorry.
    I wrote it only because I found out that the only way to use those MACROS is to undef __kernel__ so we can get the macros from linux/netfilter_ipv4.h...
    But, if we'll do that the compiler will inevitably try to import limits.h, which is undefined, I suppose because we are compiling kernel-space code.
    Anyway, because I felt like that's the start of falling down a rabbit hole, I deccided to go with this less beutiful soulution of simply defining the macros.
*/
#define NF_IP_LOCAL_IN          1 /* If the packet is destined for this box. */
#define NF_IP_FORWARD           2 /* If the packet is destined for another interface. */
#define NF_IP_LOCAL_OUT         3 /* Packets coming from a local process. */
#define NUMBER_OF_ENUMARATORS   2 /* Refers to accepted and dropped which are declared in hw2secws.h. */

#define MODULE_SIGNATURE                                                                \
    MODULE_LICENSE("GPL");                                                              \
    MODULE_AUTHOR("Alon Polsky");                                                       \
    MODULE_DESCRIPTION("HW2 solution for the course Workshop in Information Security.");\

extern unsigned int accepted; /* An enumerator for the accepted number of packets. */
extern unsigned int dropped;  /* An enumerator for the dropped number of packets. */

#define ERR_CHECK(condition, extra_code, errno_value){          \
    if(condition){                                              \
        extra_code;                                             \
        return errno_value;                                     \
    }                                                           \
}

#endif