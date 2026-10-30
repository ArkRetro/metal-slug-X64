/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa898.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa898(void)

{
  char cVar1;
  char in_D0b;
  byte in_D1b;
  short extraout_D1w;
  byte bVar2;
  byte unaff_D2b;
  short *psVar3;
  int unaff_A5;
  byte bStack_d;

  if (in_D0b == '\0') {
    psVar3 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar3 = (short *)(unaff_A5 + 0x462c);
  }
  if ((((*psVar3 != 0) && (cVar1 = *(char *)((int)psVar3 + 0x21), cVar1 != '\x02')) &&
      (cVar1 != '\x05')) && ((cVar1 != '\x06' && (cVar1 != '\a')))) {
    if (cVar1 == '\x04') {
      candidate_c0764();
      if (extraout_D1w == 1) {
        return;
      }
      unaff_D2b = in_D1b;
      in_D1b = bStack_d;
      if (extraout_D1w == 0) {
        return;
      }
    }
    if (((unaff_D2b != 1) && (unaff_D2b != 2)) && (*(char *)((int)psVar3 + 0x1f) != 'c')) {
      if (unaff_D2b != 0) {
        *(byte *)(psVar3 + 0x10) = unaff_D2b;
      }
      bVar2 = *(byte *)((int)psVar3 + 0x1f) + in_D1b;
      if (CARRY1(*(byte *)((int)psVar3 + 0x1f),in_D1b)) {
        bVar2 = 99;
      }
      if (99 < bVar2) {
        bVar2 = 99;
      }
      *(byte *)((int)psVar3 + 0x1f) = bVar2;
      return;
    }
  }
  return;
}
