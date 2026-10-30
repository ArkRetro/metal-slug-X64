/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0468f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_468f4(void)

{
  short *psVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_21288();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_46964;
  }
  candidate_469b4();
  candidate_427a();
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
