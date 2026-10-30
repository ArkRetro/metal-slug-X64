/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bf28.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2bf28(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if (!(bool)in_CF) {
    sVar1 = candidate_1b12e();
    if (sVar1 < 0x80) {
      sVar1 = 0x100 - sVar1;
    }
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      iVar3 = 0x2c50e;
    }
    else {
      iVar3 = 0x2c520;
    }
    cVar2 = *(char *)(iVar3 + (short)((ushort)(sVar1 - 0x80U) >> 3));
    if (cVar2 == *(char *)(unaff_A6 + 0x8c)) {
      *(byte *)(unaff_A6 + 0x8c) = *(byte *)(unaff_A6 + 0x8c) & 7;
      return;
    }
    if (cVar2 < *(char *)(unaff_A6 + 0x8c)) {
      cVar2 = -1;
    }
    else {
      cVar2 = '\x01';
    }
    if ((_DAT_0010000e & 1) == 0) {
      *(char *)(unaff_A6 + 0x8c) = cVar2 + *(char *)(unaff_A6 + 0x8c);
    }
  }
  *(byte *)(unaff_A6 + 0x8c) = *(byte *)(unaff_A6 + 0x8c) & 7;
  return;
}
