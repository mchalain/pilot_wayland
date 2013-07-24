#ifndef __PILOT_TYPES_H__
#define __PILOT_TYPES_H__

typedef int32_t pilot_coord_t;
typedef uint32_t pilot_length_t;
typedef uint32_t pilot_bitsfield_t;
typedef uint32_t pilot_color_t;
#define colorset	memset32;
typedef int16_t pilot_key_t;
typedef char pilot_bool_t;
#ifndef PTHREAD
typedef uint32_t pilot_mutex_t;
#define _pilot_mutex(mutex)	pilot_mutex_t mutex:1
#define mutex_init(mutex, ...) do {mutex = 0; }while(0)
#define mutex_lock(mutex)	!(mutex = (!(mutex))? 1 : 0)
#define mutex_unlock(mutex) do { mutex = 0; }while(0)
#define mutex_destroy(mutex) do {} while(0)

typedef uint32_t pilot_cond_t;
#define _pilot_cond(cond)	pilot_cond_t cond:1
#define cond_init(cond, ...) do { cond = 0; }while(0)
#define cond_wait(cond, mutex)	(cond = ((cond)? 0 : -1))
#define cond_signal(cond) do { cond = 1; }while(0)
#define cond_destroy(cond) do {} while(0)

#else
typedef pthread_mutex_t pilot_mutex_t;
#define _pilot_mutex(mutex)	pthread_mutex_t mutex
#define mutex_init(mutex, attr) pthread_mutex_init(&mutex, attr);
#define mutex_lock(mutex)	pthread_mutex_lock(&mutex)
#define mutex_unlock(mutex) pthread_mutex_unlock(&mutex)
#define mutex_destroy(mutex) pthread_mutex_destroy(&mutex)

typedef pthread_cond_t pilot_cond_t;
#define _pilot_cond(cond)	pilot_cond_t pthread_cond_t
#define cond_init(cond, ...) pthread_mutex_init(&cond, attr);
#define cond_wait(cond, mutex)	pthread_mutex_wait(&cond, &mutex)
#define cond_signal(cond) thread_mutex_signal(&cond)
#define cond_destroy(cond) pthread_mutex_destroy(&cond)

#endif
#endif