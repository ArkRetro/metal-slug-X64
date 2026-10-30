/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09b68a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9b68a(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  int *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  bVar3 = false;
  candidate_40bc();
  iVar2 = extraout_A0;
  if (bVar3) {
    candidate_1b220();
    iVar2 = extraout_A0_00;
  }
  *(undefined1 *)(iVar2 + 0x90) = *(undefined1 *)(unaff_A6 + 0x24);
  if ((*(byte *)(unaff_A6 + 0x24) & 8) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9b6e6;
  uVar4 = 0;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9cd02();
  candidate_427a();
  candidate_9d250();
  if (!(bool)uVar4) {
    *unaff_A6 = (int)candidate_9b79c;
    uVar4 = 0;
  }
  candidate_9d272();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9b804;
    uVar4 = 0;
  }
  candidate_9d140();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9b000;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar4 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  candidate_9cdb8();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *unaff_A6 = (int)candidate_9b98a;
    uVar4 = false;
    candidate_1b206();
    if ((bool)uVar4) {
      *unaff_A6 = (int)candidate_9b9c2;
      uVar4 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_9d470();
    if ((bool)uVar4) {
      *unaff_A6 = (int)candidate_9c3d4;
    }
    if (*unaff_A6 == 0x9b6e6) {
      if ((*(byte *)(unaff_A6 + 0x24) & 8) != 0) {
        candidate_140c0();
      }
    }
    else {
      candidate_13f1e();
    }
    return;
  }
  candidate_4e76();
  candidate_13f1e();
  *unaff_A6 = (int)candidate_9b550;
  return;
}
