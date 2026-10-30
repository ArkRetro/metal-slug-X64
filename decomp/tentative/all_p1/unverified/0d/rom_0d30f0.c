/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d30f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d30f0(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  *unaff_A6 = 0xd3112;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d280a;
    uVar1 = 0;
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
