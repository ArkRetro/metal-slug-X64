/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0787ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_787ae(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x9b) = *(undefined1 *)(unaff_A6[3] + 0x9b);
  if ((*(byte *)(unaff_A6 + 0x27) & 8) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  }
  uVar1 = 0;
  candidate_1abc8();
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
