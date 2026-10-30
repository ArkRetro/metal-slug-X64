/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097f52.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Instruction at (ram,0x00097fa9) overlaps instruction at (ram,0x00097fa8)
    */
/* WARNING: Control flow encountered bad instruction data */

short candidate_97f52(void)

{
  short in_D0w;
  ushort uVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  short unaff_D4w;
  undefined4 *extraout_A0;
  int extraout_A1;
  undefined4 *unaff_A6;
  char cVar5;
  bool bVar6;
  bool bVar7;

  if (*(ushort *)(unaff_A6 + 0x23) < 0x2e) {
LAB_00097f70:
    if (2 < *(ushort *)(unaff_A6 + 0x23)) {
      sVar3 = *(short *)((short)((*(short *)(unaff_A6 + 0x23) + -3) * 2) + 0x97e88);
      if (sVar3 == 0) {
        return sVar3;
      }
      candidate_9f88();
      in_D0w = candidate_9ea8();
    }
    *(short *)(unaff_A6 + 0x23) = *(short *)(unaff_A6 + 0x23) + 1;
    return in_D0w;
  }
  __m68k_trap(0xf);
  sVar3 = *(short *)(&stack0x00000065 + unaff_D4w * 4);
  bVar7 = SBORROW2(sVar3,7);
  sVar3 = sVar3 + -7;
  *(short *)(&stack0x00000065 + unaff_D4w * 4) = sVar3;
  cVar5 = sVar3 < 0;
  bVar6 = sVar3 == 0;
  in_D0w = candidate_97fd8();
  if (!bVar7) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (bVar6 || bVar7 != (bool)cVar5) {
    *extraout_A0 = uRam000008ee;
  }
  else if (!bVar6 && bVar7 == (bool)cVar5) {
    *(short *)(extraout_A1 + 0x6d65) = *(short *)(extraout_A1 + 0x6d65) + 2;
    goto LAB_00097f70;
  }
  candidate_985a0();
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  candidate_9849c();
  do {
    if ((*(short *)((int)unaff_A6 + 0x8a) != 0) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
      *unaff_A6 = 0x98114;
      if ((*(byte *)(unaff_A6 + 0x23) & 8) != 0) goto LAB_0009816e;
      candidate_98750();
      candidate_9861e();
      if ((*(char *)((int)unaff_A6 + 0x8f) + 1U & 0xf) < 3) {
        *unaff_A6 = candidate_98142;
      }
      goto LAB_0009842c;
    }
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)((int)unaff_A6 + 0x82);
    *unaff_A6 = 0x9805a;
    if (((*(char *)((int)unaff_A6 + 0x8d) != '\0') && (*(int *)((int)unaff_A6 + 0x6e) != 0)) &&
       (sVar3 = *(short *)(unaff_A6 + 0x20) + -1, *(short *)(unaff_A6 + 0x20) = sVar3, sVar3 < 0)) {
      *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 0x10;
    }
    if ((*(byte *)(unaff_A6 + 0x23) & 0x10) == 0) {
      if ((*(byte *)(unaff_A6 + 0x23) & 8) != 0) goto LAB_0009816e;
      candidate_9865c();
      candidate_98704();
      candidate_9861e();
      goto LAB_0009842c;
    }
    bVar6 = false;
    candidate_40bc();
    *(char *)(unaff_A6 + 0x18) = -!bVar6;
    *(char *)((int)unaff_A6 + 0x8d) = -!bVar6;
    *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1;
    *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) & 0xef;
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x980f0;
  } while (*(char *)(unaff_A6 + 0x18) != '\0');
  if ((*(byte *)(unaff_A6 + 0x23) & 8) == 0) {
    candidate_98750();
    candidate_9861e();
  }
  else {
LAB_0009816e:
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    sVar3 = -0x800;
    sVar4 = 0x800;
    if (0x9f < *(short *)(unaff_A6 + 0xf)) {
      sVar3 = 0x800;
      sVar4 = -0x800;
    }
    unaff_A6[0xc] = sVar3 * 0x100 + unaff_A6[0xc];
    unaff_A6[0xd] = unaff_A6[0xd] + -0x40000;
    uVar1 = candidate_29c6();
    uVar2 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x72) = (uVar1 & 0x1ff) + sVar4;
    *(ushort *)(unaff_A6 + 0x1d) = (uVar2 & 0x1ff) + 0x200;
    *unaff_A6 = candidate_981ec;
    candidate_98770();
    candidate_9861e();
  }
LAB_0009842c:
  if ((*(short *)(unaff_A6 + 0x21) != 0) &&
     (sVar3 = *(short *)(unaff_A6 + 0x21) + -1, *(short *)(unaff_A6 + 0x21) = sVar3, sVar3 == 0)) {
    *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 8;
  }
  bVar6 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar5 = *(char *)((int)unaff_A6 + 0x91);
    bVar6 = cVar5 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar5 + -1;
  }
  candidate_4c24();
  if (bVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  uVar1 = *(ushort *)((int)unaff_A6 + 0x4e);
  uVar2 = *(ushort *)(unaff_A6 + 0x1e);
  bVar6 = uVar1 >= uVar2;
  if (!bVar6 || uVar1 == uVar2) {
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  bVar7 = (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0;
  if (!bVar7) {
    *unaff_A6 = candidate_981f8;
  }
  bVar7 = bVar7 && ((bVar6 && uVar1 != uVar2) && uVar1 < uVar2);
  sVar3 = candidate_1945c();
  if (bVar7) {
    *unaff_A6 = 0x9841e;
  }
  return sVar3;
}
