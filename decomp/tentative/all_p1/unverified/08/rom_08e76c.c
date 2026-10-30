/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e76c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e76c(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined1 uVar4;
  short sVar3;
  undefined4 *unaff_A6;

  candidate_1b1dc();
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    uVar2 = 2;
    uVar4 = 0xff;
  }
  else {
    uVar2 = 3;
    uVar4 = 0;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
  candidate_4256();
  *unaff_A6 = 0x8e7e0;
  candidate_8f4ce();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 5;
  }
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    *unaff_A6 = candidate_8e786;
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
  uVar2 = (undefined1)sVar3;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = *(undefined1 *)(sVar3 + 0x8fee0);
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
