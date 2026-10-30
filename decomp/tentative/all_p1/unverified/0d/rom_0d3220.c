/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3220.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3220(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined4 *)(unaff_A6 + 0x7e) = *(undefined4 *)(iVar1 + 0x7e);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x7a) + *(short *)(iVar1 + 0x7a);
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x7c) + *(short *)(iVar1 + 0x7c);
  *(undefined1 *)(unaff_A6 + 0x71) = *(undefined1 *)(iVar1 + 0x71);
  candidate_1abc8();
  return;
}
