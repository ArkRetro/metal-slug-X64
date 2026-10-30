/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a230.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a230(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  bVar1 = false;
  candidate_1aeb0();
  if (bVar1) {
    if (*(short *)(extraout_A0 + 0x3c) < *(short *)(unaff_A6 + 0xf)) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
  }
  candidate_4256();
  unaff_A6[0xc] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x9a278;
  uVar2 = 0;
  candidate_9ac86();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9a290;
    uVar2 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
