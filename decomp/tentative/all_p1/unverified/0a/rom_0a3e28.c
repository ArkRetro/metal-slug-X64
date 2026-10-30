/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3e28.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3e28(void)

{
  undefined2 uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = 0x10;
  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    uVar1 = 0xfff0;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  *(undefined2 *)(unaff_A6 + 0x28) = uVar1;
  *(undefined2 *)(unaff_A6 + 0x2c) = 0x18;
  candidate_1abc8();
  sVar2 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x1e);
  if (*(char *)(unaff_A6 + 0x94) == -1) {
    sVar2 = sVar2 + -1;
  }
  *(short *)(unaff_A6 + 0x1e) = sVar2;
  return;
}
