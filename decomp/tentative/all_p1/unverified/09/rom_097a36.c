/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097a36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_97a36(void)

{
  undefined4 uVar1;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  candidate_4256();
  candidate_9ea8();
  uVar1 = candidate_2e7c();
  unaff_A6[0xc] = uVar1;
  unaff_A6[0xd] = 0;
  *unaff_A6 = candidate_97a86;
  return;
}
