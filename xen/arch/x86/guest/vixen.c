/******************************************************************************
 * arch/x86/guest/vixen.c
 *
 * Support for detecting and running under Xen HVM.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates.
 */

#include <asm/guest/vixen.h>

static int in_vixen;
static int vixen_domid = 1;
static uint32_t vixen_reserved_mem_pgstart = 0xfeff0000;

integer_param("vixen_domid", vixen_domid);

bool is_vixen(void)
{
    return in_vixen > 0;
}

int vixen_get_domid(void)
{
    return vixen_domid;
}

void vixen_get_reserved_mem(unsigned long *start_pfn, unsigned long *end_pfn)
{
    *start_pfn = vixen_reserved_mem_pgstart >> XEN_PAGE_SHIFT;

    /* This is part of the Xen ABI */
    *end_pfn   = 0x100000;
}
