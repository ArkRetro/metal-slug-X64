/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09b044.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9b044(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  candidate_9cd36();
  *unaff_A6 = 0x9b05e;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9ccc6();
  candidate_427a();
  uVar3 = false;
  if (*(char *)((int)unaff_A6 + 0x8a) < '\x01') {
    func_0x0009d1a6();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b0da;
      uVar3 = 0;
    }
    func_0x0009d2b0();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b0da;
      uVar3 = 0;
    }
    candidate_9cef8();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b0da;
      uVar3 = 0;
    }
    candidate_9cf32();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b0da;
      uVar3 = 0;
    }
    candidate_9cffe();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9b0da;
      uVar3 = 0;
    }
  }
  else {
    cVar2 = *(char *)((int)unaff_A6 + 0x8a);
    uVar3 = cVar2 == '\0';
    *(char *)((int)unaff_A6 + 0x8a) = cVar2 + -1;
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
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_4e76();
    candidate_13f1e();
    *unaff_A6 = candidate_9b550;
    return;
  }
  candidate_9d470();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9c3d4;
  }
  return;
}
