/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f720.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f720(void)

{
  uint uVar1;
  undefined4 *unaff_A6;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  candidate_4256();
  uVar1 = candidate_29c6();
  if ((uVar1 & 1) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0xc] = ((int)unaff_A6[0xc] >> 1) + -0xa0000;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = 0xffffa000;
  *unaff_A6 = candidate_4f794;
  return;
}
