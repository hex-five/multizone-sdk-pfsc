/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef MSS_MULTIZONE_H
#define MSS_MULTIZONE_H


#define CLINT_MSIP  0x02000000ul
#define IPC_BASE    0x01000A00ul

#define MZONE_SEND(zone, msg) ({ \
    uint64_t retVal = 0; \
    asm volatile ( \
        "csrr t0, mhartid;                  "\
        "addi t0, t0, -1;                   "\
        "slli t0, t0, 8;                    "\
        "add %2, %2, t0;                    "\
        "ld t0, (%2); bnez t0, 1f;          "\
        "   ld t0, 0*8+%1; sd t0, 1*8(%2);  "\
        "   ld t0, 1*8+%1; sd t0, 2*8(%2);  "\
        "   li %0, 1;      sd %0, 0*8(%2);  "\
        "   sw %0, (%3);                    "\
        "1:                                 "\
        : "+r"(retVal) : "m"(msg), "r"( IPC_BASE + (zone-1)*(8*3) + 0x00), "r"(CLINT_MSIP) : "t0", "memory" \
    ); \
    retVal; \
})

#define MZONE_RECV(zone, msg) ({ \
    uint64_t retVal = 0; \
    asm volatile ( \
        "csrr t0, mhartid;                  "\
        "addi t0, t0, -1;                   "\
        "slli t0, t0, 8;                    "\
        "add %2, %2, t0;                    "\
        "ld t0, (%2); beqz t0, 1f;          "\
        "   ld t0, 1*8(%2); sd t0, 0*8+%1;  "\
        "   ld t0, 2*8(%2); sd t0, 1*8+%1;  "\
        "   sd zero, (%2);                  "\
        "   li %0, 1;                       "\
        "1:                                 "\
        : "+r"(retVal), "+m"(msg) : "r"( IPC_BASE + (zone-1)*(8*3) + 0x80) : "t0", "memory" \
    ); \
    retVal; \
})


#endif /* MSS_MULTIZONE_H */
