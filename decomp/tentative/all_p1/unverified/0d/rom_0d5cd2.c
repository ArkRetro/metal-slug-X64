/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5cd2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_d5cd2(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x14;
  *(undefined1 *)((int)unaff_A6 + 0x79) = 2;
  *(undefined4 *)((int)unaff_A6 + 0x96) = 0xe90d4;
  *unaff_A6 = 0xd5cec;
  *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
  if (*(short *)((int)unaff_A6 + 0x72) < 1) {
    *unaff_A6 = candidate_d5d04;
  }
  uVar2 = 0;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_140c0();
  }
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_d5d5c;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x14;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x96);
  if (iVar1 != 0) {
    iVar1 = candidate_49e8();
  }
  return iVar1;
}
