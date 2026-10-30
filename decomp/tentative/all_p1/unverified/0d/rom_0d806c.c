/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d806c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d806c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x76) = 0xf;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x22) = 0x88;
  *unaff_A6 = 0xd8082;
  if (((*(byte *)((int)unaff_A6 + 0x61) & 0x10) != 0) &&
     (*(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1,
     *(short *)((int)unaff_A6 + 0x76) < 1)) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *(undefined1 *)(unaff_A6 + 0x22) = 0x89;
    *unaff_A6 = 0xd80aa;
  }
  uVar1 = 0;
  candidate_1abc8();
  candidate_d90d2();
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
