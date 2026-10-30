/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043792.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43792(void)

{
  short sVar1;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  if (*(char *)(unaff_A6 + 0x80) == '\0') {
    candidate_4256();
    return;
  }
  candidate_1af9a();
  if (bVar2) {
    sVar1 = 4;
  }
  else {
    sVar1 = candidate_43804();
  }
  *(short *)(unaff_A6 + 0x86) = sVar1;
  if ((short)(sVar1 + -4) < 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  candidate_4256();
  return;
}
