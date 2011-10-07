
struct k_arch_gdt_entry {
    unsigned char b[8];
};

struct k_arch_ldt_entry {
    unsigned char b[8];
};

extern struct k_arch_gdt_entry k_arch_gdt[8192];
extern struct k_arch_ldt_entry k_arch_ldt[8192];

void karch_init(void);
