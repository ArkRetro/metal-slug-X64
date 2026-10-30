/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07886e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7886e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A6[3] + 0x9a) = 0xff;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8830;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8830;
  candidate_4256();
  *unaff_A6 = 0x7889a;
  candidate_1abc8();
  candidate_427a();
  *(undefined2 *)(unaff_A6[3] + 0x84) = *(undefined2 *)(unaff_A6 + 0x21);
  uVar1 = 0;
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
    uVar1 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
