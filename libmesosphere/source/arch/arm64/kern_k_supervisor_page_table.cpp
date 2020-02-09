/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <mesosphere.hpp>

namespace ams::kern::arm64 {

    void KSupervisorPageTable::Initialize(s32 core_id) {
        /* Get the identity mapping ttbr0. */
        this->ttbr0[core_id] = cpu::GetTtbr0El1();

        /* Set sctlr_el1 */
        MESOSPHERE_TODO("Set bit in SCTLR_EL1");
        cpu::EnsureInstructionConsistency();

        /* Invalidate the entire TLB. */
        cpu::InvalidateEntireTlb();
        cpu::EnsureInstructionConsistency();

        /* If core 0, initialize our base page table. */
        if (core_id == 0) {
            /* TODO: constexpr defines. */
            const u64 ttbr1 = cpu::GetTtbr1El1() & 0xFFFFFFFFFFFFul;
            const u64 kernel_vaddr_start = 0xFFFFFF8000000000ul;
            const u64 kernel_vaddr_end   = 0xFFFFFFFFFFE00000ul;
            void *table = GetVoidPointer(KPageTableBase::GetLinearVirtualAddress(ttbr1));
            this->page_table.InitializeForKernel(table, kernel_vaddr_start, kernel_vaddr_end);
        }
    }

    void KSupervisorPageTable::Finalize(s32 core_id) {
        MESOSPHERE_TODO_IMPLEMENT();
    }
}
