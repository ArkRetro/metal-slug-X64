/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a7b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a7b2(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6 + 0x1c);
  candidate_9a956();
  *unaff_A6 = 0x9a7c2;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x1c) < 1) {
    *unaff_A6 = candidate_9a5ee;
    uVar4 = 0;
  }
  else {
    (**(code **)((short)((*(ushort *)((int)unaff_A6 + 0x7a) & 3) * 4) + 0x9a8e0))();
    if (*(short *)(unaff_A6 + 0x1f) < 2) {
      *(undefined2 *)(unaff_A6 + 0x21) = 0;
      *(undefined2 *)((int)unaff_A6 + 0x86) = 4;
    }
    else {
      sVar1 = *(short *)(unaff_A6 + 0x20);
      if (sVar1 != 0) {
        sVar1 = candidate_29f2();
      }
      *(short *)(unaff_A6 + 0x21) = sVar1 - (*(short *)(unaff_A6 + 0x20) >> 1);
      sVar1 = *(short *)((int)unaff_A6 + 0x82);
      if (sVar1 != 0) {
        sVar1 = candidate_29f2();
      }
      *(short *)((int)unaff_A6 + 0x86) = sVar1 - (*(short *)((int)unaff_A6 + 0x82) >> 1);
    }
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + -1;
    if (*(short *)((int)unaff_A6 + 0x7e) == 0) {
      bVar3 = false;
      candidate_40bc();
      iVar2 = extraout_A0;
      if (bVar3) {
        candidate_1b220();
        iVar2 = extraout_A0_00;
      }
      *(short *)(iVar2 + 0x28) = *(short *)(unaff_A6 + 0x21) + *(short *)(iVar2 + 0x28);
      *(short *)(iVar2 + 0x2c) = *(short *)((int)unaff_A6 + 0x86) + *(short *)(iVar2 + 0x2c);
      *(undefined2 *)(iVar2 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
      *(undefined2 *)(iVar2 + 0x76) = *(undefined2 *)((int)unaff_A6 + 0x76);
      *(undefined2 *)(iVar2 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x1e);
      uVar4 = 0;
    }
    else if (*(short *)(unaff_A6 + 0x1e) == 0) {
      uVar4 = 0;
      candidate_8cd58();
    }
    else {
      uVar4 = 0;
      candidate_8cd58();
    }
    candidate_1abc8();
  }
  candidate_c8bec();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
