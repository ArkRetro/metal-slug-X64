/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a61e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a61e8(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7f) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0;
  unaff_A6[0x14] = 0x2a5ff2;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  *(undefined2 *)((int)unaff_A6 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x4e);
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  candidate_a64b8();
  *unaff_A6 = candidate_a6260;
  return;
}
