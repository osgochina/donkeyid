#ifndef __SPINLOCK_H
#define __SPINLOCK_H

typedef volatile unsigned int atomic_t;

void spin_lock(atomic_t *lock, int which);
void spin_unlock(atomic_t *lock, int which);

#endif
