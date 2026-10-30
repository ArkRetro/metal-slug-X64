/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a838.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a838(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1e) = (uVar1 & 0x1f) + 0x14;
  candidate_29c6();
  candidate_4256();
  *unaff_A6 = 0x5a870;
  candidate_1436a();
  candidate_427a();
  *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + -1;
  if (*(short *)(unaff_A6 + 0x1e) < 1) {
    *unaff_A6 = candidate_5a734;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    candidate_5a894();
    uVar2 = *(short *)((int)unaff_A6 + 0x72) != -1;
    if (*(short *)((int)unaff_A6 + 0x72) == -1) {
      candidate_406b6();
    }
    else {
      *unaff_A6 = candidate_9b596;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
