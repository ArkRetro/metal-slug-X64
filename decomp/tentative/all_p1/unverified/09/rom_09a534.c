/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a534.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a534(void)

{
  short sVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_4256();
  unaff_A6[0xd] = 0xfffeab00;
  *unaff_A6 = 0x9a552;
  sVar1 = candidate_402e6();
  bVar3 = false;
  candidate_427a();
  if (bVar3) {
    *unaff_A6 = candidate_9a534;
  }
  if (sVar1 < 0) {
    *unaff_A6 = candidate_9a0f6;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_9a58e;
  }
  uVar4 = false;
  uVar2 = candidate_9ab9a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x83) = uVar2;
    *unaff_A6 = candidate_9a17e;
    uVar4 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
