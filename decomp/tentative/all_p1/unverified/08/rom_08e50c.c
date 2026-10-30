/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e50c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e50c(void)

{
  ushort uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar1 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x7e) = (uVar1 & 0xf) + 0xf;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x8e53a;
  candidate_8f4ce();
  candidate_427a();
  uVar1 = candidate_29c6();
  if ((uVar1 & 0x3f) == 0) {
    *unaff_A6 = candidate_8e6c2;
  }
  if ((*(char *)((int)unaff_A6 + 0x8e) != '\0') &&
     (*(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1,
     *(short *)((int)unaff_A6 + 0x7e) < 1)) {
    *unaff_A6 = candidate_8e72e;
  }
  uVar3 = false;
  candidate_8f960();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_8e6c2;
    uVar3 = 0;
  }
  candidate_8f8fa();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_8e6c2;
    uVar3 = 0;
  }
  candidate_8f8e0();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_8e6c2;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0xf) < 0x60) {
    sVar2 = 3;
  }
  else if (*(short *)(unaff_A6 + 0xf) < 0xc1) {
    sVar2 = 4;
  }
  else {
    sVar2 = 5;
  }
  uVar3 = (undefined1)sVar2;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar3 = *(undefined1 *)(sVar2 + 0x8fee0);
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar3;
  uVar3 = false;
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
