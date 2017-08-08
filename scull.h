#ifndef SCULL_MAJOR
#define SCULL_MAJOR     (0)
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS   (4)
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM   (4000)
#endif

#ifndef SCULL_QSET
#define SCULL_QSET      (1000)
#endif

struct scull_dev {
    int quantum;
    int qset;
    struct cdev cdev;
};
