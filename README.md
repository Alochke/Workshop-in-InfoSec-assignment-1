Conceptually, the idea comes from the facts that:
    1. All allowed packets shall go thraugh NF_IP_LOCAL_IN or NF_IP_LOCAL_OUT Netfilter hook points, and only allowed packets go thraugh those hook points.
    2. All blocked packets go thraugh NF_IP_FORWARD, and only those packets go through this hook point.
    
Thus, we create three diffrent hooks. First two are hooked into NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT, handling allowed packets, and the third is hooked into NF_IP_FORWARD handling the blocked ones.
The hooks are in kernel alloated memory, inside an array, pointed by a global pointer, namely, hooks.

To count the number of accepted and blocked packets we'll maintain two counters, accepted and dropped.
The callback function attached to the hooks that are hooked to NF_IP_LOCAL_IN and NF_IP_LOCAL_OUT will increase the accepted counter by one, and similarly the callback function attached to the hook that is hooked to NF_IP_FORWARD will increment the dropped counter by one.

The functionality of the module described in the last three paragraphs is implementted in hw2secws.c and hw2secws.h.

Also, we implement a sysfs part of the kernel module which will be used for a user space interface to communicate with the module. The sysfs kernel part is implemented in sysfs.c and sysfs.h.

fw_userspace.c implements the user space progrmam that interacts with the sysfs part of the module, it utilizes the show function of the sysfs interface to serve the functionality that the userspace program should serve when given no inputs and uses the store function to serve the functionality it should when give 0 as an input, other than that it's a trivial program.

============================= hw2secws.c FUNCTIONS =============================


static unsigned int nf_forward_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state)
    The dropped packet handling procedure.
    This function also increments the dropped packets counter.

    Returns: NF_DROP which is defined in netfilter.h and makes the handler drop the packet.

static unsigned int nf_local_fn(void* priv, struct sk_buff *skb, const struct nf_hook_ops *state) 
    The allowed packet handling procedure.
    This function also increments the accepted packets counter.

    Returns: NF_ACCEPT which is defined in netfilter.h and makes the handler accept the packet.

static void destroy_hooks(size_t max)
    Deletes hooks with indecies less than max from existance and frees their kernel allocated memory.

    Parameters:
    - max (size_t): Hooks with indecies in [0,max] will be unregisterred.

static int __init LKM_init(void)
    The module initialization function.

    Returns: -ENOMEM in case it was not able to allocate memory for the nf_hook_ops structs, 0 on success and the value returned by the function that failed in any case a function called by LKM _init failed.

static void __exit LKM_exit(void)
    module removal method.

============================= sysfs.c FUNCTIONS ===============================

ssize_t display(struct device *dev, struct device_attribute *attr, char *buf)
    The implementation of show.
	Puts inside of address- buf the values of accepted and dropped.
    
    Returns: The number of bytes transfered.

ssize_t modify(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
    The implementation of store.
	Zeros the accepted and dropped packet counters.

    Returns: 0, i.e, the bytes from the sysfs buffer modified, according to standard practice with the sysfs api.

void cleanup(enum stage stg)
    Cleans the sysfs part of the module.

	Parameters:
    - stg (enum stage): A designated enum's member that represents the stage of initialization the sysfs part of the module is at.

int sysfs_init(void)
    Initializes the sysfs part of the module.

void sysfs_exit(void)
    A wrapper function of cleanup, that serves as an abstraction layer of the clean up process of the sysfs part of the module,
	In case the initialization of that part of the module is done.
