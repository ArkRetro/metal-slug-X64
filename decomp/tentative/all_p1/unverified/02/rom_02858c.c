/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02858c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2858c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_4256();
  candidate_19dfc();
  candidate_1b1dc();
  *unaff_A6 = 0x285b4;
  uVar1 = 0;
  candidate_1436a();
  candidate_427a();
  if (((bool)uVar1) && ((*(byte *)((int)unaff_A6 + 0x7d) & 1) == 0)) {
    *unaff_A6 = candidate_285ea;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
