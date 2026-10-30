/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032cae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_32cae(void)

{
  int extraout_A0;
  short *psVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if ((bool)in_CF) {
    for (psVar1 = (short *)0xe0776; *psVar1 != -1; psVar1 = psVar1 + 3) {
      candidate_8cdb2();
      *(short *)(extraout_A0 + 0x74) = psVar1[2];
    }
    *unaff_A6 = candidate_32d08;
  }
  return;
}
