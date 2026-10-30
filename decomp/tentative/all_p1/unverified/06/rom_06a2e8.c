/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06a2e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6a2e8(void)

{
  char cVar1;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  *(undefined1 *)((int)unaff_A6 + 0x95) = 0;
  bVar2 = false;
  candidate_6bace();
  if (bVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *unaff_A6 = 0x6ad62;
      uVar3 = 0;
      candidate_6b7e0();
      candidate_427a();
      if ((bool)uVar3) {
        *unaff_A6 = 0x6a2c6;
      }
      goto LAB_0006b222;
    }
  }
  else {
    candidate_6baf8();
    if (bVar2) {
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *unaff_A6 = 0x6ad94;
      uVar3 = 0;
      candidate_6b7e0();
      candidate_427a();
      if ((bool)uVar3) {
        *unaff_A6 = 0x6a2c6;
      }
      goto LAB_0006b222;
    }
    if ((*(byte *)((int)unaff_A6 + 0x92) & 1) != 0) {
LAB_0006bb88:
                    /* WARNING: Could not recover jumptable at 0x0006bb88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)();
      return;
    }
    if (((*(byte *)((int)unaff_A6 + 0x92) & 0x20) != 0) &&
       ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0)) {
      if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) != 0) {
                    /* WARNING: Could not recover jumptable at 0x0006a2d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*(code *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x95) << 2) + 0x6a2d4))();
        return;
      }
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *unaff_A6 = candidate_6aa14;
      uVar3 = 0;
      candidate_6b7e0();
      candidate_427a();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_6aa2c;
      }
      goto LAB_0006b222;
    }
    if (((*(byte *)((int)unaff_A6 + 0x92) & 0x20) != 0) ||
       ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0)) goto LAB_0006bb88;
  }
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = candidate_6acb4;
  uVar3 = 0;
  candidate_6b7e0();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = 0x6a2c6;
  }
LAB_0006b222:
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_6accc;
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  if (*(char *)(unaff_A6 + 0x24) != '\0') {
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
  }
  uVar3 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    uVar3 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_6af44;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
