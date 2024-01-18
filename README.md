I based my code on this article:
    https://infosecwriteups.com/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e
    
Conceptually, the idea comes from the facts that:
    1. All allowed packets shall go thraugh NF_IP_LOCAL_IN or NF_IP_LOCAL_OUT Netfilter hook points, and only allowed packets go thraugh those hook points.
    2. All blocked packets go thraugh NF_IP_FORWARD, and only those packets go through this hook point.
    
Thus, we create three diffrent hooks. First two are hooked into NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT, handling allowed packets, and the third is hooked into NF_IP_FORWARD handling the blocked ones.

The hooks are in kernel alloated memory, inside an array, pointed by a global pointer, namely, hooks.


============================= FUNCTIONS =============================


static unsigned int nf_forward_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
    The dropped packet handling procedure

static unsigned int nf_local_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
    The allowed packet handling procedure

static void destroy_hooks(int max)
    Deletes hook with index less than max from existance and frees kernel's allocated memory.

    Parameters:
    - max (int): Hooks with indecies in [0,max] will be unregisterred.

static int __init LKM_init(void)
    The module initialization function.

static void __exit LKM_exit(void)
    module removal method.