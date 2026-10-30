/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0977dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_977dc(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined2 unaff_D4w;
  int unaff_A6;

  candidate_120f8();
  if ((*(byte *)(unaff_A6 + 0x5b) & 0x80) == 0) {
    sVar1 = 0x81;
    sVar2 = 0x80;
    sVar3 = 0x7c;
    sVar4 = 0x78;
  }
  else {
    sVar1 = 0x80;
    sVar2 = 0x81;
    sVar3 = 0x78;
    sVar4 = 0x7c;
  }
  if (CONCAT11((char)((ushort)unaff_D4w >> 8),*(undefined1 *)(unaff_A6 + sVar1)) == 0) {
    sVar1 = sVar2;
    sVar3 = sVar4;
  }
  *(undefined1 *)(unaff_A6 + sVar1) = 0;
  *(undefined2 *)(unaff_A6 + sVar3) = 0x8000;
  return;
}
