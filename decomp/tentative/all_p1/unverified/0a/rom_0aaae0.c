/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aaae0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aaae0(void)

{
  char cVar1;
  char in_D0b;
  short in_D1w;
  short *psVar2;
  int unaff_A5;

  if (in_D0b == '\0') {
    psVar2 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar2 = (short *)(unaff_A5 + 0x462c);
  }
  if ((*psVar2 != 0) &&
     ((((cVar1 = *(char *)((int)psVar2 + 0x21), cVar1 == '\x01' || (cVar1 == '\x04')) ||
       (cVar1 == '\x03')) ||
      (((cVar1 == '\b' || (cVar1 == '\t')) || ((cVar1 == '\n' || (cVar1 == '\v')))))))) {
    if (in_D1w != 0) {
      if ((ushort)psVar2[10] <= (ushort)psVar2[0xc]) {
        return;
      }
      psVar2[0xb] = in_D1w + psVar2[0xb];
    }
    return;
  }
  return;
}
