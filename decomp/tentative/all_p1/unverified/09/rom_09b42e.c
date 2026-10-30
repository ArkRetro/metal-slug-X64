/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09b42e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9b42e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_4256();
  *unaff_A6 = 0x9b450;
  uVar2 = 0;
  candidate_9cdf2();
  candidate_9ce20();
  candidate_9cd02();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9ae74;
    uVar2 = 0;
  }
  candidate_9d140();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9b000;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar2 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  candidate_9cdb8();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_9d470();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_9c3d4;
    }
    return;
  }
  candidate_4e76();
  candidate_13f1e();
  *unaff_A6 = candidate_9b550;
  return;
}
