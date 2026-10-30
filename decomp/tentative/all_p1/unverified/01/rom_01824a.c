/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01824a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1824a(void)

{
  short in_D0w;
  short sVar1;
  short in_D1w;
  short sVar2;
  short *psVar3;

  psVar3 = *(short **)((short)(in_D0w * 4) + 0x17d44);
  sVar1 = 0;
  sVar2 = *psVar3;
  do {
    if (sVar2 == 8) {
      return;
    }
    if (sVar2 == 0) {
      if (in_D1w == sVar1) {
        return;
      }
      sVar1 = sVar1 + 1;
    }
    psVar3 = (short *)((int)psVar3[1] + (int)psVar3);
    sVar2 = *psVar3;
  } while( true );
}
