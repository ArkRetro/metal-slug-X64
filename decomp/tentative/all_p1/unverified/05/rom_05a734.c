/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a734.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a734(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  iVar1 = *(short *)((int)unaff_A6 + 0x6e) * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = *(short *)((int)unaff_A6 + 0x6e) * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  candidate_4256();
  *unaff_A6 = 0x5a762;
  uVar2 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = candidate_5a7d0;
      uVar2 = 0;
    }
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar2) {
    candidate_5a894();
    uVar2 = *(short *)((int)unaff_A6 + 0x72) != -1;
    if (*(short *)((int)unaff_A6 + 0x72) == -1) {
      candidate_406b6();
    }
    else {
      *unaff_A6 = candidate_9b596;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
