/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08890e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8890e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_146a8();
  candidate_88978();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + 1;
  *unaff_A6 = 0x8893a;
  uVar1 = 0;
  candidate_4c24();
  candidate_146a8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_88a30;
  }
  if (unaff_A6[0x28] != 0) {
    candidate_140c0();
  }
  candidate_89152();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
