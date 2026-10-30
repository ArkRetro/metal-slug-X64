/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e5a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e5a2(void)

{
  int iVar1;
  ushort uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  iVar1 = *(int *)((int)unaff_A6 + 0x72);
  if ((*(byte *)((int)unaff_A6 + 0x93) & 0x80) == 0) {
    iVar1 = -iVar1;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  uVar2 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x7e) = (uVar2 & 0xf) + 0xf;
  bVar4 = false;
  candidate_8f4a0();
  if (bVar4) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x8e5fa;
  uVar5 = false;
  candidate_8f51c();
  if ((bool)uVar5) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
    *unaff_A6 = candidate_8e650;
    uVar5 = 0;
  }
  candidate_427a();
  candidate_8f8fa();
  if (!(bool)uVar5) {
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  }
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    *unaff_A6 = candidate_8e650;
  }
  uVar5 = false;
  candidate_8f90a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_8e650;
    uVar5 = 0;
  }
  candidate_8f8e0();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_8e650;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0xf) < 0x60) {
    sVar3 = 3;
  }
  else if (*(short *)(unaff_A6 + 0xf) < 0xc1) {
    sVar3 = 4;
  }
  else {
    sVar3 = 5;
  }
  uVar5 = (undefined1)sVar3;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar5 = *(undefined1 *)(sVar3 + 0x8fee0);
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar5;
  uVar5 = false;
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar5 = 0;
  }
  candidate_8f432();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
