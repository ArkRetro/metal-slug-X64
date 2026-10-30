/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b8edc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b8edc(void)

{
  code *pcVar1;
  int iVar2;
  short unaff_D4w;
  int extraout_A0;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *unaff_A6 = 0xb8ee8;
  if (*(short *)(*(short *)((int)unaff_A6 + 0x6e) + 0xb8ec2) == -1) {
    candidate_4126();
    iVar2 = _DAT_001063dc;
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_c6f6a();
  if (-1 < (short)(*(short *)(extraout_A0 + 0x36) * 0x10 + 0x1f0 + unaff_D4w * -0x10)) {
    (**(code **)(*(short *)((int)unaff_A6 + 0x6e) + 0xb8ec4))();
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 6;
  }
  return;
}
