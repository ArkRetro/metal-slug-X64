/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01da7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1da7c(void)

{
  int iVar1;
  short sVar2;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;

  sVar2 = candidate_29f2();
  sVar2 = *(short *)(*(int *)(unaff_A6 + 0x86) + 4) + sVar2;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar2 = -sVar2;
  }
  *(short *)(unaff_A6 + 0x30) = sVar2;
  sVar2 = candidate_29f2();
  sVar2 = *(short *)(*(int *)(unaff_A6 + 0x86) + 8) + sVar2;
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    sVar2 = -sVar2;
  }
  *(short *)(unaff_A6 + 0x34) = sVar2;
  if (*(char *)(*(int *)(unaff_A6 + 0x86) + 0xd) != '\0') {
    candidate_29c6();
  }
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x1e) = *(undefined2 *)(unaff_A6 + 0x1e);
  if ((*(byte *)(unaff_A6 + 0x15) & 4) != 0) {
    *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) | 4;
  }
  candidate_1acf2();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(extraout_A0_00 + 0x12) = *(undefined1 *)(iVar1 + 0x12);
  *(undefined1 *)(extraout_A0_00 + 0x13) = *(undefined1 *)(iVar1 + 0x13);
  return;
}
