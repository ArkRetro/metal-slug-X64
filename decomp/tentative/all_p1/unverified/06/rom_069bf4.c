/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069bf4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69bf4(void)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_69cfc();
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar2;
  *unaff_A6 = 0x69c14;
  sVar1 = *(short *)((int)unaff_A6 + 0x76) + -1;
  *(short *)((int)unaff_A6 + 0x76) = sVar1;
  if (sVar1 < 0) {
    candidate_69cfc();
    *(undefined2 *)((int)unaff_A6 + 0x76) = 8;
    *unaff_A6 = 0x69c40;
    sVar1 = *(short *)((int)unaff_A6 + 0x76) + -1;
    *(short *)((int)unaff_A6 + 0x76) = sVar1;
    if (sVar1 < 0) {
      candidate_69cfc();
      *(undefined2 *)((int)unaff_A6 + 0x76) = 8;
      *unaff_A6 = 0x69c6c;
      sVar1 = *(short *)((int)unaff_A6 + 0x76) + -1;
      *(short *)((int)unaff_A6 + 0x76) = sVar1;
      if (sVar1 < 0) {
        uVar2 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x1f) = uVar2;
        candidate_69cfc();
                    /* WARNING: Could not recover jumptable at 0x000699d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*(code *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) + 0x699d4))();
        return;
      }
      candidate_69e08();
      candidate_1436a();
      candidate_69d18();
    }
    else {
      candidate_1436a();
      candidate_69d18();
    }
  }
  else {
    candidate_1436a();
    candidate_69d4a();
  }
  bVar3 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar3) {
    bVar3 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar3;
  uVar4 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar4 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
