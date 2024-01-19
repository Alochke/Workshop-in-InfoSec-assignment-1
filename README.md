Conceptually, the idea comes from the facts that:
    1. All allowed packets shall go thraugh NF_IP_LOCAL_IN or NF_IP_LOCAL_OUT Netfilter hook points, and only allowed packets go thraugh those hook points.
    2. All blocked packets go thraugh NF_IP_FORWARD, and only those packets go through this hook point.
    
Thus, we create three diffrent hooks. First two are hooked into NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT, handling allowed packets, and the third is hooked into NF_IP_FORWARD handling the blocked ones.
The hooks are in kernel alloated memory, inside an array, pointed by a global pointer, namely, hooks.

To count the number of accepted and blocked packets we'll maintain two counters, accepted and dropped, defined in hw2secws.h.
The callback function attached to the hooks that are hooked to NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT will increase the accepted counter by one, and similarly the callback function attached to the hook that is hooked to NF_IP_FORWARD will increment the dropped counter by one.

The functionality of the module described in the last three paragraphs is implementted in hw2secws.c and hw2secws.h.

Also, we implement a sysfs part of the kernel module which will be used for a userspace interface to communicate with the module, implemented in sysfs.c and sysfs.h.

============================= hw2secws.c FUNCTIONS =============================


static unsigned int nf_forward_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
    The dropped packet handling procedure.
    This function also increments the dropped packets counter.

static unsigned int nf_local_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state) 
    The allowed packet handling procedure.
    This function also increments the accepted packets counter.

static void destroy_hooks(int max)
    Deletes hook with index less than max from existance and frees their kernel allocated memory.

    Parameters:
    - max (int): Hooks with indecies in [0,max] will be unregisterred.

static int __init LKM_init(void)
    The module initialization function.

static void __exit LKM_exit(void)
    module removal method.

============================= sysfs.c FUNCTIONS ===============================

ssize_t display(struct device *dev, struct device_attribute *attr, char *buf)
    The implementation of show.
	Puts inside of address- buf the values of accepted and dropped,
	which are defined in hw2secws.h and then returns the number of bytes written succefuly.
	On failure, zeros the values at adresses [buff, buf + 2 * sizeof(unsigned int)).

ssize_t modify(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
    The implementation of store.
	Zeros the accepted and dropped packet counters.

void cleanup(enum stage stg)
    Cleans the sysfs part of the module.

	Parameters:
    - stg (enum stage): A designated enum's member that represents the stage of initialization the sysfs part of the module is in.

int sysfs_init(void)
    Initializes the sysfs part of the module.

void sysfs_exit(void)
    A wrapper function of cleanup, that serves as an abstraction layer of the clean up process of the sysfs part of the module.
	In case the initialization of that part of the module is done.W
