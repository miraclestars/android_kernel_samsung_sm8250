/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/capability.h>
#include <linux/socket.h>

#define COMMON_FILE_SOCK_PERMS "ioctl", "read", "write", "create", \
    "getattr", "setattr", "lock", "relabelfrom", "relabelto", "append", "map"

#define COMMON_FILE_PERMS COMMON_FILE_SOCK_PERMS, "unlink", "link", \
    "rename", "execute", "quotaon", "mounton", "audit_access", \
    "open", "execmod"

#define COMMON_SOCK_PERMS COMMON_FILE_SOCK_PERMS, "bind", "connect", \
    "listen", "accept", "getopt", "setopt", "shutdown", "recvfrom",  \
    "sendto", "name_bind"

#define COMMON_IPC_PERMS "create", "destroy", "getattr", "setattr", "read", \
	    "write", "associate", "unix_read", "unix_write"

#define COMMON_CAP_PERMS  "chown", "dac_override", "dac_read_search", \
	    "fowner", "fsetid", "kill", "setgid", "setuid", "setpcap", \
	    "linux_immutable", "net_bind_service", "net_broadcast", \
	    "net_admin", "net_raw", "ipc_lock", "ipc_owner", "sys_module", \
	    "sys_rawio", "sys_chroot", "sys_ptrace", "sys_pacct", "sys_admin", \
	    "sys_boot", "sys_nice", "sys_resource", "sys_time", \
	    "sys_tty_config", "mknod", "lease", "audit_write", \
	    "audit_control", "setfcap"

#define COMMON_CAP2_PERMS  "mac_override", "mac_admin", "syslog", \
		"wake_alarm", "block_suspend", "audit_read"

#if CAP_LAST_CAP > CAP_AUDIT_READ
#error New capability defined, please update COMMON_CAP2_PERMS.
#endif

/*
 * Note: The name for any socket class should be suffixed by "socket",
 *	 and doesn't contain more than one substr of "socket".
 */
struct security_class_mapping secclass_map[] = {
	{ "security",
	  { "compute_av", "compute_create", "compute_member",
	    "check_context", "load_policy", "compute_relabel",
	    "compute_user", "setenforce", "setbool", "setsecparam",
	    "setcheckreqprot", "read_policy", "validate_trans", NULL } },
	{ "process",
	  { "fork", "transition", "sigchld", "sigkill",
	    "sigstop", "signull", "signal", "ptrace", "getsched", "setsched",
	    "getsession", "getpgid", "setpgid", "getcap", "setcap", "share",
	    "getattr", "setexec", "setfscreate", "noatsecure", "siginh",
	    "setrlimit", "rlimitinh", "dyntransition", "setcurrent",
	    "execmem", "execstack", "execheap", "setkeycreate",
	    "setsockcreate", "getrlimit", NULL } },
	{ "process2",
	  { "nnp_transition", "nosuid_transition", NULL } },
	{ "system",
	  { "ipc_info", "syslog_read", "syslog_mod",
	    "syslog_console", "module_request", "module_load", NULL } },
	{ "capability",
	  { COMMON_CAP_PERMS, NULL } },
	{ "filesystem",
	  { "mount", "remount", "unmount", "getattr",
	    "relabelfrom", "relabelto", "associate", "quotamod",
	    "quotaget", NULL } },
	{ "file",
	  { COMMON_FILE_PERMS,
	    "execute_no_trans", "entrypoint", NULL } },
	{ "dir",
	  { COMMON_FILE_PERMS, "add_name", "remove_name",
	    "reparent", "search", "rmdir", NULL } },
	{ "fd", { "use", NULL } },
	{ "lnk_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "chr_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "blk_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "sock_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "fifo_file",
	  { COMMON_FILE_PERMS, NULL } },
	{ "socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "tcp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", "name_connect",
	    NULL } },
	{ "udp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "rawip_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "node",
	  { "recvfrom", "sendto", NULL } },
	{ "netif",
	  { "ingress", "egress", NULL } },
	{ "netlink_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "packet_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "key_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "unix_stream_socket",
	  { COMMON_SOCK_PERMS, "connectto", NULL } },
	{ "unix_dgram_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "sem",
	  { COMMON_IPC_PERMS, NULL } },
	{ "msg", { "send", "receive", NULL } },
	{ "msgq",
	  { COMMON_IPC_PERMS, "enqueue", NULL } },
	{ "shm",
	  { COMMON_IPC_PERMS, "lock", NULL } },
	{ "ipc",
	  { COMMON_IPC_PERMS, NULL } },
	{ "netlink_route_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", "nlmsg_readpriv", NULL } },
	{ "netlink_tcpdiag_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_nflog_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_xfrm_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_selinux_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_iscsi_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_audit_socket",
	  { COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", "nlmsg_relay", "nlmsg_readpriv",
	    "nlmsg_tty_audit", NULL } },
	{ "netlink_fib_lookup_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_connector_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_netfilter_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_dnrt_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "association",
	  { "sendto", "recvfrom", "setcontext", "polmatch", NULL } },
	{ "netlink_kobject_uevent_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_generic_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_scsitransport_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_rdma_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netlink_crypto_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "appletalk_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "packet",
	  { "send", "recv", "relabelto", "forward_in", "forward_out", NULL } },
	{ "key",
	  { "view", "read", "write", "search", "link", "setattr", "create",
	    NULL } },
	{ "dccp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", "name_connect", NULL } },
	{ "memprotect", { "mmap_zero", NULL } },
	{ "peer", { "recv", NULL } },
	{ "capability2",
	  { COMMON_CAP2_PERMS, NULL } },
	{ "kernel_service", { "use_as_override", "create_files_as", NULL } },
	{ "tun_socket",
	  { COMMON_SOCK_PERMS, "attach_queue", NULL } },
	{ "binder", { "impersonate", "call", "set_context_mgr", "transfer",
		      NULL } },
	{ "cap_userns",
	  { COMMON_CAP_PERMS, NULL } },
	{ "cap2_userns",
	  { COMMON_CAP2_PERMS, NULL } },
	{ "sctp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", "name_connect", "association", NULL } },
	{ "icmp_socket",
	  { COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "ax25_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "ipx_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "netrom_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "atmpvc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "x25_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "rose_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "decnet_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "atmsvc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "rds_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "irda_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "pppox_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "llc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "can_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "tipc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "bluetooth_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "iucv_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "rxrpc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "isdn_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "phonet_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "ieee802154_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "caif_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "alg_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "nfc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "vsock_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "kcm_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "qipcrtr_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "smc_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "infiniband_pkey",
	  { "access", NULL } },
	{ "infiniband_endport",
	  { "manage_subnet", NULL } },
	{ "bpf",
	  {"map_create", "map_read", "map_write", "prog_load", "prog_run"} },
	{ "xdp_socket",
	  { COMMON_SOCK_PERMS, NULL } },
	{ "perf_event",
	  {"open", "cpu", "kernel", "tracepoint", "read", "write"} },
	{ NULL }
  };

#if PF_MAX > 45
#error New address family defined, please update secclass_map.
#endif
