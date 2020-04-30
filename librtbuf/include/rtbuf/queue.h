#ifndef QUEUE_H
#define QUEUE_H

/* static queue */

#define QUEUE_TYPE(type, size)  \
  type ## _queue_ ## size

#define QUEUE(type, size)          \
  struct QUEUE_TYPE(type, size) {  \
    unsigned int length;           \
    unsigned int read_index;       \
    unsigned int write_index;      \
    type item[size];               \
  }

#define S_QUEUE_TYPE(type, size)  \
  s_ ## QUEUE_TYPE(type, size)

#define QUEUE_METHOD(type, size, method)  \
  QUEUE_TYPE(type, size) ## _ ## method

#define DEFINE_QUEUE(type, size)                            \
  typedef QUEUE(type, size) S_QUEUE_TYPE(type, size);       \
  static void QUEUE_METHOD(type, size, init)                \
    (S_QUEUE_TYPE(type, size) *q)                           \
  {                                                         \
    bzero(q, sizeof(q));                                    \
  }                                                         \
  static void QUEUE_METHOD(type, size, free)                \
    (S_QUEUE_TYPE(type, size) *q)                           \
  {                                                         \
    bzero(q, sizeof(q));                                    \
  }                                                         \
  static int QUEUE_METHOD(type, size, enqueue)              \
    (S_QUEUE_TYPE(type, size) *q, type item)                \
  {                                                         \
    if (q->length < size) {                                 \
      unsigned int write_index = q->write_index;            \
      q->write_index = (q->write_index + 1) % size;         \
      q->item[write_index] = item;                          \
      q->length++;                                          \
      return 0;                                             \
    }                                                       \
    return -1;                                              \
  }                                                         \
  static int QUEUE_METHOD(type, size, dequeue)              \
    (S_QUEUE_TYPE(type, size) *q, type *ptr)                \
  {                                                         \
    if (q->length > 0) {                                    \
      unsigned int read_index = q->read_index;              \
      q->read_index = (read_index + 1) % size;              \
      *ptr = q->item[read_index];                           \
      q->length--;                                          \
      return 0;                                             \
    }                                                       \
    return -1;                                              \
  }

#endif
