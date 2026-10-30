/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5d5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_d5d5c(void)

{
  undefined2 uVar2;
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x9e) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x28) = 0;
  candidate_13f1e();
  *(undefined4 *)((int)unaff_A6 + 0x9a) = 0xe94ac;
  *unaff_A6 = 0xd5d84;
  uVar3 = 0;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_140c0();
  }
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  *(short *)((int)unaff_A6 + 0x9e) = *(short *)((int)unaff_A6 + 0x9e) + -1;
  if (0x7fff < *(ushort *)((int)unaff_A6 + 0x9e)) {
    *(undefined2 *)((int)unaff_A6 + 0x9e) = 0xffff;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (*(short *)((int)unaff_A6 + 0x9e) < 1) {
      *unaff_A6 = candidate_d5e5c;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x14;
    }
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x96);
  if (iVar1 != 0) {
    iVar1 = candidate_49e8();
  }
  return iVar1;
}
