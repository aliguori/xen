/******************************************************************************
 * include/asm-x86/guest.h
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms and conditions of the GNU General Public
 * License, version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2018 Amazon.com, Inc. or its affiliates.
 */

#ifndef __ARM_GUEST_VIXEN_H__
#define __ARM_GUEST_VIXEN_H__

#include <xen/types.h>

static inline int
HYPERVISOR_xen_version(int cmd, void *arg)
{
    return -ENOSYS;
}

static inline unsigned long
HYPERVISOR_hvm_op(int op, void *arg)
{
    return -ENOSYS;
}

static inline int
HYPERVISOR_grant_table_op(unsigned int cmd, void *uop, unsigned int count)
{
    return -ENOSYS;
}

static inline long
HYPERVISOR_memory_op(unsigned int cmd, void *arg)
{
    return -ENOSYS;
}

static inline int
HYPERVISOR_event_channel_op(int cmd, void *arg)
{
    return -ENOSYS;
}

static inline int
HYPERVISOR_sched_op(int cmd, void *arg)
{
    return -ENOSYS;
}

static inline int
HYPERVISOR_vcpu_op(int cmd, int vcpuid, void *extra_args)
{
    return -ENOSYS;
}

static inline bool is_vixen(void)
{
    return false;
}

static inline bool vixen_has_per_cpu_notifications(void)
{
    return false;
}

static inline bool vixen_ring_process(uint16_t port)
{
    return false;
}

static inline int vixen_get_domid(void)
{
    return 0;
}

static bool vixen_passthru_version(void)
{
    return false;
}

long vixen_cpu_up(void *data)
{
    return -ENOSYS;
}

long vixen_cpu_down(void *data)
{
    return -ENOSYS;
}

#endif
