
struct k_arch_gdt_entry {
    unsigned char b[8];
};

struct k_arch_ldt_entry {
    unsigned char b[8];
};

struct k_arch_tss {
    unsigned char b[114];
};

extern struct k_arch_gdt_entry k_arch_gdt[8192];
extern struct k_arch_ldt_entry k_arch_ldt[8192];

void karch_init(void);

void karch_set_gdt_entry(unsigned int index, unsigned int base, unsigned int limit, unsigned char type);
