#include "arch/multitask.h"

#include "lib/lib.h"
#include "lib/memory.h"

volatile task *current_task;
volatile task *ready_queue; // The start of the task linked list.

uint32_t next_pid = 1;

// Defined in process.s
uint32_t read_eip();

void initialise_tasking()
{
    // Disable interrupts.
    __asm__ volatile("cli");

    // Relocate the stack so we know where it is.
    move_stack((void*)0xE0000000, 0x2000);

    // Initialise the first task (kernel task)
    current_task = ready_queue = (task*)kmalloc(sizeof(task));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_dir = current_directory;
    current_task->next = 0;

    // Reenable interrupts.
    __asm__ volatile("sti");
}

int fork()
{
    // Disable interrupts.
    __asm__ volatile("cli");

    // Take a pointer to this process' task struct for later reference.
    task *parent_task = (task*)current_task;

    // Clone the address space.
    page_directory *directory = clone_directory(current_directory);

    // Create a new process.
    task *new_task = (task*)kmalloc(sizeof(task));
    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_dir = directory;
    new_task->next = 0;

    // Add it to the end of the ready queue.
    task *tmp_task = (task*)ready_queue;
    while (tmp_task->next)
       tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    // This will be the entry point for the new process.
    uint32_t eip = read_eip();

    if (current_task != parent_task)
        // We are the child - by convention return 0.
        return 0;

    // We are the parent, so set up the esp/ebp/eip for our child.
    uint32_t esp, ebp;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));
    new_task->esp = esp;
    new_task->ebp = ebp;
    new_task->eip = eip;

    //Reenable interrupts.
    __asm__ volatile("sti");

    return new_task->id;
}

void switch_task()
{
    if (!current_task)
        return;

    // Read esp, ebp
    uint32_t esp, ebp, eip;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    if (eip == 0x12345) // Speciall value to know if we've switched tasks
        return;

    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;

    current_task = current_task->next;
    if (!current_task)
        current_task = ready_queue;

    esp = current_task->esp;
    ebp = current_task->ebp;

    // Here we:
    // * Stop interrupts so we don't get interrupted.
    // * Temporarily put the new EIP location in ECX.
    // * Load the stack and base pointers from the new task struct.
    // * Change page directory to the physical address (physicalAddr) of the new directory.
    // * Put a dummy value (0x12345) in EAX so that above we can recognise that we've just
    // switched task.
    // * Restart interrupts. The STI instruction has a delay - it doesn't take effect until after
    // the next instruction.
    // * Jump to the location in ECX (remember we put the new EIP in there).
    __asm__ volatile("                    \
                     cli;                 \
                     mov %0, %%ecx;       \
                     mov %1, %%esp;       \
                     mov %2, %%ebp;       \
                     mov %3, %%cr3;       \
                     mov $0x12345, %%eax; \
                     sti;                 \
                     jmp *%%ecx           "
        : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physical_addr));
}
