/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e72e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e72e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x8e750;
  candidate_8f4ce();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    *unaff_A6 = candidate_8e76c;
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
