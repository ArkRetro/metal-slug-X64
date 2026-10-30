/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09bbd4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9bbd4(void)

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
  *unaff_A6 = 0x9bc30;
  uVar4 = 0;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9cd02();
  candidate_427a();
  candidate_9d2a8();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9bcf4;
    uVar4 = 0;
  }
  candidate_9d288();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9bf98;
    uVar4 = 0;
  }
  candidate_9d140();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9c23a;
    uVar4 = 0;
  }
  candidate_9d496();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_9c292;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar4 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  candidate_9cdb8();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *unaff_A6 = (int)candidate_9c2d0;
    uVar4 = false;
    candidate_1b206();
    if ((bool)uVar4) {
      *unaff_A6 = (int)candidate_9bcf4;
      uVar4 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_9d470();
    if ((bool)uVar4) {
      *unaff_A6 = (int)candidate_9c3d4;
    }
    if (*unaff_A6 == 0x9bc30) {
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
