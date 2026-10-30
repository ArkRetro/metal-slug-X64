/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d17c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d17c(void)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  short sVar4;
  int unaff_A6;

  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  bVar3 = *(char *)(unaff_A6 + 0x83) + 4U & 0xf8;
  sVar4 = 0x80;
  if ((*(byte *)(unaff_A6 + 0x80) & 2) != 0) {
    sVar4 = 0x100;
  }
  if (bVar3 < 0x40) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
    sVar4 = -sVar4;
  }
  if ((*(byte *)(unaff_A6 + 0x80) & 1) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
  }
  sVar4 = sVar4 + *(short *)(unaff_A6 + 0x7a);
  if (sVar4 < 0) {
    sVar4 = 0x500;
  }
  if (0x500 < sVar4) {
    sVar4 = 0;
  }
  *(short *)(unaff_A6 + 0x7a) = sVar4;
  cVar1 = bVar3 - 0x40;
  if (cVar1 < '\0') {
    cVar1 = -cVar1;
  }
  bVar2 = (byte)(cVar1 + 4U) >> 3;
  bVar3 = -(bVar2 != *(byte *)(unaff_A6 + 0x81));
  if ((char)*(byte *)(unaff_A6 + 0x81) < (char)bVar2) {
    bVar3 = bVar3 & 1;
  }
  bVar2 = -(bVar3 != *(byte *)(unaff_A6 + 0x82));
  if ((char)*(byte *)(unaff_A6 + 0x82) < (char)bVar3) {
    bVar2 = bVar2 & 1;
  }
  *(char *)(unaff_A6 + 0x82) = bVar2 + *(char *)(unaff_A6 + 0x82);
  *(char *)(unaff_A6 + 0x81) = *(char *)(unaff_A6 + 0x82) + *(char *)(unaff_A6 + 0x81);
  candidate_4256();
  candidate_427a();
  return;
}
