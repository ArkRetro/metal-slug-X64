/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da590.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_da590(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar3;

  candidate_1abc8();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    candidate_1da40();
    *(short *)(extraout_A0 + 0x28) = *(short *)(extraout_A0 + 0x28) + 0x20;
    *(short *)(extraout_A0 + 0x2c) = *(short *)(extraout_A0 + 0x2c) + 0x30;
    candidate_1da40();
    *(short *)(extraout_A0_00 + 0x28) = *(short *)(extraout_A0_00 + 0x28) + 0x20;
    *(short *)(extraout_A0_00 + 0x2c) = *(short *)(extraout_A0_00 + 0x2c) + 0x30;
    *unaff_A6 = candidate_da5fa;
  }
  bVar3 = false;
  candidate_da488();
  if (bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
