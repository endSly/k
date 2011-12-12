
#ifndef __MULTITASK_H__
#define __MULTITASK_H__

#include "arch/paging.h"

page_directory* clone_directory(page_directory *src);

#endif // __MULTITASK_H__
