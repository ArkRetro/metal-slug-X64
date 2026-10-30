/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08dfa6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8dfa6(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
  if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 0x1e;
    *(undefined1 *)((int)unaff_A6 + 0x8f) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 4;
    candidate_4256();
    *unaff_A6 = 0x8e06e;
    candidate_8f4ce();
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
      *unaff_A6 = candidate_8e08a;
    }
  }
  else {
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
    *unaff_A6 = 0x8e000;
    candidate_8f4ce();
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 5;
    }
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      *unaff_A6 = candidate_8dfa6;
    }
  }
  uVar3 = 0;
  candidate_8e15a();
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
