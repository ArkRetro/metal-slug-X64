/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05aca2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5aca2(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
  if (*(short *)((int)unaff_A6 + 0x72) != 0) {
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)((int)unaff_A6 + 0x72) = 0;
  }
  candidate_4256();
  *unaff_A6 = 0x5ace6;
  uVar1 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5ab90;
  }
  candidate_5b714();
  return;
}
