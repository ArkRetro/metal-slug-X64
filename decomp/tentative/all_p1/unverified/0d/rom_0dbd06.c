/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dbd06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dbd06(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  unaff_A6[0x14] = 0;
  candidate_dbbf4();
  candidate_1da40();
  *(short *)(extraout_A0 + 0x28) = *(short *)(extraout_A0 + 0x28) + -0x20;
  *(short *)(extraout_A0 + 0x2c) = *(short *)(extraout_A0 + 0x2c) + 0x30;
  *unaff_A6 = 0xdbecc;
  bVar3 = false;
  candidate_408c();
  if (!bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  return;
}
