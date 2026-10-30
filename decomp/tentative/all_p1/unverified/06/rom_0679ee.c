/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0679ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_679ee(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4e76();
  candidate_4256();
  sVar1 = candidate_68a50();
  if (sVar1 == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x67a24;
  uVar2 = 0;
  candidate_8ccd4();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_67a40;
  }
  candidate_688d6();
  return;
}
