/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e6c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e6c2(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar2;
  bool bVar3;

  candidate_8f90a();
  candidate_8f4a0();
  if ((bool)in_CF) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x8e6f8;
  uVar2 = false;
  candidate_8f51c();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
    *unaff_A6 = candidate_8e650;
    uVar2 = 0;
  }
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_8e5a2;
    bVar3 = false;
    candidate_8f8e0();
    if (bVar3) {
      *unaff_A6 = candidate_8e826;
    }
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0xf) < 0x60) {
    sVar1 = 3;
  }
  else if (*(short *)(unaff_A6 + 0xf) < 0xc1) {
    sVar1 = 4;
  }
  else {
    sVar1 = 5;
  }
  uVar2 = (undefined1)sVar1;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = *(undefined1 *)(sVar1 + 0x8fee0);
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar2;
  uVar2 = false;
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar2 = 0;
  }
  candidate_8f432();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
