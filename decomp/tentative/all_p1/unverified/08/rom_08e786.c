/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e786.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e786(void)

{
  ushort uVar1;
  undefined2 uVar2;
  undefined1 uVar4;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
  if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
    *(undefined1 *)((int)unaff_A6 + 0x8f) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
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
    uVar5 = false;
    candidate_8f960();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_8e6c2;
      uVar5 = 0;
    }
    candidate_8f8fa();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_8e6c2;
      uVar5 = 0;
    }
    candidate_8f8e0();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_8e6c2;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
      uVar5 = 2;
      uVar4 = 0xff;
    }
    else {
      uVar5 = 3;
      uVar4 = 0;
    }
    *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar5;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar2;
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
