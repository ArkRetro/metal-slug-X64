/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x036486.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_36486(void)

{
  char cVar2;
  short sVar1;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x96) != '\x02') {
    return;
  }
  do {
    cVar2 = candidate_29f2();
  } while (cVar2 == *(char *)(unaff_A6 + 0x98));
  *(char *)(unaff_A6 + 0x98) = cVar2;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  sVar1 = candidate_365a4();
  if (-1 < (short)(sVar1 - *(short *)(unaff_A6 + 0x28))) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  sVar1 = candidate_29f2();
  *(short *)(unaff_A6 + 0x92) = sVar1 + 0x3c;
  return;
}
