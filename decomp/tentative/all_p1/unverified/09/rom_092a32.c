/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x092a32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_92a32(void)

{
  undefined4 extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8810;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8810;
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    candidate_146e8();
  }
  candidate_14ae8();
  unaff_A6[0x23] = extraout_A0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x30;
  *(undefined1 *)(unaff_A6 + 0x1f) = 4;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 10;
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    *(int *)(extraout_A0_00 + 0x6e) = (int)unaff_A6 + 0x86;
    *(undefined1 *)(extraout_A0_00 + 0x72) = 0;
    *(undefined1 *)(extraout_A0_00 + 0x73) = 0;
    *(undefined1 *)(extraout_A0_00 + 0x74) = 0;
    *(undefined1 *)(extraout_A0_00 + 0x75) = 0;
  }
  *unaff_A6 = candidate_92ad8;
  return;
}
