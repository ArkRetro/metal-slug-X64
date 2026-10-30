/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037404.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37404(void)

{
  byte bVar1;
  short sVar2;
  undefined1 extraout_D1b;
  undefined1 extraout_D1b_00;
  undefined4 *puVar3;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  bool bVar4;

  sVar2 = 1;
  bVar4 = false;
  puVar3 = (undefined4 *)((int)unaff_A6 + 0x76);
  while (!bVar4) {
    *puVar3 = 0;
    sVar2 = sVar2 + -1;
    puVar3 = puVar3 + 1;
    bVar4 = sVar2 == 0;
  }
  sVar2 = 1;
  bVar4 = false;
  puVar3 = (undefined4 *)((int)unaff_A6 + 0x7e);
  while (!bVar4) {
    *puVar3 = 0;
    sVar2 = sVar2 + -1;
    puVar3 = puVar3 + 1;
    bVar4 = sVar2 == 0;
  }
  candidate_40bc();
  *unaff_A6 = 0x3743e;
  bVar4 = false;
  candidate_373ea();
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x95) = extraout_D1b;
    bVar4 = false;
    candidate_40bc();
    if (bVar4) {
      bVar1 = *(byte *)((int)unaff_A6 + 0x95);
      *(byte *)(extraout_A0 + 0x95) = bVar1;
      *(int *)((int)unaff_A6 + (short)((bVar1 & 0xf) << 2) + 0x7e) = extraout_A0;
    }
  }
  bVar4 = false;
  candidate_373ea();
  if (bVar4) {
    *(undefined1 *)(unaff_A6 + 0x25) = extraout_D1b_00;
    bVar4 = false;
    candidate_40bc();
    if (bVar4) {
      bVar1 = *(byte *)(unaff_A6 + 0x25);
      *(byte *)(extraout_A0_00 + 0x94) = bVar1;
      *(int *)((int)unaff_A6 + (short)((bVar1 & 0xf) << 2) + 0x76) = extraout_A0_00;
    }
  }
  return;
}
