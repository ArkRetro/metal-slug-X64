/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048882.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48882(void)

{
  undefined2 uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;

  candidate_49ebe();
  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8600;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8600;
  unaff_A6[0x14] = 0xe0ab2;
  candidate_1ad6e();
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  if (*(char *)(unaff_A6[3] + 0x61) == '\0') {
    candidate_9ea8();
    candidate_120f8();
  }
  else {
    candidate_9ea8();
  }
  candidate_4520();
  candidate_40bc();
  *(undefined1 *)(extraout_A0 + 0x60) = 1;
  *unaff_A6 = candidate_4893e;
  return;
}
