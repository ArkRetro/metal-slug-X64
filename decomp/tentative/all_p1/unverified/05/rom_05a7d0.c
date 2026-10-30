/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a7d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a7d0(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x5a7e6;
  uVar1 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5a7fe;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    candidate_5a894();
    uVar1 = *(short *)((int)unaff_A6 + 0x72) != -1;
    if (*(short *)((int)unaff_A6 + 0x72) == -1) {
      candidate_406b6();
    }
    else {
      *unaff_A6 = candidate_9b596;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
