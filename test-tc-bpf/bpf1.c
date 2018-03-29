__section("bpf") int bpf_main(struct __sk_buff *skb)
{
	src_ip = load_word(skb, nh_off+offsetof(struct iphdr, saddr));
	if(src_ip == ((172 << 24) | (17 << 16) | (0 << 8) | 3))
		return TC_ACT_SHOT;

	return TC_ACT_OK;
}

