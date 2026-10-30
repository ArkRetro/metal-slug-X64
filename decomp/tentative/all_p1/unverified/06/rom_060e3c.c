/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060e3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_60e3c(void)

{
  short sVar1;
  int extraout_A0;
  short *psVar2;
  short *psVar3;

  psVar2 = (short *)0x60728;
  sVar1 = sRam00060728;
  while (-1 < sVar1) {
    candidate_40bc();
    candidate_1b220();
    *(undefined1 *)(extraout_A0 + 0x60) = *(undefined1 *)psVar2;
    *(char *)(extraout_A0 + 0x61) = (char)*psVar2;
    *(short *)(extraout_A0 + 0x72) = psVar2[1];
    psVar3 = psVar2 + 3;
    *(short *)(extraout_A0 + 0x6e) = psVar2[2];
    psVar2 = psVar2 + 4;
    *(short *)(extraout_A0 + 0x70) = *psVar3;
    sVar1 = *psVar2;
  }
  return;
}
