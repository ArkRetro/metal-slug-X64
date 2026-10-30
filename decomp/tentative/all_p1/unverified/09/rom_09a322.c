/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a322.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a322(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
  *unaff_A6 = 0x9a346;
  uVar1 = 0;
  candidate_9ac86();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_9a364;
    uVar1 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
