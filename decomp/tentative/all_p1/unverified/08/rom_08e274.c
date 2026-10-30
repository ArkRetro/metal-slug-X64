/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e274.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e274(void)

{
  short sVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;

  candidate_8f662();
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar2;
  candidate_4256();
  *unaff_A6 = 0x8e2a4;
  candidate_8f4ce();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
  if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
    *unaff_A6 = candidate_8e1e2;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x7e);
  uVar2 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7e) = sVar1 + -1;
  candidate_8f7dc();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_8e2d8;
    uVar2 = 0;
  }
  candidate_8e15a();
  candidate_8f67c();
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
