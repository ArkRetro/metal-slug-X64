/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8fdc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8fdc(void)

{
  short sVar1;
  char cVar2;
  short sVar3;
  ushort uVar4;
  ushort unaff_D5w;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x8e) - *(short *)(unaff_A6 + 0x3c);
  sVar1 = *(short *)(unaff_A6 + 0x90) - *(short *)(unaff_A6 + 0x40);
  if (sVar3 < 0) {
    sVar3 = -sVar3;
  }
  uVar4 = sVar1 + sVar3;
  if (sVar1 < 0) {
    uVar4 = uVar4 + sVar1 * -2;
  }
  if (unaff_D5w <= uVar4) {
    cVar2 = candidate_2eb4();
    if (sVar1 < 0x21) {
      *(char *)(unaff_A6 + 0x6e) = cVar2;
    }
    if (cVar2 != *(char *)(unaff_A6 + 0x6e)) {
      sVar3 = *(short *)(unaff_A6 + 0xa8) * -2;
      if (-1 < (char)(cVar2 - *(char *)(unaff_A6 + 0x6e))) {
        sVar3 = *(short *)(unaff_A6 + 0xa8) * 2;
      }
      *(short *)(unaff_A6 + 0x6e) = sVar3 + *(short *)(unaff_A6 + 0x6e);
    }
    candidate_d939a();
    return;
  }
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  return;
}
