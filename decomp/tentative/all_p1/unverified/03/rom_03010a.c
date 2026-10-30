/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03010a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3010a(void)

{
  char cVar1;
  short in_D0w;
  short *in_A0;
  short *psVar2;
  short *psVar3;

  psVar2 = in_A0 + 1;
  if (in_D0w <= *in_A0) {
    for (; psVar3 = psVar2, in_D0w != 0; in_D0w = in_D0w + -1) {
      do {
        psVar2 = (short *)((int)psVar3 + 1);
        cVar1 = *(char *)psVar3;
        psVar3 = psVar2;
      } while (cVar1 != '\0');
    }
    return;
  }
  return;
}
