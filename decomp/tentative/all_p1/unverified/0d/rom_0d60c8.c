/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d60c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d60c8(void)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int unaff_A6;

  sVar2 = *(short *)(*(int *)(*(int *)(unaff_A6 + 0xc) + 0x8a) + 0x3c) - *(short *)(unaff_A6 + 0x3c)
  ;
  if (sVar2 < -0xff) {
    sVar2 = -0x100;
  }
  if (0xff < sVar2) {
    sVar2 = 0x100;
  }
  bVar1 = *(byte *)((short)((ushort)(sVar2 + 0x100U) >> 4) + 0xe9288) & 0xf;
  if (bVar1 != *(byte *)(unaff_A6 + 0xa6)) {
    cVar3 = '\x01';
    if ((char)(bVar1 - *(char *)(unaff_A6 + 0xa6)) < '\x01') {
      cVar3 = -1;
    }
    *(char *)(unaff_A6 + 0xa6) = cVar3 + *(char *)(unaff_A6 + 0xa6);
    return;
  }
  return;
}
