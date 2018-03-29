#include "bpf_api.h"
#include <linux/if_ether.h>
#include <linux/ip.h>

__section("bpf") int bpf_main(struct __sk_buff *skb)
{
	uint32_t src_ip;
	int nh_off = BPF_LL_OFF + ETH_HLEN;
	int ret = TC_ACT_OK;

	src_ip = load_word(skb, nh_off+offsetof(struct iphdr, saddr));
	if(src_ip == ((172 << 24) | (17 << 16) | (0 << 8) | 3))
		ret = TC_ACT_SHOT;

	return ret;
}

char license[] __section("license") = "GPL";

