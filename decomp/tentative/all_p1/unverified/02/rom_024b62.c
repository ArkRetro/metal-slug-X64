/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024b62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24b62(void)

{
  undefined4 uVar1;
  short sVar2;
  short sVar3;
  int unaff_A6;

  candidate_24aec();
  if ((*(byte *)(unaff_A6 + 0x9c) & 1) != 0) {
    return;
  }
  uVar1 = *(undefined4 *)(unaff_A6 + 0x40);
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + 0x70;
  sVar2 = candidate_1b14a();
  *(undefined4 *)(unaff_A6 + 0x40) = uVar1;
  if (sVar2 < 0x80) {
    sVar2 = 0x100 - sVar2;
  }
  for (sVar3 = 0; *(short *)(sVar3 + 0x24bb6) < sVar2; sVar3 = sVar3 + 4) {
  }
  *(char *)(unaff_A6 + 0x9d) = (char)*(undefined2 *)(sVar3 + 0x24bb8);
  return;
}
