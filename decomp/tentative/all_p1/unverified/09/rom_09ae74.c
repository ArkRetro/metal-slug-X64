/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ae74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ae74(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_4256();
  *unaff_A6 = 0x9ae9a;
  uVar3 = 0;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9cd02();
  candidate_427a();
  if ((*(byte *)(unaff_A6 + 0x24) & 1) != 0) {
    uVar3 = false;
    if (*(char *)((int)unaff_A6 + 0x8b) < '\x01') {
      candidate_9d2de();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_9b044;
        uVar3 = 0;
      }
      candidate_9cfca();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_9b13a;
        uVar3 = 0;
      }
      candidate_9cf8c();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_9b044;
        uVar3 = 0;
      }
    }
    else {
      cVar2 = *(char *)((int)unaff_A6 + 0x8b);
      uVar3 = cVar2 == '\0';
      *(char *)((int)unaff_A6 + 0x8b) = cVar2 + -1;
    }
  }
  candidate_9cffe();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9afac;
    uVar3 = 0;
  }
  candidate_9d20c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9b61c;
    uVar3 = 0;
  }
  candidate_9d19e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9b1b4;
    uVar3 = 0;
  }
  candidate_9d2a8();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9b2ec;
    uVar3 = 0;
  }
  candidate_9d140();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9b000;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar3 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  candidate_9cdb8();
  if ((bool)uVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_4e76();
      candidate_13f1e();
      *unaff_A6 = candidate_9b550;
      return;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *unaff_A6 = candidate_9b40e;
    uVar3 = false;
    candidate_1b206();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b42e;
      uVar3 = 0;
    }
  }
  candidate_9d470();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9c3d4;
  }
  return;
}
