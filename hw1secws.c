/*
    I based my code on this article:
        https://infosecwriteups.com/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e
    
    Conceptually, the idea comes from the facts that:
        1. All allowed packets shall go thraugh NF_IP_LOCAL_IN or NF_IP_LOCAL_OUT Netfilter hook points, and only allowed packets go thraugh those hook points.
        2. All blocked packets go thraugh NF_IP_FORWARD, and only those packets go through this hook point.
    
    Those we create three diffrent hooks, first two are hooked into NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT, handling allowed packets, and the third is hooked into NF_IP_FORWARD handling the blocked ones.
*/

#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/slab.h>     /* For kmaloc and kfree */
#include <linux/errno.h>    /* For standard error numbering */
#include <linux/netfilter.h>/* Next two includes are for the Netfilter API */
#include <linux/netfilter_ipv4.h>

#define ALLOW_MESSAGE "*** Packet Accepted ***"
#define BLOCK_MESSAGE "*** Packet Dropped ***"
#define HOOKS_NUM 3 /* The number of nf_hook_ops structs We'll use */
#define IN 0 /* The index of the NF_IP_LOCAL_IN hook */
#define OUT 1 /* The index of the NF_IP_LOCAL_OUT hook */
#define FORWARD 2 /* The index of the NF_IP_LOCAL_FORWARD hook */

#define ERR_CHECK(condition, extra_code, function, errno_value){\
    if(condition){\
        extra_code;\
        printk(KERN_ERR funtion " failed.");\
        return errno_value;\
    }\
}

/* Declaring the license as the free GNU Public Licencse, as we were taught one should do when writing a kernel module in OS course */
MODULE_LICENSE("GPL");

/* All nf_hook_ops will be pointed by the hooks array */
static struct nf_hook_ops hooks[HOOKS_NUM];

/* The dropped packet handling procedure */
static unsigned int nf_forward_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
{
    printk(BLOCK_MESSAGE);
    return NF_DROP;
}

/* The allowed packet handling procedure */
static unsigned int nf_local_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
{
    printk(ALLOW_MESSAGE);
    return NF_ACCEPT;
}

/* Deletes hook with index less than max from existance */
static void destroy_hooks(int max)
{
    size_t i;   /* for loop's index */
    for (i = 0; i < max; i++)
    {
        nf_unregister_net_hook(&init_net, &hooks[i]);
        kfree(&hooks[i]);
    }
}

static int __init LKM_init(void)
{
    size_t i;   /* for loop's index */

    for(i = 0; i < HOOKS_NUM; i++)
    {
        ERR_CHECK(hooks = (struct nf_hook_ops*)kmalloc(sizeof(struct nf_hook_ops), GFP_KERNEL), destroy_hooks(i), "kmalloc", -EMVSDYNALC)
        hook[i]->pf = PF_INET;                      /* IPv4 */
        hook[i]->priority 	= NF_IP_PRI_FIRST;		/* max hook priority */
        switch (i)                                  /* Netfilter hook point and hook function */
        {
        case FORWARD:
            hook[i]->hook = (nf_hookfn*) nf_forward_fn;
            hook[i]->hooknum = NF_IP_FORWARD;
            break;
        default:
            hook[i]->hook = (nf_hookfn*) nf_local_fn;
            switch (i)
            {
            case IN:
                hook[i]->hooknum = NF_IP_LOCAL_IN;
                break;
            default:
                hook[i]->hooknum = NF_IP_LOCAL_OUT; /* The only hook point left possible */
            }
        }
        ERR_CHECK(int err = (nf_register_net_hook(&init_net, &hook[i]) < 0), destroy_hooks(i), "nf_register_net_hook", err);
    }
}

static void __exit LKM_exit(void)
{
    destroy_hooks(HOOKS_NUM);
}

module_init(LKM_init);
module_exit(LKM_exit);
