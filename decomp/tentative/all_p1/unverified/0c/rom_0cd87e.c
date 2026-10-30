/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd87e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd87e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x7fe;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x7fe;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xcd8b8;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6[3] + 0x6e);
  *(undefined1 *)((int)unaff_A6 + 0x7f) = *(undefined1 *)(unaff_A6[3] + 0x7f);
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if (((bool)uVar1) && ((*(byte *)((int)unaff_A6 + 0x7f) & 1) != 0)) {
    uVar1 = 0;
    candidate_4256();
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
