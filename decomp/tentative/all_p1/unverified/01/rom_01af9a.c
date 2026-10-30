/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01af9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1af9a(void)

{
  short sVar1;
  char cVar2;
  short sVar3;
  short sVar4;
  int unaff_A6;

  cVar2 = candidate_1ae8e();
  if (cVar2 == '\0') {
    return;
  }
  if (cVar2 == '\x01') {
    return;
  }
  if (cVar2 != '\x02') {
    sVar1 = *(short *)(unaff_A6 + 0x3c);
    sVar3 = sRam00101500 - sVar1;
    if (sRam00101500 < sVar1) {
      sVar3 = -sVar3;
    }
    sVar4 = sRam001015b0 - sVar1;
    if (sRam001015b0 < sVar1) {
      sVar4 = -sVar4;
    }
    if (sVar3 <= sVar4) {
      return;
    }
  }
  return;
}
