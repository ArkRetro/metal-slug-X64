/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08889c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8889c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined2 *)(unaff_A6 + 0x21) = 10;
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  *unaff_A6 = 0x888c8;
  uVar2 = 0;
  candidate_4c24();
  candidate_146a8();
  candidate_427a();
  if (((bool)uVar2) &&
     (sVar1 = *(short *)(unaff_A6 + 0x21) + -1, *(short *)(unaff_A6 + 0x21) = sVar1, sVar1 == 0)) {
    *unaff_A6 = candidate_8890e;
  }
  if (unaff_A6[0x28] != 0) {
    candidate_140c0();
  }
  candidate_89152();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
