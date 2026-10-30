/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08df92.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8df92(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined1 uVar3;
  undefined4 *unaff_A6;

  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar2;
  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    uVar2 = 2;
    uVar3 = 0xff;
  }
  else {
    uVar2 = 3;
    uVar3 = 0;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x92) = uVar3;
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
  uVar2 = 0;
  candidate_8e15a();
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
