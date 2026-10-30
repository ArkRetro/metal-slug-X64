/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa994.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa994(void)

{
  short sVar1;
  byte bVar2;
  char in_D0b;
  int iVar3;
  int unaff_A5;
  int unaff_A6;

  if (in_D0b == '\0') {
    iVar3 = unaff_A5 + 0x4608;
  }
  else {
    iVar3 = unaff_A5 + 0x462c;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x5f);
  *(byte *)(unaff_A6 + 0x5f) = bVar2 & 0x7f;
  if (((bVar2 & 0x80) != 0) && ((*(byte *)(unaff_A6 + 0x5f) & 0x40) == 0)) {
    *(undefined2 *)(iVar3 + 0x16) = 0;
  }
  sVar1 = *(short *)(iVar3 + 0x16);
  *(undefined2 *)(iVar3 + 0x16) = 0;
  *(short *)(iVar3 + 0x18) = sVar1 + *(short *)(iVar3 + 0x18);
  sVar1 = *(short *)(iVar3 + 0x18);
  *(short *)(unaff_A6 + 0x4e) = sVar1;
  if (sVar1 == 0) {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
  }
  return;
}
