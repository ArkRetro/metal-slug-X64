/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c3778.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c3778(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;
  undefined2 uStack_6;

  sVar1 = *(short *)(unaff_A6 + 0x72);
  sVar2 = *(short *)(unaff_A6 + 0x74);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar1 = -sVar1;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    sVar2 = -sVar2;
  }
  *(short *)(unaff_A6 + 0x3c) = sVar1 + *(short *)(unaff_A6 + 0x3c);
  *(short *)(unaff_A6 + 0x40) = sVar2 + *(short *)(unaff_A6 + 0x40);
  *(short *)(unaff_A6 + 0x44) = sVar1 + *(short *)(unaff_A6 + 0x44);
  *(short *)(unaff_A6 + 0x48) = sVar2 + *(short *)(unaff_A6 + 0x48);
  candidate_427a();
  *(short *)(unaff_A6 + 0x3c) = *(short *)(unaff_A6 + 0x3c) - sVar1;
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) - uStack_6;
  *(short *)(unaff_A6 + 0x44) = *(short *)(unaff_A6 + 0x44) - sVar1;
  *(short *)(unaff_A6 + 0x48) = *(short *)(unaff_A6 + 0x48) - uStack_6;
  return;
}
