/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0d64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0d64(void)

{
  char *pcVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  ushort in_D0w;
  char in_D1b;
  short sVar5;
  int unaff_A5;

  uVar3 = in_D0w & 1;
  sVar4 = uVar3 * 2;
  if (in_D1b != *(char *)(unaff_A5 + 0x471e + (int)(short)uVar3)) {
    return;
  }
  if (((DAT_0010f001 & 8) == 0) && (DAT_0010c2e7 == '\0')) {
    if (*(short *)(unaff_A5 + 0x4724 + (int)sVar4) == 0) {
      return;
    }
    sVar2 = *(short *)(unaff_A5 + 0x4724 + (int)sVar4);
    if (sVar2 != -1) {
      sVar5 = sVar2 + -1;
      if (sVar2 == 0) {
        sVar5 = 0;
      }
      *(short *)(unaff_A5 + 0x4724 + (int)sVar4) = sVar5;
    }
  }
  if (*(char *)(unaff_A5 + 0x471c + (int)(short)uVar3) == -1) {
    return;
  }
  if (*(byte *)(unaff_A5 + 0x471a + (int)(short)uVar3) <
      *(byte *)(unaff_A5 + 0x471c + (int)(short)uVar3)) {
    pcVar1 = (char *)(unaff_A5 + 0x471a + (int)(short)uVar3);
    *pcVar1 = *pcVar1 + '\x01';
    return;
  }
  return;
}
