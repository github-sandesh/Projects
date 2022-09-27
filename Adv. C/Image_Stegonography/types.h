// define it if not defined.
#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;
// operation type will be used as return type.
typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
