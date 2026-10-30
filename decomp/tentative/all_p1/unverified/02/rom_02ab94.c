/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02ab94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2ab94(void)

{
  ushort uVar1;
  uint uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_120f8();
  *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) & 0x7f;
  if ((*(byte *)((int)unaff_A6 + 0x91) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(int *)((int)unaff_A6 + 0x72) = -(*(int *)((int)unaff_A6 + 0x62) >> 5);
  *unaff_A6 = 0x2abe4;
  candidate_146a8();
  uVar2 = *(int *)((int)unaff_A6 + 0x72) + *(int *)((int)unaff_A6 + 0x62);
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    uVar2 = 0;
  }
  *(uint *)((int)unaff_A6 + 0x62) = uVar2;
  if ((uVar2 & 0xffffff00) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  }
  bVar4 = false;
  if ((_DAT_0010000e & 7) == 0) {
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_00;
    }
    *(undefined4 *)(iVar3 + 0x30) = *(undefined4 *)((int)unaff_A6 + 0x72);
    uVar1 = *(ushort *)(iVar3 + 0x1e);
    bVar4 = 0xfffe < uVar1;
    *(ushort *)(iVar3 + 0x1e) = uVar1 + 1;
  }
  candidate_427a();
  if ((bVar4) && (*(int *)((int)unaff_A6 + 0x62) == 0)) {
    *unaff_A6 = candidate_2ac6c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_2ae46;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
