/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04493e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4493e(void)

{
  char cVar1;
  short *psVar2;
  int unaff_A6;

  cVar1 = -1;
  psVar2 = (short *)0x4496c;
  while (*(short *)(unaff_A6 + 0x2c) < *psVar2) {
    cVar1 = cVar1 + '\x01';
    psVar2 = psVar2 + 1;
  }
  if ((cVar1 != *(char *)(unaff_A6 + 0xa5)) && (*(char *)(unaff_A6 + 0xa5) = cVar1, -1 < cVar1)) {
    candidate_44978();
  }
  return;
}
