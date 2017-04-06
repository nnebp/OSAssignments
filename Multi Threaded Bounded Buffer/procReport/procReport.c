#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

void getProcessStatuses(long *unrunnable, long *runnable, long *stopped) {
    struct task_struct *task = NULL;

    for_each_process(task) {
        switch (task->state) {
            case(-1): //unrunnabe 
                ++*unrunnable;
                break;
            case(0): //runnable 
                ++*runnable;
                break;
            default: // >0 stopped
                ++*stopped;
        }
    }

}

int numChildren(struct list_head *head) {
    struct list_head *ptr;
    int counter = 0;

    list_for_each(ptr, head) {
        counter++;
    }

    return counter;
}

static int proc_report_show(struct seq_file *m, void *v) {
    struct task_struct *task = NULL;
    struct task_struct *child = NULL;
    struct list_head *head = NULL;


    //TODO in the future we need to see if we are sending these outside of the function
    long unrunnable = 0, runnable = 0, stopped = 0;
    getProcessStatuses(&unrunnable, &runnable, &stopped);
    seq_printf(m, "PROCESS REPORTER\nUnrunnable:%ld \nRunnable:%ld \nStopped:%ld\n", unrunnable, runnable, stopped);


    for_each_process(task) {
        seq_printf(m, "Process ID:%d Name=%s", task->pid, task->comm);
        
        //grab the first child process
        head = &task->children;
        head = head->next;
        child = list_entry(head, struct task_struct, sibling);

        //print children info
        if (numChildren(&task->children) > 0) {
            seq_printf(m, " number_of_children=%d", numChildren(&task->children));
            seq_printf(m, " first_child_pid=%d first_child_name=%s\n", child->pid, child->comm);
        } else {
            seq_printf(m," *No Children\n");
        }

    }
    return 0;
}

static int proc_report_open(struct inode *inode, struct  file *file) {
      return single_open(file, proc_report_show, NULL);
}

static const struct file_operations proc_report_fops = {
    .owner = THIS_MODULE,
    .open = proc_report_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};



int proc_init (void) {
    //init proc file
    proc_create("proc_report", 0, NULL, &proc_report_fops);
    printk(KERN_INFO "procReport: init module\n");

    return 0;
}

void proc_cleanup(void) {
    printk(KERN_INFO "proc_report: performing cleanup of module endddddd\n");
    remove_proc_entry("proc_report", NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);
