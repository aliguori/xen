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
#include <public/version.h>
#include <public/hvm/hvm_info_table.h>

#define X86_HVM_END_SPECIAL_REGION  0xff000u

#define SHARED_INFO_PFN		(X86_HVM_END_SPECIAL_REGION + 0)

static int in_vixen;
static int vixen_domid = 1;
static uint32_t vixen_reserved_mem_pgstart = 0xfeff0000;
static shared_info_t *global_si;
static bool vixen_ptver;

integer_param("vixen_domid", vixen_domid);
boolean_param("vixen_ptver", vixen_ptver);

void __init init_vixen(void)
{
    int major, minor, version;
    struct hvm_info_table *hvm_info;

    if ( !xen_guest )
    {
        printk("Disabling Vixen because we are not running under Xen\n");
        in_vixen = -1;
        return;
    }

    if ( vixen_domid < 0 )
    {
        printk("Disabling Vixen due to user request\n");
        in_vixen = -1;
        return;
    }

    version = HYPERVISOR_xen_version(XENVER_version, NULL);
    major = version >> 16;
    minor = version & 0xffff;

    printk("Vixen running under Xen %d.%d\n", major, minor);

    hvm_info = maddr_to_virt(HVM_INFO_PADDR);
    if ( strncmp(hvm_info->signature, "HVM INFO", 8) == 0 &&
	 hvm_info->length >= sizeof(struct hvm_info_table) &&
	 hvm_info->length < (PAGE_SIZE - HVM_INFO_OFFSET) )
    {
	uint8_t sum;
	uint32_t i;

	for ( i = 0, sum = 0; i < hvm_info->length; i++ )
	    sum += ((uint8_t *)hvm_info)[i];

	if ( sum == 0 )
	{
	    vixen_reserved_mem_pgstart = hvm_info->reserved_mem_pgstart << XEN_PAGE_SHIFT;
	}
    }

    in_vixen = 1;
}

void __init early_vixen_init(void)
{
    struct xen_add_to_physmap xatp;
    long rc;

    if ( !is_vixen() )
	return;

    global_si = mfn_to_virt(SHARED_INFO_PFN);

    /* Setup our own shared info area */
    xatp.domid = DOMID_SELF;
    xatp.idx = 0;
    xatp.space = XENMAPSPACE_shared_info;
    xatp.gpfn = virt_to_mfn(global_si);

    rc = HYPERVISOR_memory_op(XENMEM_add_to_physmap, &xatp);
    if ( rc < 0 )
        printk("Setting shared info page failed: %ld\n", rc);

    memset(&global_si->native.evtchn_mask[0], 0x00,
           sizeof(global_si->native.evtchn_mask));
}

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

u64 vixen_get_cpu_freq(void)
{
    volatile vcpu_time_info_t *timep = &global_si->native.vcpu_info[0].time;
    vcpu_time_info_t time;
    uint32_t version;
    u64 imm;

    do {
	version = timep->version;
	rmb();
	time = *timep;
    } while ((version & 1) || version != time.version);

    imm = (1000000000ULL << 32) / time.tsc_to_system_mul;

    if (time.tsc_shift < 0) {
	return imm << -time.tsc_shift;
    } else {
	return imm >> time.tsc_shift;
    }
}

bool vixen_passthru_version(void)
{
    return is_vixen() && vixen_ptver;
}
