/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e342.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e342(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
  if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar2;
    *(undefined4 *)((int)unaff_A6 + 0x7a) = 0;
    candidate_8f67c();
    candidate_8f662();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    uVar4 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar4;
    candidate_4256();
    *unaff_A6 = 0x8e2a4;
    candidate_8f4ce();
    candidate_427a();
    *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
    if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
      *unaff_A6 = candidate_8e1e2;
    }
    sVar1 = *(short *)((int)unaff_A6 + 0x7e);
    uVar4 = sVar1 == 0;
    *(short *)((int)unaff_A6 + 0x7e) = sVar1 + -1;
    candidate_8f7dc();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_8e2d8;
      uVar4 = 0;
    }
  }
  else {
    if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
      uVar4 = 2;
      uVar3 = 0xff;
    }
    else {
      uVar4 = 3;
      uVar3 = 0;
    }
    *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar4;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar3;
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar2;
    candidate_4256();
    *unaff_A6 = 0x8e39c;
    uVar4 = 0;
    candidate_8f4ce();
    candidate_427a();
    if (((bool)uVar4) && (*(char *)((int)unaff_A6 + 0x8e) != '\0')) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      *unaff_A6 = candidate_8e342;
    }
    uVar4 = 0;
  }
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
