/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c152.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c152(void)

{
  short in_D0w;
  short sVar1;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(*(int *)(unaff_A6 + 0xc) + 0x1e);
  if (*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x7f) < 0x10) {
    sVar1 = in_D0w + 1;
  }
  else {
    sVar1 = in_D0w + -1;
  }
  *(short *)(unaff_A6 + 0x1e) = sVar1;
  return;
}
