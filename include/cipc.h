#ifndef __CIPC_H__
#define __CIPC_H__

#include <limits.h>
#include <inttypes.h>
#include <sys/types.h>

#ifndef PACKED 
#define PACKED __attribute__((packed))
#endif

struct cipc_context_st_; /* Opaque types */
typedef struct cipc_context_st_* cipc_handle_t;

typedef struct cipc_msg_st_ cipc_msg_st;

typedef enum cipc_method_en_ {
    CIPC_METHOD_UNKNOWN = 0,
    CIPC_METHOD_UNIX_RELIABLE,
    CIPC_METHOD_UNIX_UNRELIABLE,
    CIPC_METHOD_UNIX_SEQPACK,
    CIPC_METHOD_INET_RELIABLE,
    CIPC_METHOD_INET_UNRELIABLE,
    CIPC_METHOD_INVALID
} cipc_method_en;

#define CIPC_METHOD_IS_VALID(__arg__) \
(((__arg__) > CIPC_METHOD_UNKNOWN) && ((__arg__) < CIPC_METHOD_INVALID))

typedef enum cipc_scope_en_ {
    CIPC_SCOPE_UNKNOWN = 0,
    CIPC_SCOPE_LOCAL,
    CIPC_SCOPE_DISTRIB,
    CIPC_SCOPE_INVALID
} cipc_scope_en;

#define CIPC_SCOPE_IS_VALID(__arg__) \
(((__arg__) > CIPC_SCOPE_UNKNOWN) && ((__arg__) < CIPC_SCOPE_INVALID))


#define CIPC_CONFIG_IS_COMPATIBLE_WITH_SCOPE(__config__, __scope__) \
(((((__config__) == CIPC_METHOD_UNIX_RELIABLE) ||   \
((__config__) == CIPC_METHOD_UNIX_UNRELIABLE) ||    \
((__config__) == CIPC_METHOD_UNIX_SEQPACK)) &&      \
((__scope__) == CIPC_SCOPE_LOCAL)) ||               \
(((__scope__) == CIPC_METHOD_INET_RELIABLE)         \
((__scope__) == CIPC_METHOD_INET_UNRELIABLE)))      

typedef void (*cipc_error_cb) (cipc_msg_st *msg);
typedef void (*cipc_receive_cb) (cipc_msg_st *msg);

typedef struct cipc_in_args_st_ {
    cipc_error_cb       on_error;
    cipc_receive_cb     on_receive;
    cipc_method_en      ipc_method;
    cipc_scope_en       ipc_scope;
    union {
        char path[PATH_MAX];
        struct {
            char hostname[HOST_NAME_MAX];
            int  port;
        } ip;
    } mdata;
    size_t              max_buffer_size; /* Size of buffer in bytes */
} cipc_in_args_st;

struct cipc_msg_st_ {
    uint64_t    id;
    uint32_t    type;
    uint32_t    count;
    size_t      length;
    uint8_t    *value;
} PACKED;

int
cipc_create_handle(cipc_in_args_st *args,
                   cipc_handle_t   *ch);
#endif //__CIPC_H__