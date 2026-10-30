/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e2d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e2d8(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if ((*(byte *)((int)unaff_A6 + 0x93) & 4) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar3;
    if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
      uVar3 = 2;
      uVar2 = 0xff;
    }
    else {
      uVar3 = 3;
      uVar2 = 0;
    }
    *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar3;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar2;
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
    candidate_4256();
    *unaff_A6 = 0x8e39c;
    uVar3 = 0;
    candidate_8f4ce();
    candidate_427a();
    if (((bool)uVar3) && (*(char *)((int)unaff_A6 + 0x8e) != '\0')) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      *unaff_A6 = candidate_8e342;
    }
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
    candidate_4256();
    *unaff_A6 = 0x8e308;
    candidate_8f4ce();
    candidate_427a();
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      *unaff_A6 = 0x8e32a;
    }
  }
  uVar3 = 0;
  candidate_8e15a();
  candidate_8f67c();
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
