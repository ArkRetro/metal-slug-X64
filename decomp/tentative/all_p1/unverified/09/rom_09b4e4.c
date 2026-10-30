/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09b4e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9b4e4(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  undefined1 in_CF;

  candidate_9cd02();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_9b500;
  }
  unaff_A6[0x14] = 0;
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
