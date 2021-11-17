/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef MSS_MULTIZONE_H
#define MSS_MULTIZONE_H


#define MSIP     0x02000000ul
#define IPC_RECV 0x01000A00ul
#define IPC_SEND 0x01000A80ul

#define MZONE_SEND(zone, msg) ({ \
    uint64_t retVal = 0; \
    asm volatile ( \
        "   li t0, 0x1;                     "\
        "   amoswap.d.aq t0, t0, 0*8(%2);   "\
        "   bgtz t0, 2f;                    "\
        "   bnez t0, 1f;                    "\
        "   ld t0, 0*8+%1; sd t0, 1*8(%2);  "\
        "   ld t0, 1*8+%1; sd t0, 2*8(%2);  "\
        "   li %0, 1; slli t0, %0, 63;      "\
        "1: amoswap.d.rl t0, t0, 0*8(%2);   "\
        "   beqz %0, 2f; sw %0, (%3);       "\
        "2:                                 "\
        : "+r"(retVal) : "m"(msg), "r"( IPC_RECV + (zone-1)*(8*3) ), "r"(MSIP) : "t0", "memory" \
    ); \
    retVal; \
})

#define MZONE_RECV(zone, msg) ({ \
    uint64_t retVal = 0; \
    asm volatile ( \
        "   li t0, 0x1;                     "\
        "   amoswap.d.aq t0, t0, 0*8(%2);   "\
        "   bgtz t0, 2f;                    "\
        "   beqz t0, 1f;                    "\
        "   sd zero, 0*8(%2);               "\
        "   ld t0, 1*8(%2); sd t0, 0*8+%1;  "\
        "   ld t0, 2*8(%2); sd t0, 1*8+%1;  "\
        "   li %0, 1; li t0, 0;             "\
        "1: amoswap.d.rl t0, t0, 0*8(%2);   "\
        "2:                                 "\
        : "+r"(retVal), "+m"(msg) : "r"( IPC_SEND + (zone-1)*(8*3) ) : "t0", "memory" \
    ); \
    retVal; \
})


#endif /* MSS_MULTIZONE_H */
