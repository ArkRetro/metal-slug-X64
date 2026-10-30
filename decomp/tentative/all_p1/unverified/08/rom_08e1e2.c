/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e1e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e1e2(void)

{
  short sVar1;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_8f5de();
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  iVar2 = *(int *)((int)unaff_A6 + 0x72);
  if ((*(byte *)((int)unaff_A6 + 0x93) & 0x80) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  bVar3 = false;
  candidate_8f4a0();
  if (bVar3) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x8e232;
  uVar4 = false;
  candidate_8f51c();
  if ((bool)uVar4) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
    *unaff_A6 = candidate_8e274;
    uVar4 = 0;
  }
  candidate_427a();
  candidate_8f57c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_8e274;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7e);
  uVar4 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7e) = sVar1 + -1;
  candidate_8f7dc();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_8e2d8;
    uVar4 = 0;
  }
  candidate_8f3b6();
  candidate_8e15a();
  candidate_8f67c();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar4 = 0;
  }
  candidate_8f432();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
