/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0683fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_683fa(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;

  candidate_4256();
  unaff_A6[0x14] = 0xe1a96;
  candidate_4e76();
  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  candidate_4520();
  candidate_146e8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x27) = 0;
  sVar1 = candidate_29f2();
  *(short *)(unaff_A6 + 0x25) = 0x38 - sVar1;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *(undefined4 *)((int)unaff_A6 + 0x62) = *(undefined4 *)((int)unaff_A6 + 0x8a);
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1436a();
  *(undefined2 *)((int)unaff_A6 + 0x8e) = *(undefined2 *)(unaff_A6 + 10);
  *unaff_A6 = candidate_684ca;
  return;
}
