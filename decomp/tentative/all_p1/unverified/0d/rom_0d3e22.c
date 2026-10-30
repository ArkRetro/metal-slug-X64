/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3e22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3e22(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_146e8();
  }
  *(ushort *)((int)unaff_A6 + 0xa2) = (ushort)((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0);
  iVar1 = -0xc000;
  if (*(short *)((int)unaff_A6 + 0xa2) != 0) {
    iVar1 = 0xc000;
  }
  unaff_A6[0xc] = iVar1;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    iVar1 = -iVar1;
  }
  if (iVar1 < 1) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_120f8();
  *unaff_A6 = 0xd3ea6;
  if (*(int *)((int)unaff_A6 + 0x9e) != 0) {
    candidate_4e76();
    *(undefined4 *)((int)unaff_A6 + 0x9e) = 0;
  }
  candidate_d5566();
  candidate_1ac38();
  candidate_427a();
  if (unaff_A6[0xc] == 0) {
    *unaff_A6 = candidate_d3f0a;
  }
  bVar2 = false;
  candidate_d5060();
  if (bVar2) {
    *unaff_A6 = candidate_d3f0a;
  }
  uVar3 = 0;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_49e8();
  }
  candidate_d50a8();
  candidate_d50ec();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d40a6;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d41de;
  }
  return;
}
