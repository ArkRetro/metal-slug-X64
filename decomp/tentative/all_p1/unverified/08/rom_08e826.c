/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e826.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e826(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  *(undefined1 *)((int)unaff_A6 + 0x8d) = 4;
  iVar1 = *(int *)((int)unaff_A6 + 0x72);
  if (*(short *)(unaff_A6 + 0xf) < 0xa1) {
    iVar1 = -iVar1;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0x7f;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x80;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  bVar2 = false;
  candidate_8f4a0();
  if (bVar2) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x8e882;
  uVar3 = false;
  candidate_8f51c();
  if ((bool)uVar3) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
    *unaff_A6 = candidate_8e650;
    uVar3 = 0;
  }
  candidate_427a();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar3 = 0;
  }
  candidate_8f432();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
